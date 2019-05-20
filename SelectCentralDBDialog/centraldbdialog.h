#ifndef CENTRALDBDIALOG_H
#define CENTRALDBDIALOG_H

#include <QDialog>
#include <QSqlTableModel>
#include <QDataWidgetMapper>

namespace Ui {
class CentralDbDialog;
}

class CentralDbDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CentralDbDialog(int row = -1, QWidget *parent = nullptr);
    ~CentralDbDialog();

signals:
    void signalReady();
private slots:
    void on_buttonBox_accepted();

private:
    Ui::CentralDbDialog *ui;
    QSqlTableModel *model;
    QDataWidgetMapper *mapper;
private:
    void setupModel();
    void createUI();
    void accept();
};

#endif // CENTRALDBDIALOG_H
