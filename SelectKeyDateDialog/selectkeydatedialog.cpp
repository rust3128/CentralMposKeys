#include "selectkeydatedialog.h"
#include "ui_selectkeydatedialog.h"

SelectKeyDateDialog::SelectKeyDateDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectKeyDateDialog)
{
    ui->setupUi(this);
    createUI();
}

SelectKeyDateDialog::~SelectKeyDateDialog()
{
    delete ui;
}

void SelectKeyDateDialog::createUI()
{
    ui->radioButtonAll->setChecked(true);
    ui->dateEdit->hide();
}
