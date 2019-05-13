#include "findkeysdialog.h"
#include "ui_findkeysdialog.h"
#include "LoggingCategories/loggingcategories.h"
#include "SelectKeyDateDialog/selectkeydatedialog.h"

#include <QFileDialog>
#include <QRegExpValidator>
#include <QMessageBox>

FindKeysDialog::FindKeysDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FindKeysDialog)
{
    ui->setupUi(this);

    createConnections();
    createModels();
    createUI();
}

FindKeysDialog::~FindKeysDialog()
{
    delete ui;
}


void FindKeysDialog::createConnections()
{
    connect(ui->radioButtonDatabase,&QRadioButton::clicked,this,&FindKeysDialog::slotCheckFindMethod);
    connect(ui->radioButtonFolder,&QRadioButton::clicked,this,&FindKeysDialog::slotCheckFindMethod);
    connect(this, &FindKeysDialog::signalUpdateLabelInfo,this,&FindKeysDialog::slotUpdateLabelInfo);
}

void FindKeysDialog::createUI()
{
    ui->radioButtonDatabase->setChecked(true);
    ui->comboBoxFirms->setEnabled(true);
    ui->lineEditPath->setEnabled(false);
    ui->toolButtonFolder->setEnabled(false);
    ui->pushButtonFind->setEnabled(false);

    ui->comboBoxFirms->setModel(modelFirms);
    ui->comboBoxFirms->setModelColumn(1);
    ui->comboBoxFirms->setCurrentIndex(-1);
    ui->comboBoxFirms->setCurrentText("Выберите организацию...");

    ui->lineEditZN->setValidator(new QRegExpValidator(QRegExp("[0-9]{10}"),ui->lineEditZN));
    m_currentFirmID=-1;
    ui->groupBoxRro->hide();
    ui->labelInfo->hide();
}

void FindKeysDialog::createModels()
{
    modelFirms = new QSqlQueryModel(this);
    modelFirms->setQuery("SELECT r.FIRM_ID, TRIM(r.NAME) as F FROM FIRMS r "
                         "WHERE r.ISACTIVE='T' "
                         "ORDER BY F");
    if(modelFirms->lastError().isValid()) {
        qCritical(logCritical()) << "Не удалось получить список фирм." << modelFirms->lastError().text();
    }
}

void FindKeysDialog::slotCheckFindMethod()
{
    if(ui->radioButtonFolder->isChecked()){
        ui->comboBoxFirms->setEnabled(false);
        ui->lineEditPath->setEnabled(true);
        ui->toolButtonFolder->setEnabled(true);
    }
    if(ui->radioButtonDatabase->isChecked()){
        ui->comboBoxFirms->setEnabled(true);
        ui->lineEditPath->setEnabled(false);
        ui->toolButtonFolder->setEnabled(false);
    }
    qInfo(logInfo()) << "Size RRO num" << m_rroZN.size();
    if((m_rroZN.size() > 0) && (m_rroZN.size() < 10))
        ui->pushButtonFind->setEnabled(false);
}

void FindKeysDialog::on_comboBoxFirms_activated(int idx)
{
    m_currentFirmID = modelFirms->data(modelFirms->index(idx,0)).toInt();
    qInfo(logInfo()) << "Selected firmID" << m_currentFirmID;
    ui->pushButtonFind->setEnabled(true);
}

void FindKeysDialog::on_toolButtonFolder_clicked()
{
    keyPath = QFileDialog::getExistingDirectory(nullptr, "Выберите папку с ключами", "");
    ui->lineEditPath->setText(keyPath);
    ui->pushButtonFind->setEnabled(true);
}

void FindKeysDialog::on_pushButtonFind_clicked()
{

    if(ui->radioButtonDatabase->isChecked()) {
        SelectKeyDateDialog *selectDateDlg = new SelectKeyDateDialog(this);
        selectDateDlg->exec();
        selectDateDlg->move(this->geometry().center().x() - selectDateDlg->geometry().center().x(),
                            this->geometry().center().y() - selectDateDlg->geometry().center().y());


        m_DateWhereStr.clear();

        if(selectDateDlg->result() == QDialog::Accepted){
            m_DateWhereStr = selectDateDlg->getWhereStr();
            databaseFindKey();
        }

    }

}


void FindKeysDialog::databaseFindKey()
{
    modelRRO = new QSqlQueryModel();
    QString strSQL = QString("SELECT r.KEY_ID, r.FIRM_ID, TRIM(r.POSNUMBER), r.DAT, r.KEYDATA, r.DAT_EXPIRE FROM KEYS r "
            "WHERE r.FIRM_ID= %1 AND r.").arg(m_currentFirmID)
            + m_DateWhereStr +
            " ORDER BY r.POSNUMBER";
    ui->groupBoxRro->show();
    modelRRO->setQuery(strSQL);
    rowCount = modelRRO->rowCount();
    ui->groupBox->setDisabled(rowCount>0);
    modelRRO->setHeaderData(2,Qt::Horizontal,"№ ЭККА");
    modelRRO->setHeaderData(3,Qt::Horizontal,"Дата создания:");
    modelRRO->setHeaderData(5,Qt::Horizontal,"Действует до:");
    ui->tableView->setModel(modelRRO);
    ui->tableView->hideColumn(0);
    ui->tableView->hideColumn(1);
    ui->tableView->hideColumn(4);

    ui->tableView->verticalHeader()->hide();
    ui->tableView->resizeColumnsToContents();
    emit signalUpdateLabelInfo("Найдено ключей "+QString::number(rowCount));

    ui->labelInfo->show();
    ui->plainTextEdit->setReadOnly(true);
    QSqlQuery q;
    q.prepare("SELECT o.NAME FROM FIRMKEYOPTIONS r "
              "LEFT JOIN KEYOPTIONS o ON o.KEYOPTION_ID=r.KEYOPTION_ID "
              "where r.FIRM_ID= :firmID "
              "order by r.KEYOPTION_ID");
    q.bindValue(":firmID", m_currentFirmID);
    q.exec();
    while(q.next()){
        ui->plainTextEdit->appendPlainText(q.value(0).toString());
    }
}


void FindKeysDialog::on_lineEditZN_textChanged(const QString &arg1)
{
    m_rroZN = arg1;
    if(m_rroZN.size()==10)
        ui->pushButtonFind->setEnabled(true);
}


void FindKeysDialog::on_pushButton_clicked()
{
    this->reject();
}

void FindKeysDialog::on_toolButtonSelectAll_clicked()
{
    ui->tableView->selectAll();

    emit signalUpdateLabelInfo("Найдено ключей "+QString::number(rowCount)+". Выбрано "+QString::number(rowCount)+".");
}

void FindKeysDialog::on_toolButtonUnSelectAll_clicked()
{
    ui->tableView->clearSelection();
    emit signalUpdateLabelInfo("Найдено ключей "+QString::number(rowCount));
}


void FindKeysDialog::slotUpdateLabelInfo(QString str)
{
    ui->labelInfo->setText(str);
}



void FindKeysDialog::on_tableView_clicked(const QModelIndex &index)
{
    Q_UNUSED(index);
    QModelIndexList selection = ui->tableView->selectionModel()->selectedRows();
    emit signalUpdateLabelInfo("Найдено ключей "+QString::number(rowCount)+". Выбрано "+QString::number(selection.count())+".");
}
