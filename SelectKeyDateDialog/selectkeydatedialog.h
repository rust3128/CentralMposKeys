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

private:
    Ui::SelectKeyDateDialog *ui;
private:
    void createUI();
};

#endif // SELECTKEYDATEDIALOG_H
