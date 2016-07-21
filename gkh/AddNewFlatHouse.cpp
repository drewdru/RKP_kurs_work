#include "AddNewHouse.h"
#include "ui_AddNewHouse.h"

#include <QtSQL>
AddNewHouse::AddNewHouse(QWidget *parent) :
QDialog(parent),
ui(new Ui::AddNewHouse)
{
	ui->setupUi(this);
	this->setWindowTitle("Добавить новый дом в базу");
	QSqlTableModel* seterModel =
		new QSqlTableModel(this,QSqlDatabase::database());
	seterModel->setTable("material_of_house");
	qDebug()<< seterModel->select();
	QSqlRecord rec = seterModel->record();
	for(int nRow =0; nRow<seterModel->rowCount();nRow++)
	{
		rec = seterModel->record(nRow);
		ui->material->addItem(rec.value("name").toString(),\
								rec.value("id").toInt());
	}
	seterModel->setTable("house_type");
	qDebug()<< seterModel->select();
	for(int nRow =0; nRow<seterModel->rowCount();nRow++)
	{
		rec = seterModel->record(nRow);
		ui->housetype->addItem(rec.value("name").toString(),\
								rec.value("id").toInt());
	}
	seterModel->setTable("houses");
	qDebug()<< seterModel->select();
}

AddNewHouse::~AddNewHouse()
{
	delete ui;
}

void AddNewHouse::on_pushButton_5_clicked()
{
	QString str;
	QSqlQuery query;
	// Добавляем данные в базу
	QString strF =
	"INSERT INTO houses (address, levels, doorways, space,"
	"material,type,elevator) "
	"VALUES(%1, %2, %3, %4,%5,%6,%7);";
	str =	strF.arg(ui->HouseNewAddress->text())
				.arg(ui->levels->text())
				.arg(ui->doorways->text())
				.arg(ui->space->text())
				.arg(ui->material->currentData().toString())
				.arg(ui->housetype->currentData().toString())
				.arg(ui->elevator->text());
	if (!query.exec(str)) {
		qDebug() << "Unable to make insert operation";
	}
	qDebug() << query.lastError();
	this->close();
}
