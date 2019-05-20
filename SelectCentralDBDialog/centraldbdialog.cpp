#include "centraldbdialog.h"
#include "ui_centraldbdialog.h"

CentralDbDialog::CentralDbDialog(int row, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CentralDbDialog)
{
    ui->setupUi(this);

    setupModel();

    if(row == -1) {
        model->insertRow(model->rowCount(QModelIndex()));
        mapper->toLast();
    } else {
        mapper->setCurrentModelIndex(model->index(row,0));
    }
    createUI();
}

CentralDbDialog::~CentralDbDialog()
{
    delete ui;
}

void CentralDbDialog::setupModel()
{
    QSqlDatabase db = QSqlDatabase::database("options");
    model = new QSqlTableModel(this,db);
    model->setTable("connections");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();

    mapper = new QDataWidgetMapper();
    mapper->setModel(model);
    mapper->addMapping(ui->lineEditName,1);
    mapper->addMapping(ui->lineEditServer,2);
    mapper->addMapping(ui->lineEditDataBase,3);
    mapper->addMapping(ui->lineEditUser,4);
    mapper->addMapping(ui->lineEditPassword,5);

    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);


}

void CentralDbDialog::createUI()
{

}

void CentralDbDialog::accept()
{

}

void CentralDbDialog::on_buttonBox_accepted()
{
    mapper->submit();
    model->submitAll();
    emit signalReady();
    this->close();
}
