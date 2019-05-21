#include "selectcentraldbdialog.h"
#include "ui_selectcentraldbdialog.h"

SelectCentralDBDialog::SelectCentralDBDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectCentralDBDialog)
{
    ui->setupUi(this);

    setupModel();
    createUI();
}

SelectCentralDBDialog::~SelectCentralDBDialog()
{
    delete ui;
}

void SelectCentralDBDialog::on_pushButtonCancel_clicked()
{
    this->reject();
}

void SelectCentralDBDialog::createUI()
{
    ui->tableView->setModel(modelConnections);
    ui->tableView->setColumnHidden(0,true);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->verticalHeader()->hide();
    ui->tableView->verticalHeader()->setMinimumSectionSize(ui->tableView->verticalHeader()->minimumSectionSize());
    ui->tableView->selectRow(0);
//    connect(ui->tableView, &QAbstractItemView::doubleClicked, this, &SelectCentralDBDialog::slotEditRecord);
    connect(ui->pushButtonEdit, &QPushButton::clicked, this, &SelectCentralDBDialog::slotEditRecord);

}

void SelectCentralDBDialog::setupModel()
{
    QSqlDatabase db = QSqlDatabase::database("options");

    modelConnections = new QSqlTableModel(this, db);
    modelConnections->setTable("connections");

    modelConnections->select();
    QStringList  headers = {"ID", "Имя", "Сервер", "База", "Пользователь", "Пароль"};

    for(int i = 0; i<modelConnections->columnCount(); ++i){
        modelConnections->setHeaderData(i, Qt::Horizontal, headers[i]);
    }
}

void SelectCentralDBDialog::on_pushButtonAdd_clicked()
{
    CentralDbDialog *addConnectionsDlg = new CentralDbDialog();
    connect(addConnectionsDlg, &CentralDbDialog::signalReady, this, &SelectCentralDBDialog::slotUpdateModel);
    addConnectionsDlg->setWindowTitle("Новое соединение");
    addConnectionsDlg->exec();

    addConnectionsDlg->move(this->geometry().center().x() - addConnectionsDlg->geometry().center().x(),
                       this->geometry().center().y() - addConnectionsDlg->geometry().center().y());
}

void SelectCentralDBDialog::slotUpdateModel()
{
    modelConnections->select();
    ui->tableView->resizeColumnsToContents();
}

void SelectCentralDBDialog::slotEditRecord()
{
    QModelIndex index=ui->tableView->selectionModel()->currentIndex();

    CentralDbDialog *addConnectionsDlg = new CentralDbDialog(index.row());
    connect(addConnectionsDlg, &CentralDbDialog::signalReady, this, &SelectCentralDBDialog::slotUpdateModel);
    addConnectionsDlg->setWindowTitle("Редактировать соединение");
    addConnectionsDlg->exec();

    addConnectionsDlg->move(this->geometry().center().x() - addConnectionsDlg->geometry().center().x(),
                       this->geometry().center().y() - addConnectionsDlg->geometry().center().y());
}
