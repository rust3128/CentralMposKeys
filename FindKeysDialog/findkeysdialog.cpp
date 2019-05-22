#include "findkeysdialog.h"
#include "ui_findkeysdialog.h"




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
    QString curPath = QDir::currentPath()+"/Keys";

    dir = QDir(curPath);
    if(!dir.exists()){
        dir.mkdir(curPath);
    }
    dir.cd(curPath);

    ui->lineEditPath->setText(dir.absolutePath());
    ui->groupBoxProgress->hide();
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
        ui->comboBoxFirms->setCurrentIndex(-1);
    }
    if(ui->radioButtonDatabase->isChecked()){
        ui->comboBoxFirms->setEnabled(true);
        ui->lineEditPath->setEnabled(false);
        ui->toolButtonFolder->setEnabled(false);
    }
    if((m_rroZN.size() > 0) && (m_rroZN.size() < 10))
        ui->pushButtonFind->setEnabled(false);
}

void FindKeysDialog::on_comboBoxFirms_activated(int idx)
{
    m_currentFirmID = modelFirms->data(modelFirms->index(idx,0)).toInt();
    m_currentFirmName = modelFirms->data(modelFirms->index(idx,1)).toString();
    ui->pushButtonFind->setEnabled(true);
}

void FindKeysDialog::on_toolButtonFolder_clicked()
{
    keyPath = QFileDialog::getExistingDirectory(nullptr, "Выберите папку с ключами",dir.absolutePath());
    ui->lineEditPath->setText(keyPath);
    ui->pushButtonFind->setEnabled(true);
}

void FindKeysDialog::on_pushButtonFind_clicked()
{


    if(ui->radioButtonDatabase->isChecked()) {
        ui->groupBoxProgress->show();
        SelectKeyDateDialog *selectDateDlg = new SelectKeyDateDialog(this);
        selectDateDlg->exec();
        selectDateDlg->move(this->geometry().center().x() - selectDateDlg->geometry().center().x(),
                            this->geometry().center().y() - selectDateDlg->geometry().center().y());
        m_DateWhereStr.clear();

        if(selectDateDlg->result() == QDialog::Accepted){
            m_DateWhereStr = selectDateDlg->getWhereStr();
            databaseFindKey();
        }
        ui->groupBoxProgress->hide();
    }
    if(ui->radioButtonFolder->isChecked()){
        fileFindKey();

    }
    ui->tableView->verticalHeader()->hide();
    ui->tableView->resizeColumnsToContents();
    ui->tableView->verticalHeader()->setMinimumSectionSize(ui->tableView->verticalHeader()->minimumSectionSize());
}


void FindKeysDialog::databaseFindKey()
{

    QApplication::processEvents();
    QString strSQLWhere = "";
    modelRRO = new QSqlQueryModel();
    QString strSQL = "SELECT r.KEY_ID, r.FIRM_ID, TRIM(r.POSNUMBER), r.DAT, r.KEYDATA, r.DAT_EXPIRE FROM KEYS r WHERE ";

    if(m_currentFirmID >= 0 && m_DateWhereStr.size() == 0){
        strSQL += QString("r.FIRM_ID= %1").arg(m_currentFirmID);
        strSQL += " AND r.POSNUMBER LIKE '";
        strSQL += (m_rroZN.size() == 0) ? "%'" : m_rroZN +"%'";
     }

    if(m_currentFirmID >= 0 && m_DateWhereStr.size() > 0){
        strSQL += QString("r.FIRM_ID= %1 AND %2 " ).arg(m_currentFirmID).arg(m_DateWhereStr);
        strSQL += " AND r.POSNUMBER LIKE '";
        strSQL += (m_rroZN.size() == 0) ? "%'" : m_rroZN +"%'";
     }

    if(m_currentFirmID == -1 && m_DateWhereStr.size() > 0){
        strSQL += m_DateWhereStr;
        strSQL += " AND r.POSNUMBER LIKE '";
        strSQL += (m_rroZN.size() == 0) ? "%'" : m_rroZN +"%'";
     }

    if(m_currentFirmID == -1 && m_DateWhereStr.size() == 0){
        strSQL += " r.POSNUMBER LIKE '";
        strSQL += (m_rroZN.size() == 0) ? "%'" : m_rroZN +"%'";
     }

    strSQL += " ORDER BY r.POSNUMBER";

//     qInfo(logInfo()) << "Str SQL " << strSQL;
    ui->groupBoxRro->show();
    modelRRO->setQuery(strSQL);
    while(modelRRO->canFetchMore())
        modelRRO->fetchMore();
    rowCount = modelRRO->rowCount();
    ui->groupBox->setDisabled(rowCount>0);
    modelRRO->setHeaderData(2,Qt::Horizontal,"№ ЭККА");
    modelRRO->setHeaderData(3,Qt::Horizontal,"Дата создания:");
    modelRRO->setHeaderData(5,Qt::Horizontal,"Действует до:");
    ui->tableView->setModel(modelRRO);
    ui->tableView->hideColumn(0);
    ui->tableView->hideColumn(1);
    ui->tableView->hideColumn(4);

    emit signalUpdateLabelInfo("Найдено ключей "+QString::number(rowCount));

    ui->labelInfo->show();
    ui->plainTextEdit->setReadOnly(true);

    getKeyOptions();



}

