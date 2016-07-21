#include "OperatorBD.h"
#include "ui_OperatorBD.h"

OperatorBD::OperatorBD(QWidget *parent) :
QDialog(parent),
ui(new Ui::OperatorBD)
{
	ui->setupUi(this);

	this->setWindowFlags(Qt::Window);
	this->setWindowTitle("Оператор");

	OperatorBDModel = new QSqlRelationalTableModel(this,QSqlDatabase::database());
	OperatorBDModel->setTable("repairoperatorview");
	OperatorBDModel->setFilter("State = 'Не активно'");
	qDebug()<< OperatorBDModel->select();
	QSqlRecord rec = OperatorBDModel->record();

	ui->tableView->setModel(OperatorBDModel);
	ui->tableView->setCurrentIndex(OperatorBDModel->index(0,0));
	ui->tableView->resizeColumnsToContents();
	ui->tableView->setColumnHidden(rec.indexOf("State"),true);

	workerModel = new QSqlRelationalTableModel(this,QSqlDatabase::database());
	workerModel->setTable("workerview");
	qDebug()<< workerModel->select();
	ui->tw->setModel(workerModel);
	ui->tw->resizeColumnsToContents();

	ui->frame_3->hide();
	flat_counterModel = new QSqlRelationalTableModel(this,QSqlDatabase::database());
	flat_counterModel->setTable("flat_counter_view");
	flat_counterModel->select();
	ui->flat_counterView->setModel(flat_counterModel);
	ui->flat_counterView->resizeColumnsToContents();

	house_counterModel = new QSqlRelationalTableModel(this,QSqlDatabase::database());
	house_counterModel->setTable("house_counter_view");
	house_counterModel->select();
	ui->house_counterView->setModel(house_counterModel);
	ui->house_counterView->resizeColumnsToContents();
	ui->house_counterView->setCurrentIndex(house_counterModel->index(0,0));

	ui->widget->SetModels(OperatorBDModel);
	connect(this, SIGNAL(sendOperatorBDData(QModelIndex)), ui->widget,\
			SLOT(recieveData(QModelIndex)));
	connect(this, SIGNAL(sendOperatorBDStates(QString)), ui->widget,\
			SLOT(recieveState(QString)));
	emit sendOperatorBDStates("State = 'Не активно'");
	connect(ui->widget, SIGNAL(getCurrentIndex()), this,\
			SLOT(onOperatorBDFindButtonClick()));
	connect(ui->widget, SIGNAL(sendCurrentIndex(QModelIndex)), this,\
			SLOT(recieveOperatorBDCurrentIndex(QModelIndex)));

	ui->frame->hide();
	ui->frame_2->hide();

	QSqlQueryModel model;
	model.setQuery("SELECT * FROM works_type");
	if (model.lastError().isValid()) {
		qDebug() << model.lastError();
	}
	QSqlRecord addRec = model.record();
	for(int nRow =0; nRow<model.rowCount();nRow++)
	{
		addRec = model.record(nRow);
		ui->work_typeCBox->addItem(addRec.value("name").toString(),\
								addRec.value("id").toInt());
	}
	model.setQuery("SELECT id,address FROM houses");
	if (model.lastError().isValid()) {
		qDebug() << model.lastError();
	}
	addRec = model.record();
	for(int nRow =0; nRow<model.rowCount();nRow++)
	{
		addRec = model.record(nRow);
		ui->addressCBox->addItem(addRec.value("address").toString(),\
		addRec.value("id").toInt());
		ui->address_counterCBox->addItem(addRec.value("address").toString(),\
		addRec.value("id").toInt());
	}

	model.setQuery("SELECT id,name FROM counter_type");
	if (model.lastError().isValid()) {
		qDebug() << model.lastError();
	}
	addRec = model.record();
	for(int nRow =0; nRow<model.rowCount();nRow++)
	{
		addRec = model.record(nRow);
		ui->counter_typeCBox->addItem(addRec.value("name").toString(),\
		addRec.value("id").toInt());
	}

	model.setQuery("SELECT id,address FROM work_space");
	if (model.lastError().isValid()) {
		qDebug() << model.lastError();
	}
	addRec = model.record();
	for(int nRow =0; nRow<model.rowCount();nRow++)
	{
		addRec = model.record(nRow);
		ui->officeCBox->addItem(addRec.value("address").toString(),\
		addRec.value("id").toInt());
	}
	model.setQuery("SELECT id,name FROM workertype");
	if (model.lastError().isValid()) {
		qDebug() << model.lastError();
	}
	addRec = model.record();
	for(int nRow =0; nRow<model.rowCount();nRow++)
	{
		addRec = model.record(nRow);
		ui->worker_typeCBox->addItem(addRec.value("name").toString(),\
		addRec.value("id").toInt());
	}

	workerTypeModel = new QSqlTableModel(this,QSqlDatabase::database());
	workerTypeModel->setTable("workertype");
	workerTypeModel->select();
	ui->widget_2->SetModels(workerModel,workerTypeModel,true);
	connect(this, SIGNAL(sendData(QModelIndex)), ui->widget,\
			SLOT(recieveData(QModelIndex)));
	connect(ui->widget_2, SIGNAL(getCurrentIndex()), this,\
			SLOT(onFindButtonClick()));
	connect(ui->widget_2, SIGNAL(sendCurrentIndex(QModelIndex)), this,\
			SLOT(recieveCurrentIndex(QModelIndex)));	

	ui->EndDate->setDateTime(QDateTime::currentDateTime());
	ui->StartDate->setDisplayFormat("dd.MM.yyyy hh:mm");
	ui->EndDate->setDisplayFormat("dd.MM.yyyy hh:mm");
	ui->EndDate_2->setDateTime(QDateTime::currentDateTime());
	ui->StartDate_2->setDisplayFormat("dd.MM.yyyy hh:mm");
	ui->EndDate_2->setDisplayFormat("dd.MM.yyyy hh:mm");
	initPayPlotI();
	initPayPlotO();

	QSqlQuery q;
	QString strQuery = "SELECT * FROM workertype;";
	q.exec(strQuery);
	while(q.next())
	{
		ui->filtergraphworktypeCBox->addItem(q.value("name").toString(),q.value("id").toString());
	}
	initworkerPlot();
	strQuery = "SELECT * FROM vehicletype;";
	q.exec(strQuery);
	while(q.next())
	{
		ui->filtergraphvehicletypeCBox_2->addItem(q.value("name").toString(),q.value("id").toString());
	}
	initvehiclePlot();
}
void OperatorBD::recieveCurrentIndex(const QModelIndex &index)
{
	ui->tw->setCurrentIndex(index);
	//ui->tw->setFocus();
}
void OperatorBD::onFindButtonClick()
{
	emit sendData(ui->tw->currentIndex());
}
OperatorBD::~OperatorBD()
{
	delete ui;
}

