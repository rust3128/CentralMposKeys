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

public:
    explicit FindKeysDialog(QWidget *parent = nullptr);
    ~FindKeysDialog();

private slots:
    void slotCheckFindMethod();
    void on_comboBoxFirms_activated(int index);

    void on_toolButtonFolder_clicked();

    void on_pushButtonFind_clicked();

    void on_lineEditZN_textChanged(const QString &arg1);

private:
    Ui::FindKeysDialog *ui;
    QSqlQueryModel *modelFirms;
    QSqlQueryModel *modelRRO;
    int m_currentFirmID;
    QString keyPath;
    QString m_rroZN;
    QString m_dateCreate;
    QString m_dateFinish;
private:
    void createConnections();
    void createUI();
    void createModels();
    void databaseFindKey();
};

#endif // FINDKEYSDIALOG_H
