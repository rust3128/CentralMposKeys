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


        m_dateCreate.clear();
        m_dateFinish.clear();
        if(selectDateDlg->result() == QDialog::Accepted){
            databaseFindKey();
        }

    }

}


void FindKeysDialog::databaseFindKey()
{
    modelRRO = new QSqlQueryModel();

}


void FindKeysDialog::on_lineEditZN_textChanged(const QString &arg1)
{
    m_rroZN = arg1;
    if(m_rroZN.size()==10)
        ui->pushButtonFind->setEnabled(true);
}

