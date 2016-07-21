#include "EmergencyBid.h"
#include "ui_EmergencyBid.h"

#include "webpageparser.h"
EmergencyBid::EmergencyBid(QWidget *parent) :
QDialog(parent),
ui(new Ui::EmergencyBid)
{
	ui->setupUi(this);

	ui->movementDate->setDateTime(QDateTime::currentDateTime());
	ui->movementDate->setDisplayFormat("dd.MM.yyyy hh:mm");

	EmergencyModel = new QSqlRelationalTableModel(this,QSqlDatabase::database());
	WorkerModel = new QSqlRelationalTableModel(this,QSqlDatabase::database());
	VehicleModel = new QSqlRelationalTableModel(this,QSqlDatabase::database());

	QSqlQuery q;
	q.exec("SELECT * FROM workertype;");
	while(q.next())
	{
		ui->workerTypeCBox->addItem(q.value("name").toString(),q.value("id").toString());
	}
	ui->workerTypeCBox->setCurrentIndex(-1);

	q.exec("SELECT * FROM vehicletype;");
	while(q.next())
	{
		ui->vehicleTypeCBox->addItem(q.value("name").toString(),q.value("id").toString());
	}
	ui->vehicleTypeCBox->setCurrentIndex(-1);

	q.exec("SELECT * FROM work_space;");
	while(q.next())
	{
		ui->spaceCBox->addItem(q.value("address").toString());
	}
	ui->spaceCBox->setCurrentIndex(-1);

}

EmergencyBid::~EmergencyBid()
{
	delete ui;
}

void EmergencyBid::recieveEmergencyID(int account)
{
	this->EmergencyID = account;
	EmergencyModel->setTable("repairview");
	EmergencyModel->setFilter("id='"+QString::number(EmergencyID)+"'");
	qDebug()<<EmergencyModel->select();
	ui->tableView->setModel(EmergencyModel);
	ui->tableView->setCurrentIndex(EmergencyModel->index(0,0));
	ui->tableView->resizeColumnsToContents();

	WorkerModel->setTable("workers_list_view");
	WorkerModel->setFilter("repair_tickets_id='"+QString::number(EmergencyID)+"'");
	qDebug()<<WorkerModel->select();
	ui->workerView->setModel(WorkerModel);
	ui->workerView->resizeColumnsToContents();
	QSqlRecord rec = WorkerModel->record();
	ui->workerView->setColumnHidden(rec.indexOf("repair_tickets_id"),true);
	ui->workerView->setColumnHidden(rec.indexOf("worker_id"),true);

	VehicleModel->setTable("mov_list_view");
	VehicleModel->setFilter("repair_tickets_id='"+QString::number(EmergencyID)+"'");
	qDebug()<<VehicleModel->select();
	ui->vehicleView->setModel(VehicleModel);
	ui->vehicleView->resizeColumnsToContents();
}

void EmergencyBid::recieveUserID(int user)
{
	this->systemUserID = user;
}

void EmergencyBid::on_workerTypeCBox_currentIndexChanged(int index)
{
	QString querySTR ="SELECT id,fio FROM worker WHERE worker_type = %1;";
	querySTR = querySTR.arg(ui->workerTypeCBox->currentData().toString());
	QSqlQuery q;
	q.exec(querySTR);
	ui->workerCBox->clear();
	while(q.next())
	{
		ui->workerCBox->addItem(q.value("fio").toString(),q.value("id").toString());
	}
}

void EmergencyBid::on_vehicleTypeCBox_currentIndexChanged(int index)
{
	QString querySTR =	"SELECT id,license_number "\
						"FROM vehicle WHERE vehicle_type = %1;";
	querySTR = querySTR.arg(ui->vehicleTypeCBox->currentData().toString());
	QSqlQuery q;
	q.exec(querySTR);
	ui->vehicleCBox->clear();
	while(q.next())
	{
		ui->vehicleCBox->addItem(q.value("license_number").toString(),q.value("id").toString());
	}
}


