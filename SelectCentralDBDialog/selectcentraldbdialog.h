#ifndef SELECTCENTRALDBDIALOG_H
#define SELECTCENTRALDBDIALOG_H

#include "centraldbdialog.h"

#include <QDialog>
#include <QSqlTableModel>
#include <QSqlError>
#include <QSqlRecord>

namespace Ui {
class SelectCentralDBDialog;
}

class SelectCentralDBDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectCentralDBDialog(QWidget *parent = nullptr);
    ~SelectCentralDBDialog();
    QSqlRecord currentConnData();

private slots:
    void on_pushButtonCancel_clicked();

    void on_pushButtonAdd_clicked();
    void slotUpdateModel();
    void slotEditRecord();
    void on_tableView_doubleClicked(const QModelIndex &idx);

    void on_pushButtonSelect_clicked();

private:
    Ui::SelectCentralDBDialog *ui;
    QSqlTableModel *modelConnections;
    QSqlRecord connRecord;
private:
    void createUI();
    void setupModel();
};

#endif // SELECTCENTRALDBDIALOG_H
