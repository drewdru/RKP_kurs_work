#include "EmergencyService.h"
#include "ui_EmergencyService.h"

#include <QDebug>
#include "EmergencyBid.h"
EmergencyService::EmergencyService(QWidget *parent) :
QDialog(parent),
ui(new Ui::EmergencyService)
{
	ui->setupUi(this);
	this->setWindowFlags(Qt::Window);
	this->setWindowTitle("Аварийная служба");

	/*rec = repairModel->record();
	repairModel->setHeaderData( rec.indexOf("id"), Qt::Horizontal, QObject::tr("№") );
	repairModel->setHeaderData( rec.indexOf("license_number"), Qt::Horizontal, QObject::tr("Номер транспорта") );
	repairModel->setHeaderData( rec.indexOf("vehicletype"), Qt::Horizontal, QObject::tr("Тип транспорта") );
	repairModel->setHeaderData( rec.indexOf("garage"), Qt::Horizontal, QObject::tr("Адрес гаража") );
	ui->repairView->resizeColumnsToContents();*/
	EmergencyModel = new QSqlRelationalTableModel(this,QSqlDatabase::database());
	EmergencyModel->setTable("repairview");
	EmergencyModel->setFilter("repair_state != 'Не активно'");
	qDebug()<< EmergencyModel->select();
	QSqlRecord rec = EmergencyModel->record();

	ui->tableView->setModel(EmergencyModel);	
	ui->tableView->setCurrentIndex(EmergencyModel->index(0,0));
	ui->tableView->resizeColumnsToContents();

	ui->widget->SetModels(EmergencyModel);
	connect(this, SIGNAL(sendEmergencyData(QModelIndex)), ui->widget,\
			SLOT(recieveData(QModelIndex)));
	connect(this, SIGNAL(sendEmergencyStates(QString)), ui->widget,\
			SLOT(recieveState(QString)));
	emit sendEmergencyStates("repair_state != 'Не активно'");
	connect(ui->widget, SIGNAL(getCurrentIndex()), this,\
			SLOT(onEmergencyFindButtonClick()));
	connect(ui->widget, SIGNAL(sendCurrentIndex(QModelIndex)), this,\
			SLOT(recieveEmergencyCurrentIndex(QModelIndex)));

	ui->stateBox->addItem("Ожидает","1");
	ui->stateBox->addItem("Выполнено","2");
	ui->stateBox->addItem("Отклонено","3");
	ui->stateBox->addItem("В работе","4");

	QString strQuery = "SELECT * FROM vehicletype;";
	QSqlQuery q;
	q.exec(strQuery);
	while(q.next())
	{
		ui->graphFilterCBox->addItem(q.value("name").toString(),q.value("id").toString());
	}
	initPayPlotV();

	strQuery = "SELECT * FROM workertype;";
	q.exec(strQuery);
	while(q.next())
	{
		ui->graphFilterCBox_2->addItem(q.value("name").toString(),q.value("id").toString());
	}
	initPayPlotW();
}

EmergencyService::~EmergencyService()
{
	delete ui;
}
//запрет закрытия при нажатии ESC
void EmergencyService::keyPressEvent(QKeyEvent *e) {
	if(e->key() != Qt::Key_Escape)
		QDialog::keyPressEvent(e);
}

void EmergencyService::recieveUserID(int user)
{
	this->systemUserID = user;
}

void EmergencyService::recieveEmergencyCurrentIndex(const QModelIndex &index)
{
	ui->tableView->setCurrentIndex(index);
	ui->tableView->setFocus();
}
void EmergencyService::onEmergencyFindButtonClick()
{
	emit sendEmergencyData(ui->tableView->currentIndex());
}

void EmergencyService::on_stateButton_clicked()
{
	EmergencyBid* eb = new EmergencyBid;
	connect(this,SIGNAL(sendEmergencyID(int)),eb,SLOT(recieveEmergencyID(int)));
	QSqlRecord rec = EmergencyModel->record(ui->tableView->currentIndex().row());
	emit sendEmergencyID(rec.value("id").toInt());
	connect(this,SIGNAL(sendUser(int)),eb,SLOT(recieveUserID(int)));
	emit sendUser(this->systemUserID);
	eb->showMaximized();
	eb->exec();
	eb->deleteLater();
	QModelIndex ind= ui->tableView->currentIndex();
	EmergencyModel->select();
	ui->tableView->setCurrentIndex(ind);
}

void EmergencyService::on_changeSButton_clicked()
{
	QSqlRecord rec = EmergencyModel->record(ui->tableView->currentIndex().row());
	QSqlQuery query;
	query.prepare("UPDATE repair_tickets SET state =:value1  WHERE id=:value2;");
	query.bindValue( ":value1", ui->stateBox->currentData().toInt());
	query.bindValue( ":value2", rec.value("id").toInt());
	if (!query.exec()) {
		qDebug() << query.lastError();
	}
	QModelIndex ind= ui->tableView->currentIndex();
	EmergencyModel->select();
	ui->tableView->setCurrentIndex(ind);
}

void EmergencyService::on_pushButton_clicked()
{
}

void EmergencyService::on_updatesumButton_clicked()
{
	QSqlRecord rec = EmergencyModel->record(ui->tableView->currentIndex().row());
	QSqlQuery query;
	query.prepare("UPDATE repair_tickets SET sum =:value1  WHERE id=:value2;");
	query.bindValue( ":value1", ui->sumDSBox->value());
	query.bindValue( ":value2", rec.value("id").toInt());
	if (!query.exec()) {
		qDebug() << query.lastError();
	}
	QModelIndex ind= ui->tableView->currentIndex();
	qDebug() <<EmergencyModel->select();
	ui->tableView->setCurrentIndex(ind);
}