void FindKeysDialog::getKeyOptions()
{
    QSqlQuery q;


    if(m_currentFirmID < 0){
        if(ui->radioButtonDatabase->isChecked()){
            m_currentFirmID= modelRRO->data(modelRRO->index(0,1)).toInt();
        } else {
            q.prepare("select k.FIRM_ID, f.name from keys k "
                      "LEFT JOIN FIRMS f ON f.FIRM_ID = k.FIRM_ID "
                      "where k.POSNUMBER = :posNumber");
            q.bindValue(":posNumber", modelFromFile->data(modelFromFile->index(0,0),Qt::DisplayRole).toString());
            if(!q.exec()) qInfo(logInfo()) << "NO get firmID" << q.lastError().text();
            q.next();
            m_currentFirmID = q.value(0).toInt();
            ui->plainTextEdit->appendHtml("<b>"+q.value(1).toString()+"</b>");
        }

    }

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



void FindKeysDialog::fileFindKey()
{
    DataKeysFileClass dk;

    QDir dir(ui->lineEditPath->text());
    QStringList nameFilter;
    nameFilter << "*.key";
    QFileInfoList list = dir.entryInfoList(nameFilter, QDir::Files);
    QFileInfo fileInfo;
    foreach(fileInfo, list) {
        dk.setPosnumber(fileInfo.fileName().mid(8,10));
        dk.setDatExpire(QDate::fromString(fileInfo.fileName().mid(19,10),"yyyy-MM-dd"));
        QFile f(fileInfo.absoluteFilePath());
        if(!f.open(QIODevice::ReadOnly)){
            qCritical(logCritical()) << "Не могу отрыть файл" << fileInfo.fileName();
            continue;
        }
        dk.setKeyData(QByteArray(f.readAll()));
        m_fileKeyList.append(dk);
    }
    rowCount= m_fileKeyList.size();
    ui->groupBox->setDisabled(rowCount>0);
    emit signalUpdateLabelInfo("Загружено "+QString::number(rowCount)+" файлов с ключами.");

//    qInfo(logInfo()) << "READ files " << m_fileKeyList.size();
    ui->labelInfo->show();
    ui->groupBoxRro->show();
    modelFromFile = new KeysFileModel(m_fileKeyList);
    ui->tableView->setModel(modelFromFile);
    ui->tableView->hideColumn(2);
    ui->pushButtonSaveFolder->setEnabled(false);
    getKeyOptions();
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

void FindKeysDialog::on_pushButtonSaveFolder_clicked()
{
    QString pathKey = dir.absolutePath()+"/" + m_currentFirmName;
    dir.mkdir(pathKey);
//    dir.cd(pathKey);
    QModelIndexList selection = ui->tableView->selectionModel()->selectedRows();
    int selCount=selection.size();
    if(selCount == 0){
        QMessageBox::information(this, "Ошибка", "Не выбрано ни одного ключа!");
        return;
    }
    QString fileName;
    QProgressDialog prDlg(this);
    prDlg.setRange(0, selCount);
    prDlg.setLabelText("Создание файлов с ключами.");
    prDlg.setCancelButton(nullptr);
    prDlg.show();
    int progress=0;
    for(int i =0;i<selCount;i++){
        QModelIndex idx = selection.at(i);
        fileName = "mposdrv_"+modelRRO->data(modelRRO->index(idx.row(),2)).toString()+"_"+modelRRO->data(modelRRO->index(idx.row(),5)).toDate().toString("yyyy-MM-dd") +".key";
        fileName = pathKey+"/"+fileName;
//        qInfo(logInfo()) << fileName;
        QFile newKeyFile(fileName);
        if(newKeyFile.open(QIODevice::WriteOnly)){
            newKeyFile.write(modelRRO->data(modelRRO->index(idx.row(),4)).toByteArray());
        }
        newKeyFile.close();
        QApplication::processEvents();
        ++progress;
        prDlg.setValue(progress);
    }
    prDlg.deleteLater();
    QMessageBox::information(this,"Информация",
                             QString("Сохранено %1 афйлов с ключами.\nВ папку %2")
                             .arg(selCount)
                             .arg(pathKey));
}

void FindKeysDialog::on_pushButtonSaveDB_clicked()
{
    QModelIndexList selection = ui->tableView->selectionModel()->selectedRows();
    int selCount=selection.size();
    if(selCount == 0){
        QMessageBox::information(this, "Ошибка", "Не выбрано ни одного ключа!");
        return;
    }

    SelectCentralDBDialog *centralDbDlg = new SelectCentralDBDialog(this);
    int res = centralDbDlg->exec();
    centralDbDlg->move(this->geometry().center().x() - centralDbDlg->geometry().center().x(),
                       this->geometry().center().y() - centralDbDlg->geometry().center().y());
    if(res == QDialog::Rejected){
        return;
    }

    QSqlRecord connInfo = centralDbDlg->currentConnData();
//    qInfo(logInfo()) << connInfo;

    QSqlDatabase dbc = QSqlDatabase::addDatabase("QIBASE","dbase");

    dbc.setHostName(connInfo.value(2).toString());
    dbc.setDatabaseName(connInfo.value(3).toString());
    dbc.setUserName(connInfo.value(4).toString());
    dbc.setPassword(connInfo.value(5).toString());

    if(!dbc.open()) {
        QMessageBox::critical(this, "Ошибка соединения с базой данных",
                              QString("Не могу соединится с базой данных\n%1:%2.\nПричина:\n%3")
                              .arg(dbc.hostName())
                              .arg(dbc.databaseName())
                              .arg(dbc.lastError().text()));
        return;
    }
    if(ui->radioButtonDatabase->isChecked())
        insertKeyFromDatabase();
    else
        insertKeyFromFile();

}

void FindKeysDialog::insertKeyFromFile()
{
    QSqlDatabase db = QSqlDatabase::database("dbase");
    QSqlQuery q = QSqlQuery(db);
    QModelIndexList selection = ui->tableView->selectionModel()->selectedRows();
    static int selCount = selection.size();
    QProgressDialog pd(this);
    pd.setRange(0,selCount);
    pd.setLabelText("Добавление ключей в базу данных");
    QProgressBar pb;
    pb.setFormat("%v из %m");
    pd.setBar(&pb);
    pd.setCancelButton(nullptr);
    pd.show();
    int progress = 0;

    for(int i=0;i<selCount; ++i){

    }

}

void FindKeysDialog::insertKeyFromDatabase()
{
    QSqlDatabase db = QSqlDatabase::database("dbase");
    QSqlQuery q = QSqlQuery(db);




}