//запрет закрытия при нажатии ESC
void OperatorBD::keyPressEvent(QKeyEvent *e) {
	if(e->key() != Qt::Key_Escape)
		QDialog::keyPressEvent(e);
}

void OperatorBD::recieveUserID(int user)
{
	this->systemUserID = user;
}

void OperatorBD::recieveOperatorBDCurrentIndex(const QModelIndex &index)
{
	ui->tableView->setCurrentIndex(index);
	ui->tableView->setFocus();
}
void OperatorBD::onOperatorBDFindButtonClick()
{
	emit sendOperatorBDData(ui->tableView->currentIndex());

}

void OperatorBD::on_approveButton_clicked()
{
	QString str1 = "SELECT id FROM repair_state WHERE name = 'Ожидает'";
	QSqlQuery q;
	q.exec(str1);
	q.next();
	int standbystate= q.value("id").toInt();
	QSqlRecord rec = OperatorBDModel->record(ui->tableView->currentIndex().row());
	QString str = "UPDATE repair_tickets SET state = %1 WHERE id =%2";
	str = str.arg(QString::number(standbystate),rec.value("id").toString());
	q.exec(str);

	QModelIndex ind= ui->tableView->currentIndex();
	OperatorBDModel->select();
	ui->tableView->setCurrentIndex(ind);

}

void OperatorBD::on_addButton_clicked()
{
	ui->frame->show();
	ui->insertButton->setText("Добавить");
	ui->work_typeCBox->setCurrentIndex(-1);
	ui->addressCBox->setCurrentIndex(-1);
	ui->flatNumberCBox->clear();
}

void OperatorBD::on_editButton_clicked()
{
	ui->frame->show();
	ui->insertButton->setText("Изменить");

	QSqlRecord rec = OperatorBDModel->record(ui->tableView->currentIndex().row());
	ui->work_typeCBox->setCurrentIndex(ui->work_typeCBox->findText(\
		rec.value("works_type").toString()));
	ui->addressCBox->setCurrentIndex(ui->addressCBox->findText(\
		rec.value("address").toString()));
}

