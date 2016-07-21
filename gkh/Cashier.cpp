#include "Cashier.h"
#include "ui_Cashier.h"
#include "Payment.h"

Cashier::Cashier(QWidget *parent) :
QDialog(parent),
ui(new Ui::Cashier)
{
	ui->setupUi(this);
	this->setWindowFlags(Qt::Window);
	this->setWindowTitle("Кассир");

	AccountModel = new QSqlRelationalTableModel(this,QSqlDatabase::database());
	AccountModel->setTable("CashierView");
	qDebug()<< AccountModel->select();
	QSqlRecord rec = AccountModel->record();
    //AccountModel->setSort(rec.indexOf("fio"),Qt::AscendingOrder);
	qDebug()<< AccountModel->select();
	ui->tw->setModel(AccountModel);
	ui->tw->resizeColumnsToContents();

	ui->widget->SetModels(AccountModel);
	connect(this, SIGNAL(sendAccountData(QModelIndex)), ui->widget,\
			SLOT(recieveData(QModelIndex)));
	connect(ui->widget, SIGNAL(getCurrentIndex()), this,\
			SLOT(onAccountFindButtonClick()));
	connect(ui->widget, SIGNAL(sendCurrentIndex(QModelIndex)), this,\
			SLOT(recieveAccountCurrentIndex(QModelIndex)));
}

//запрет закрытия при нажатии ESC
void Cashier::keyPressEvent(QKeyEvent *e) {
	if(e->key() != Qt::Key_Escape)
		QDialog::keyPressEvent(e);
}

void Cashier::recieveUserID(int user)
{
	this->systemUserID = user;
}

Cashier::~Cashier()
{
	delete ui;
}

void Cashier::recieveAccountCurrentIndex(const QModelIndex &index)
{
	ui->tw->setCurrentIndex(index);
	ui->tw->setFocus();
}
void Cashier::onAccountFindButtonClick()
{
	emit sendAccountData(ui->tw->currentIndex());
}

void Cashier::on_tw_doubleClicked(const QModelIndex &index)
{
	Payment* pay = new Payment;
	connect(this,SIGNAL(sendAccountID(int)),pay,SLOT(recieveAccountID(int)));
	QSqlRecord rec = AccountModel->record(index.row());
	emit sendAccountID(rec.value("id").toInt());
	connect(this,SIGNAL(sendUser(int)),pay,SLOT(recieveUserID(int)));
	emit sendUser(this->systemUserID);
	pay->exec();
	pay->deleteLater();

	QModelIndex ind= ui->tw->currentIndex();
	AccountModel->select();
	ui->tw->setCurrentIndex(ind);

}
