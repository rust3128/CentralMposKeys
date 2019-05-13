#ifndef FINDKEYSDIALOG_H
#define FINDKEYSDIALOG_H

#include <QDialog>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlQueryModel>


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

private:
    Ui::FindKeysDialog *ui;
    QSqlQueryModel *modelFirms;
    QSqlQueryModel *modelRRO;
    int m_currentFirmID;
    int rowCount;
    QString keyPath;
    QString m_rroZN;
    QString m_DateWhereStr;

private:
    void createConnections();
    void createUI();
    void createModels();
    void databaseFindKey();

};

#endif // FINDKEYSDIALOG_H