void OperatorBD::on_addressCBox_currentIndexChanged(int index)
{
	ui->flatNumberCBox->clear();
	QString str = "SELECT id,flat_number FROM flats WHERE address = '%1'";
	str = str.arg(ui->addressCBox->currentData().toString());
	QSqlQueryModel model;
	model.setQuery(str);
	if (model.lastError().isValid()) {
		qDebug() << model.lastError();
	}

	QSqlRecord addRec = model.record();
	for(int nRow =0; nRow<model.rowCount();nRow++)
	{
		addRec = model.record(nRow);
		ui->flatNumberCBox->addItem(addRec.value("flat_number").toString(),\
								addRec.value("id").toInt());
	}
	addRec = OperatorBDModel->record(ui->tableView->currentIndex().row());
	ui->flatNumberCBox->setCurrentIndex(ui->flatNumberCBox->findText(\
		addRec.value("flat_number").toString()));
}

void OperatorBD::on_cancel_insert_Button_clicked()
{
	ui->frame->hide();
	ui->work_typeCBox->setCurrentIndex(-1);
	ui->addressCBox->setCurrentIndex(-1);
	ui->flatNumberCBox->clear();
}

void OperatorBD::on_insertButton_clicked()
{
	QSqlRecord rec = OperatorBDModel->record(ui->tableView->currentIndex().row());

	QString str1 = "SELECT id FROM repair_state WHERE name = 'Не активно'";
	QSqlQuery q;
	q.exec(str1);
	q.next();
	QString standbystate= q.value("id").toString();
	if(ui->insertButton->text() == "Добавить")
	{
		//setDisplayFormat("dd.MM.yyyy hh:mm");
		str1.clear();
		str1 = "SELECT id FROM flats WHERE address = %1 AND flat_number = %2;";
		str1 = str1.arg(ui->addressCBox->currentData().toString(),\
			ui->flatNumberCBox->currentText());
		q.exec(str1);
		q.next();
		QString flatid= q.value("id").toString();
		//QString str = "INSERT INTO repair_tickets SET state = %1 WHERE id =%2";
		QString str =
			"INSERT INTO repair_tickets (date, flat, works_type, state) "
			"VALUES(STR_TO_DATE('%1', '%d.%m.%Y %H:%i'), %2, %3, %4);";
		str =	str.arg(\
			QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm"),\
			flatid,\
			ui->work_typeCBox->currentData().toString(),\
			standbystate);
		if (!q.exec(str)) {
			qDebug() << "Unable to make insert operation";
		}
		qDebug() << q.lastError();
	}
	else if(ui->insertButton->text() == "Изменить")
	{
		str1.clear();
		str1 = "SELECT id FROM flats WHERE address = %1 AND flat_number = %2;";
		str1 = str1.arg(ui->addressCBox->currentData().toString(),\
			ui->flatNumberCBox->currentText());
		q.exec(str1);
		q.next();
		QString flatid= q.value("id").toString();

		QString str = "UPDATE repair_tickets SET works_type = %1,"\
			"flat = %2 WHERE id =%3";
		str = str.arg(ui->work_typeCBox->currentData().toString(),\
			flatid,rec.value("id").toString());
		if (!q.exec(str)) {
			qDebug() << "Unable to make insert operation";
		}
		qDebug() << q.lastError();
	}	

	QModelIndex ind= ui->tableView->currentIndex();
	OperatorBDModel->select();
	ui->tableView->setCurrentIndex(ind);

	ui->frame->hide();
	ui->work_typeCBox->setCurrentIndex(-1);
	ui->addressCBox->setCurrentIndex(-1);
	ui->flatNumberCBox->clear();
}

void OperatorBD::on_deleteButton_clicked()
{
	QString str1 = "SELECT id FROM repair_state WHERE name = 'Отклонено'";
	QSqlQuery q;
	q.exec(str1);
	q.next();
	int standbystate= q.value("id").toInt();
	QSqlRecord rec = OperatorBDModel->record(ui->tableView->currentIndex().row());
	QString str = "UPDATE repair_tickets SET state = %1 WHERE id =%2";
	str = str.arg(QString::number(standbystate),rec.value("id").toString());
	q.exec(str);

	QModelIndex ind= ui->tableView->currentIndex();
	OperatorBDModel->select();
	ui->tableView->setCurrentIndex(ind);
}

void OperatorBD::on_add_button_clicked()
{
	ui->frame_2->show();
	ui->insert_button->setText("Добавить");
	ui->officeCBox->setCurrentIndex(-1);
	ui->worker_typeCBox->setCurrentIndex(-1);
	ui->fio->clear();
	ui->inn->clear();
	ui->reg->clear();
}

void OperatorBD::on_edit_button_clicked()
{
	ui->frame_2->show();
	ui->insert_button->setText("Изменить");

	QSqlRecord rec = workerModel->record(ui->tw->currentIndex().row());
	ui->officeCBox->setCurrentIndex(ui->officeCBox->findText(\
		rec.value("office").toString()));
	ui->worker_typeCBox->setCurrentIndex(ui->worker_typeCBox->findText(\
		rec.value("worker_type").toString()));
	ui->fio->setText(rec.value("fio").toString());
	ui->inn->setText(rec.value("inn").toString());
	ui->reg->setText(rec.value("registration").toString());
}


void OperatorBD::on_delete_button_clicked()
{
	QSqlRecord rec = workerModel->record(ui->tw->currentIndex().row());
	QSqlQuery q;
	QString str = "DELETE FROM worker WHERE id = %1;";
	str = str.arg(rec.value("id").toString());
	if (!q.exec(str)) {
		qDebug() << "Unable to make insert operation";
	}
	qDebug() << q.lastError();

	QModelIndex ind= ui->tw->currentIndex();
	workerModel->select();
	ui->tw->setCurrentIndex(ind);
}


void OperatorBD::on_insert_button_clicked()
{
	QSqlQuery q;
	if(ui->insert_button->text() == "Добавить")
	{
		QString str =
			"INSERT INTO worker (fio, inn, registration, office,worker_type) "
			"VALUES(%1, %2, %3, %4,%5);";
		str =	str.arg(\
			ui->fio->text(),\
			ui->inn->text(),\
			ui->reg->text(),\
			ui->officeCBox->currentData().toString(),\
			ui->worker_typeCBox->currentData().toString());
		if (!q.exec(str)) {
			qDebug() << "Unable to make insert operation";
		}
		qDebug() << q.lastError();
	}
	else if(ui->insert_button->text() == "Изменить")
	{
		QSqlRecord rec = workerModel->record(ui->tw->currentIndex().row());
		QString str = "UPDATE worker SET fio = %1,"\
			"inn = %2,registration=%3,office=%4,worker_type=%5 WHERE id =%6;";
		str = str.arg(\
		ui->fio->text(),\
		ui->inn->text(),\
		ui->reg->text(),\
		ui->officeCBox->currentData().toString(),\
		ui->worker_typeCBox->currentData().toString(),\
		rec.value("id").toString());
		if (!q.exec(str)) {
			qDebug() << "Unable to make insert operation";
		}
		qDebug() << q.lastError();
	}

	QModelIndex ind= ui->tw->currentIndex();
	workerModel->select();
	ui->tw->setCurrentIndex(ind);
	ui->frame_2->hide();
}

void OperatorBD::on_insert_ipuButton_clicked()
{
	ui->frame_3->show();
	ui->ok_counterButton->setText("Добавить ИПУ");
	ui->address_counterCBox->setCurrentIndex(-1);
	ui->flat_number_counterCbox->clear();
	ui->counter_typeCBox->setCurrentIndex(-1);
	ui->newvalueSBox->clear();

	ui->flat_number_counterCbox->setEnabled(true);
	ui->address_counterCBox->setEnabled(true);
	ui->counter_typeCBox->setEnabled(true);
}

void OperatorBD::on_insert_opuButton_clicked()
{
	ui->frame_3->show();
	ui->ok_counterButton->setText("Добавить ОПУ");
	ui->address_counterCBox->setCurrentIndex(-1);
	ui->flat_number_counterCbox->clear();
	ui->counter_typeCBox->setCurrentIndex(-1);
	ui->newvalueSBox->clear();

	ui->flat_number_counterCbox->setEnabled(false);
	ui->address_counterCBox->setEnabled(true);
	ui->counter_typeCBox->setEnabled(true);
}

void OperatorBD::on_update_opuButton_clicked()
{
	ui->frame_3->show();
	ui->ok_counterButton->setText("Изменить показания ИПУ");
	ui->newvalueSBox->clear();
	ui->flat_number_counterCbox->clear();

	QSqlRecord rec = house_counterModel->record(ui->house_counterView->currentIndex().row());
	ui->address_counterCBox->setCurrentIndex(ui->address_counterCBox->findText(\
		rec.value("address").toString()));
	ui->counter_typeCBox->setCurrentIndex(ui->counter_typeCBox->findText(\
		rec.value("counter_type").toString()));
	ui->flat_number_counterCbox->setEnabled(false);
	ui->address_counterCBox->setEnabled(false);
	ui->counter_typeCBox->setEnabled(false);
}

void OperatorBD::on_address_counterCBox_currentIndexChanged(int index)
{
	ui->flat_number_counterCbox->clear();
	QString str = "SELECT id,flat_number FROM flats WHERE address = '%1'";
	str = str.arg(ui->address_counterCBox->currentData().toString());
	QSqlQueryModel model;
	model.setQuery(str);
	if (model.lastError().isValid()) {
		qDebug() << model.lastError();
	}

	QSqlRecord addRec = model.record();
	for(int nRow =0; nRow<model.rowCount();nRow++)
	{
		addRec = model.record(nRow);
		ui->flat_number_counterCbox->addItem(addRec.value("flat_number").toString(),\
								addRec.value("id").toInt());
	}

	addRec = house_counterModel->record(ui->house_counterView->currentIndex().row());
	ui->flat_number_counterCbox->setCurrentIndex(ui->flat_number_counterCbox->findText(\
		addRec.value("flat_number").toString()));
}

void OperatorBD::on_cancel_counterButton_clicked()
{
	ui->frame_3->hide();
}

void OperatorBD::on_ok_counterButton_clicked()
{
	QSqlQuery q;
	if(ui->ok_counterButton->text() == "Добавить ИПУ")
	{
		QString str =
			"INSERT INTO flat_counter (adress, counter_type, date_readings,"
			"old_value,value) "
			"VALUES(%1, %2, STR_TO_DATE('%3', '%d.%m.%Y'), %4,%5);";
		str =	str.arg(\
			ui->flat_number_counterCbox->currentData().toString(),\
			ui->counter_typeCBox->currentData().toString(),\
			QDate::currentDate().toString("dd.MM.yyyy"),\
			QString::number(ui->newvalueSBox->value()),\
			QString::number(ui->newvalueSBox->value()));
		if (!q.exec(str)) {
			qDebug() << "Unable to make insert operation";
		}

		qDebug() << q.lastError();
		QModelIndex ind= ui->flat_counterView->currentIndex();
		flat_counterModel->select();
		ui->flat_counterView->setCurrentIndex(ind);
	}
	else if(ui->ok_counterButton->text() == "Добавить ОПУ")
	{
		QString str =
			"INSERT INTO house_counter (house_id, type_counter, date_readings,"
			"old_value,value) "
			"VALUES(%1, %2, STR_TO_DATE('%3', '%d.%m.%Y'), %4,%5);";
		str =	str.arg(\
			ui->address_counterCBox->currentData().toString(),\
			ui->counter_typeCBox->currentData().toString(),\
			QDate::currentDate().toString("dd.MM.yyyy"),\
			QString::number(ui->newvalueSBox->value()),\
			QString::number(ui->newvalueSBox->value()));
		if (!q.exec(str)) {
			qDebug() << "Unable to make insert operation";
		}

		qDebug() << q.lastError();
		QModelIndex ind= ui->house_counterView->currentIndex();
		house_counterModel->select();
		ui->house_counterView->setCurrentIndex(ind);
	}
	else if(ui->ok_counterButton->text() == "Изменить показания ИПУ")
	{
		QSqlRecord rec = house_counterModel->record(ui->house_counterView->currentIndex().row());
		QString str =
			"UPDATE house_counter SET value = %1 WHERE id = %2;";
		str =	str.arg(QString::number(ui->newvalueSBox->value()),\
			rec.value("id").toString());
		if (!q.exec(str)) {
			qDebug() << "Unable to make insert operation";
		}

		qDebug() << q.lastError();
		QModelIndex ind= ui->house_counterView->currentIndex();
		house_counterModel->select();
		ui->house_counterView->setCurrentIndex(ind);
	}

	ui->frame_3->hide();
}

void OperatorBD::on_fpushButton_clicked()
{
	flat_counterModel->setFilter("value "\
		+ui->fmlCBox->currentText()\
		+" '"\
		+ui->fvSBox->text()+"'");
	qDebug()<<flat_counterModel->select();
}

void OperatorBD::on_hpushButton_2_clicked(){}

void OperatorBD::on_hpushButto_clicked()
{
	house_counterModel->setFilter("value "\
		+ui->hmlCBox->currentText()\
		+" '"\
		+ui->hvSBox->text()+"'");
	qDebug()<<house_counterModel->select();
}

