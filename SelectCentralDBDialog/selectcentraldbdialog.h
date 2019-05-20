#ifndef SELECTCENTRALDBDIALOG_H
#define SELECTCENTRALDBDIALOG_H

#include "centraldbdialog.h"

#include <QDialog>
#include <QSqlTableModel>
#include <QSqlError>

namespace Ui {
class SelectCentralDBDialog;
}

class SelectCentralDBDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectCentralDBDialog(QWidget *parent = nullptr);
    ~SelectCentralDBDialog();

private slots:
    void on_pushButtonCancel_clicked();

    void on_pushButtonAdd_clicked();
    void slotUpdateModel();
    void slotEditRecord(QModelIndex index);
private:
    Ui::SelectCentralDBDialog *ui;
    QSqlTableModel *modelConnections;
private:
    void createUI();
    void setupModel();
};

#endif // SELECTCENTRALDBDIALOG_H
