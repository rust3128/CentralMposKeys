#include "selectkeydatedialog.h"
#include "ui_selectkeydatedialog.h"
#include "LoggingCategories/loggingcategories.h"

SelectKeyDateDialog::SelectKeyDateDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectKeyDateDialog)
{
    ui->setupUi(this);
    createConnections();
    createUI();
}

SelectKeyDateDialog::~SelectKeyDateDialog()
{
    delete ui;
}

QString SelectKeyDateDialog::getWhereStr()
{
    return m_whereStr;
}

void SelectKeyDateDialog::createUI()
{
    ui->radioButtonAll->setChecked(true);
    ui->dateEdit->setDate(QDate::currentDate());
    ui->dateEdit->hide();
}

void SelectKeyDateDialog::createConnections()
{
    connect(ui->radioButtonAll, &QRadioButton::clicked, this, &SelectKeyDateDialog::slotSelectFilter);
    connect(ui->radioButtonCreate, &QRadioButton::clicked, this, &SelectKeyDateDialog::slotSelectFilter);
    connect(ui->radioButtonFinish, &QRadioButton::clicked, this, &SelectKeyDateDialog::slotSelectFilter);
}

void SelectKeyDateDialog::slotSelectFilter()
{
    if(ui->radioButtonAll->isChecked()){
        ui->dateEdit->hide();
    } else {
        ui->dateEdit->show();
    }
}

void SelectKeyDateDialog::on_pushButtonCancel_clicked()
{
    this->reject();
}

void SelectKeyDateDialog::on_pushButtonOk_clicked()
{
    m_whereStr.clear();
    if(ui->radioButtonCreate->isChecked()){
        m_whereStr = "AND r.DAT > '" + ui->dateEdit->date().toString("yyyy-MM-dd")+"'";
    }
    if(ui->radioButtonFinish->isChecked()){
        m_whereStr = "AND r.DAT_EXPIRE = '"+ ui->dateEdit->date().toString("yyyy-MM-dd")+"'";
    }

    this->accept();
}
