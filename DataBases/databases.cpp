#include "databases.h"
#include "LoggingCategories/loggingcategories.h"
#include "ConnectionSettingDialog/connectionsettingdialog.h"
#include <QSettings>
#include <QFile>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>

#define DATABASE_NAME "CentralMposKeys.opt"
#define DATABASE_HOSTNAME "CentralMposKeys"

DataBases::DataBases(QObject *parent) : QObject(parent)
{

}

bool DataBases::connectCentralDatabase()
{
    bool result = true;
    QSettings settings("CentralMposKeys.cfg", QSettings::IniFormat);

    QSqlDatabase db = QSqlDatabase::addDatabase("QIBASE");

    settings.beginGroup("DATABASE");
    db.setHostName(settings.value("HostName").toString());
    db.setDatabaseName(settings.value("DataBase").toString());
    db.setUserName(settings.value("User").toString());
    db.setPassword(settings.value("Password").toString());
    settings.endGroup();

    if(!db.open()) {
        qCritical(logCritical()) <<  "Не возможно подключиться к базе данных." << endl << "Причина:" << db.lastError().text();
        int rez = QMessageBox::question(nullptr, QObject::tr("Ошибка подключения"),
                              QString("Не могу установить соединение с базой данных.\nПричина: %1\nПроверить настройки подключения?").arg(db.lastError().text()),
                              QMessageBox::Yes | QMessageBox::No);
        if(rez == QMessageBox::Yes) {
            ConnectionSettingDialog *connDlg = new ConnectionSettingDialog();
            int result = connDlg->exec();
            if(result == QDialog::Accepted) {
                qInfo(logInfo()) << "Сохранение настроек подключения.";
            }
        }
        result = false;
    } else {
        qInfo(logInfo()) << "Успешное подключение к центральной базе данных.";
        result = connectOptions();

    }

    return result;
}

bool DataBases::connectOptions()
{
    bool rez = true;

    if(QFile(DATABASE_NAME).exists()){
        qInfo(logInfo()) << "Открываем файл настроек приложения.";
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","options");
        db.setHostName(DATABASE_HOSTNAME);
        db.setDatabaseName(DATABASE_NAME);
        if(db.open()){
            qInfo(logInfo()) << "Файл настроек открыт успешно";
            rez = true;
        } else {
            qCritical(logCritical()) << "Не удалось открыть файл настроек приложения.";
            rez = false;
        }
    } else {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","options");
        db.setHostName(DATABASE_HOSTNAME);
        db.setDatabaseName(DATABASE_NAME);
        db.open();
        QStringList listSQL;
        QSqlQuery q = QSqlQuery(db);

        listSQL << "CREATE TABLE `connections` ( "
                   "`connID`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, "
                   "`connName`	TEXT NOT NULL, "
                   "`connServer`	TEXT NOT NULL, "
                   "`connDatabase`	TEXT NOT NULL, "
                   "`connUser`	TEXT NOT NULL, "
                   "`connPass`	TEXT NOT NULL)";


        for (int i =0;i<listSQL.size();++i) {
            q.exec(listSQL.at(i));
        }
        qInfo(logInfo()) << "Создан файл настроек приложения.";
        rez = true;

    }

    return rez;
}