void EmergencyBid::on_addWorker_clicked()
{
	/*int ind = 0;
	WorkerModel->insertRow(ind);
	QSqlRecord rec = WorkerModel->record();
	WorkerModel->setData(WorkerModel->index(ind, rec.indexOf("repair_tickets_id")),\
						this->EmergencyID);
	WorkerModel->setData(WorkerModel->index(ind, rec.indexOf("worker_id")),\
						ui->workerCBox->currentData().toInt());
	if (!WorkerModel->submitAll()) {
		qDebug() << "Insertion error!";
	}
	WorkerModel->select();
	ui->workerView->resizeColumnsToContents();*/
	QString str;
	QSqlQuery query;
	// Добавляем данные в базу
	QString strF =
	"INSERT INTO workers_list (repair_tickets_id, worker_id) "
	"VALUES(%1, %2);";
	str =	strF.arg(QString::number(this->EmergencyID),ui->workerCBox->currentData().toString());
	if (!query.exec(str)) {
		qDebug() << "Unable to make insert operation";
	}
	qDebug() << query.lastError();
	WorkerModel->select();
	ui->workerView->resizeColumnsToContents();
}

void EmergencyBid::on_deleteWorker_clicked()
{
	QSqlRecord rec = WorkerModel->record(ui->workerView->currentIndex().row());
	QString querySTR = "DELETE FROM workers_list "\
		"WHERE repair_tickets_id =%1 AND worker_id = %2";
	querySTR = querySTR.arg(rec.value("repair_tickets_id").toString(),
		rec.value("worker_id").toString());

	QSqlQuery q;
	q.exec(querySTR);
	qDebug()<<q.lastError();
	WorkerModel->select();
	ui->workerView->resizeColumnsToContents();
}

void EmergencyBid::on_addVehicle_clicked()
{
	QString from = ui->spaceCBox->currentText();
	QSqlRecord rec = EmergencyModel->record(ui->tableView->currentIndex().row());
	QString to =rec.value("address").toString();

	QString urlText = "http://maps.googleapis.com/maps/api/directions/json?origin=%1&destination=%2&sensor=false";
	urlText = urlText.arg(from,to);
	QUrl url;
	url.setUrl(urlText);
	WebPageParser* a = new WebPageParser(url);
	connect(a, SIGNAL(sendData(QString)), this,SLOT(recieveHTMLData(QString)));
}

void EmergencyBid::recieveHTMLData(QString str)
{
	QStringList a = str.split("\"value\" :", QString::SkipEmptyParts);
	a = a.at(0).split("\"text\" : \"",QString::SkipEmptyParts);
	a = a.last().split("\"",QString::SkipEmptyParts);
	a = a.at(0).split(" км",QString::SkipEmptyParts);
	QString distance = a.at(0);
	distance.replace(",",".");
	this->addMovLog(distance.toFloat());
}

void EmergencyBid::addMovLog(float dist)
{
	QString querySTR = "INSERT INTO movements_log"\
		"(vehicle,distance,date,mov_from) "\
		"VALUES(%1,%2,STR_TO_DATE('%3', '%d.%m.%Y %H:%i'),'%4')";
	querySTR = querySTR.arg(ui->vehicleCBox->currentData().toString(),
		QString::number(dist),ui->movementDate->text(),
		ui->spaceCBox->currentText());

	QSqlQuery q;
	q.exec(querySTR);
	qDebug()<<q.lastError();	
	querySTR.clear();


	q.exec("SELECT MAX(id) FROM movements_log;");
	q.next();
	QString id = q.value("MAX(id)").toString();
	QSqlRecord rec = WorkerModel->record(ui->workerView->currentIndex().row());

	qDebug()<<id;

	querySTR = "INSERT INTO movings_list (repair_tickets_id,mov)"\
		"VALUES(%1,%2)";
	querySTR = querySTR.arg(QString::number(EmergencyID),id);
	q.exec(querySTR);
	qDebug()<<q.lastError();
	VehicleModel->select();
	ui->vehicleView->resizeColumnsToContents();
}

void EmergencyBid::on_deleteVehicle_clicked()
{
	QSqlRecord rec = VehicleModel->record(ui->vehicleView->currentIndex().row());
	QString querySTR = "DELETE FROM movings_list "\
		"WHERE repair_tickets_id =%1 AND mov = %2";
	querySTR = querySTR.arg(rec.value("repair_tickets_id").toString(),
		rec.value("mov").toString());

	QSqlQuery q;
	q.exec(querySTR);
	qDebug()<<q.lastError();
	VehicleModel->select();
	ui->vehicleView->resizeColumnsToContents();
}
