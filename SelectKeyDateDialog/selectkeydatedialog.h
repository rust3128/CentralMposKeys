#ifndef SELECTKEYDATEDIALOG_H
#define SELECTKEYDATEDIALOG_H

#include <QDialog>

namespace Ui {
class SelectKeyDateDialog;
}

class SelectKeyDateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectKeyDateDialog(QWidget *parent = nullptr);
    ~SelectKeyDateDialog();
    QString getWhereStr();

private slots:
    void slotSelectFilter();
    void on_pushButtonCancel_clicked();

    void on_pushButtonOk_clicked();

private:
    Ui::SelectKeyDateDialog *ui;
    QString m_whereStr;
private:
    void createUI();
    void createConnections();
};

#endif // SELECTKEYDATEDIALOG_H
