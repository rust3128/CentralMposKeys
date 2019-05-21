#ifndef FINDKEYSDIALOG_H
#define FINDKEYSDIALOG_H


#include "LoggingCategories/loggingcategories.h"
#include "SelectKeyDateDialog/selectkeydatedialog.h"
#include "DataKeysFileClass/datakeysfileclass.h"
#include "KeysFileModel/keysfilemodel.h"
#include "SelectCentralDBDialog/selectcentraldbdialog.h"


#include <QRegExpValidator>
#include <QMessageBox>
#include <QDateTime>
#include <QFile>
#include <QProgressDialog>
#include <QDialog>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlQueryModel>
#include <QFileDialog>


namespace Ui {
class FindKeysDialog;
}

class FindKeysDialog : public QDialog
{
    Q_OBJECT

signals:
    void signalUpdateLabelInfo(QString);
public:
    explicit FindKeysDialog(QWidget *parent = nullptr);
    ~FindKeysDialog();

private slots:
    void slotCheckFindMethod();
    void on_comboBoxFirms_activated(int index);
    void on_toolButtonFolder_clicked();
    void on_pushButtonFind_clicked();
    void on_lineEditZN_textChanged(const QString &arg1);
    void on_pushButton_clicked();
    void on_toolButtonSelectAll_clicked();
    void on_toolButtonUnSelectAll_clicked();
    void slotUpdateLabelInfo(QString str);
    void on_tableView_clicked(const QModelIndex &index);

    void on_pushButtonSaveFolder_clicked();

    void on_pushButtonSaveDB_clicked();

private:
    Ui::FindKeysDialog *ui;
    QSqlQueryModel *modelFirms;
    QSqlQueryModel *modelRRO;
    KeysFileModel *modelFromFile;
    int m_currentFirmID;
    QString m_currentFirmName;
    int rowCount;
    QString keyPath;
    QString m_rroZN;
    QString m_DateWhereStr;
    QDir dir;
    QList<DataKeysFileClass> m_fileKeyList;

private:
    void createConnections();
    void createUI();
    void createModels();
    void databaseFindKey();
    void fileFindKey();

};

#endif // FINDKEYSDIALOG_H
