#include "Accountant.h"
#include "ui_Accountant.h"
#include <QDebug>
#include <QPrinter>
#include <QPrintDialog>

double FindFirstTarif(QSqlRelationalTableModel* model,QString servt);
double FindFirstNorm(QSqlRelationalTableModel* model,QString servt);
Accountant::Accountant(QWidget *parent) :
QDialog(parent),
ui(new Ui::Accountant)
{
	ui->setupUi(this);
	this->setWindowFlags(Qt::Window);
	this->setWindowTitle("Бухгалтер");
	/*accountModel = new QSqlRelationalTableModel(this,QSqlDatabase::database());
	accountModel->setTable("");
	ui->AccountTView*/
	tariffModel = new QSqlRelationalTableModel(this,QSqlDatabase::database());
	tariffModel->setTable("tariffView");
	tariffModel->select();
	ui->tariffView->setModel(tariffModel);
	ui->tariffView->resizeColumnsToContents();
	ui->tariffView->setCurrentIndex(tariffModel->index(0,0));

	///
	normModel = new QSqlRelationalTableModel(this,QSqlDatabase::database());
	normModel->setTable("normView");
	normModel->select();
	ui->normView->setModel(normModel);
	ui->normView->resizeColumnsToContents();
	ui->normView->setCurrentIndex(normModel->index(0,0));

	cashier_log = new QSqlRelationalTableModel(this,QSqlDatabase::database());
	cashier_log->setTable("cashier_log");
	cashier_log->select();
	QSqlRecord rec = cashier_log->record();
	ui->cashier_logView->setModel(cashier_log);
	ui->cashier_logView->resizeColumnsToContents();
	ui->cashier_logView->setCurrentIndex(cashier_log->index(0,0));
	ui->cashier_logView->setColumnHidden(rec.indexOf("user"),true);
	ui->cashier_logView->setColumnHidden(rec.indexOf("cashier"),true);

	accountModel = new QSqlRelationalTableModel(this,QSqlDatabase::database());
	accountModel->setTable("accounts");
	accountModel->select();
	ui->accountView->setModel(accountModel);
	ui->accountView->resizeColumnsToContents();
	ui->accountView->setCurrentIndex(accountModel->index(0,0));
	rec = accountModel->record();
	ui->accountView->setColumnHidden(rec.indexOf("pass"),true);
	ui->accountView->setColumnHidden(rec.indexOf("login"),true);
	ui->accountView->setColumnHidden(rec.indexOf("flat"),true);
	ui->accountView->setColumnHidden(rec.indexOf("last_cashier"),true);
	ui->accountView->horizontalHeader()->moveSection(rec.indexOf("fio"),\
											rec.indexOf("isrelief"));

	repairModel = new QSqlRelationalTableModel(this,QSqlDatabase::database());
	repairModel->setTable("repairview");
	repairModel->select();
	rec = repairModel->record();
	ui->repairView->setModel(repairModel);
	ui->repairView->resizeColumnsToContents();
	ui->repairView->horizontalHeader()->moveSection(rec.indexOf("address"),\
											rec.indexOf("works_type"));
	rec = repairModel->record();
	ui->group_repairCBox->addItem("состояние","repair_state");
	ui->group_repairCBox->addItem("тип работ","works_type");
	ui->group_repairCBox->addItem("дата","date_date");
	ui->group_repairCBox->addItem("адрес","address");
	ui->group_repairCBox_2->addItem("состояние","repair_state");
	ui->group_repairCBox_2->addItem("тип работ","works_type");
	ui->group_repairCBox_2->addItem("дата","date_date");
	ui->group_repairCBox_2->addItem("адрес","address");

	vehicleModel = new QSqlRelationalTableModel(this,QSqlDatabase::database());
	vehicleModel->setTable("vehicleview");
	vehicleModel->select();
	ui->vehicleView->setModel(vehicleModel);
	ui->vehicleView->setCurrentIndex(vehicleModel->index(0,0));
	rec = vehicleModel->record();
	ui->vehicleView->setColumnHidden(rec.indexOf("consumption"),true);
	emit ui->vehicleView->clicked(vehicleModel->index(0,0));

	ui->lastdateEdit_2->setDateTime(QDateTime::currentDateTime());
	ui->lastdateEdit_2->setDisplayFormat("dd.MM.yyyy");
	ui->firstdateEdit->setDisplayFormat("dd.MM.yyyy");

	ui->EndDate->setDateTime(QDateTime::currentDateTime());
	ui->StartDate->setDisplayFormat("dd.MM.yyyy hh:mm");
	ui->EndDate->setDisplayFormat("dd.MM.yyyy hh:mm");
	initPayPlot();

	/*rec = repairModel->record();
	repairModel->setHeaderData( rec.indexOf("id"), Qt::Horizontal, QObject::tr("№") );
	repairModel->setHeaderData( rec.indexOf("license_number"), Qt::Horizontal, QObject::tr("Номер транспорта") );
	repairModel->setHeaderData( rec.indexOf("vehicletype"), Qt::Horizontal, QObject::tr("Тип транспорта") );
	repairModel->setHeaderData( rec.indexOf("garage"), Qt::Horizontal, QObject::tr("Адрес гаража") );
	ui->repairView->resizeColumnsToContents();*/
}
//запрет закрытия при нажатии ESC
void Accountant::keyPressEvent(QKeyEvent *e) {
	if(e->key() != Qt::Key_Escape)
		QDialog::keyPressEvent(e);
}
void Accountant::recieveUserID(int user)
{
	this->systemUserID = user;
}
Accountant::~Accountant()
{
	delete ui;
}


void Accountant::on_tariff_updateButton_clicked()
{
	QSqlRecord rec = tariffModel->record(ui->tariffView->currentIndex().row());

	QString str = "UPDATE tariff SET cost = %1 WHERE id =%2;";
	str = str.arg(QString::number(ui->tariffSBox->value()),\
		rec.value("id").toString());
	QSqlQuery q;
	q.exec(str);

	QModelIndex index= ui->tariffView->currentIndex();
	tariffModel->select();
	ui->tariffView->setCurrentIndex(index);
}

void Accountant::on_norm_updateButton_clicked()
{
	QSqlRecord rec = normModel->record(ui->normView->currentIndex().row());

	QString str = "UPDATE norm SET norm_value = %1 WHERE id =%2;";
	str = str.arg(QString::number(ui->normSBox->value()),\
		rec.value("id").toString());
	QSqlQuery q;
	q.exec(str);

	QModelIndex index= ui->normView->currentIndex();
	normModel->select();
	ui->normView->setCurrentIndex(index);
}

void Accountant::on_pushButton_clicked()
{
	QSqlRecord rec;
	for(int nRow =0; nRow<accountModel->rowCount();nRow++)
	{
		QString str;
		rec = accountModel->record(nRow);


		str = "SELECT flats.id,flats.rooms_number,flats.level,houses.address,flats.flat_number, "\
			"flats.area,flats.total_areal,houses.elevator,houses.levels,"\
			"houses.doorways,houses.space,house_type.name AS 'house_type',"\
			"material_of_house.name AS 'material_of_house', "\
			"(SELECT count(*) from residents where account = "\
			+ rec.value("id").toString() +") AS 'counter' "\
			"FROM flats "\
			"INNER JOIN houses ON houses.id = flats.address "\
			"INNER JOIN house_type ON house_type.id = houses.type "\
			"INNER JOIN material_of_house ON material_of_house.id = houses.material "\
			"WHERE flats.id = '"+rec.value("flat").toString()+"';";
		QSqlQueryModel qModel;
		qModel.setQuery(str);
		if (qModel.lastError().isValid()) {
			qDebug() << qModel.lastError();
		}
		QSqlRecord addRec = qModel.record(0);
		double a = addRec.value("total_areal").toDouble();
		double b = addRec.value("area").toDouble();
		double c = addRec.value("counter").toInt()+1;
		double d = addRec.value("rooms_number").toInt();
		double koef = a /((b*c)	/d);
		double sumvalue = 0;

		str = "SELECT service_type.id,service_type.name,measures.measure FROM service_type INNER JOIN measures ON measures.id = service_type.measure;";
		QSqlQueryModel qModel2;
		qModel2.setQuery(str);
		if (qModel2.lastError().isValid()) {
			qDebug() << qModel2.lastError();
		}
		//tariffModel->setFilter(addRec.value("material_of_house").toString());
		tariffModel->setFilter("material_of_house LIKE '%"\
			+addRec.value("material_of_house").toString()+"%'");
		tariffModel->select();
		normModel->setFilter("isrelief ="+rec.value("isrelief").toString());
		normModel->select();

QString strF = "<H1>УК \"Солнышко\"<br>ПЛАТЁЖНЫЙ ДОКУМЕНТ</H1>"
"<H4>для внесение платы за содержание и ремонт жилого помещения и представление коммунальных услуг</H4>"
"<div><table border=\"0\" cellpadding=\"0\" cellspacing=\"2\""
"<tr><td><table cellspacing=\"2\" border=\"1\" cellpadding=\"5\">"
	"<caption>Раздел 1. Сведения о плательщике</caption>"
	"<tr><td>Расчётный период: %1<br>"
		"ФИО собственника/нанимателя: %2<br>"
		"Адрес помещения: %3, кв.%4<br>"
		"Площадь: %5 м2,количество зарегистрированных: %6</td></tr>"
	"</table></td>"
	"<td><table cellspacing=\"2\" border=\"1\" cellpadding=\"5\">"
	"<caption>Раздел 2. Сведения об исполнителе услуг</caption>"
	"<tr><td>Исполнитель услуг: УК \"Солнышко\"<br>"
		"Адрес: %7, (пн-пт с 8:00 до 17:00)<br>"
		"Контактные данные: %8<br>"
		"Бухгалтер: %9"
	"</td></tr>"
	"</table></td></tr>"
"</table></div>"
"<div><table cellspacing=\"2\" border=\"1\" cellpadding=\"5\">"
"<caption>Раздел 3. Расчёт размера платы за содержание и ремонт жилого помещения и коммунальные услуги</caption>"
	"<tr><th>Вид услуг</th><th>Ед. изм.</th><th>Объём услуг</th><th>Тариф р. за ед.изм.</th><th>Начисление</th></tr>";

		for(int nRow =0; nRow<qModel2.rowCount();nRow++)
		{
			QSqlRecord s_tRec = qModel2.record(nRow);
			double tar = FindFirstTarif(tariffModel,s_tRec.value("name").toString());
			double nor = FindFirstNorm(normModel,s_tRec.value("name").toString());
			double valuest = tar*nor*1.12*koef*rec.value("part").toDouble();
			sumvalue += valuest;
	strF.append("<tr><td>"+s_tRec.value("name").toString()+"</td><td>"+s_tRec.value("measure").toString()+"<td>"+QString::number(nor)+"</td><td>"+QString::number(tar)+"</td><td>"+QString::number(valuest)+"</td></tr>");
		}
		QSqlQuery q;
		str =	"SELECT user.fio,work_space.address,work_space.phones "
				"FROM user "
				"INNER JOIN work_space ON work_space.id = user.office "
				"WHERE user.id = '"+QString::number(systemUserID)+"';";
		q.exec(str);
		q.next();
strF.append("</table><H3>ИТОГО К ОПЛАТЕ: "+QString::number(sumvalue)+" р.</H3></div>");
		strF = strF.arg(QDate::currentDate().toString("MM.yyyy"),\
			rec.value("fio").toString(),\
			addRec.value("address").toString(),\
			addRec.value("flat_number").toString(),\
			addRec.value("total_areal").toString(),\
			addRec.value("counter").toString(),\
			q.value("fio").toString(),\
			q.value("phones").toString(),\
			q.value("address").toString());

		this->print(strF);

		str = "UPDATE accounts SET money = money + %1 WHERE id = %2;";
		str = str.arg(QString::number(sumvalue),rec.value("id").toString());

		q.exec(str);
	}
	tariffModel->setFilter("");
	tariffModel->select();
	normModel->setFilter("");
	normModel->select();

	QModelIndex index= ui->accountView->currentIndex();
	accountModel->select();
	ui->accountView->setCurrentIndex(index);
}

double FindFirstTarif(QSqlRelationalTableModel* model,QString servt)
{
	QSqlRecord rec;
	for(int nRow =0; nRow<model->rowCount();nRow++)
	{
		rec = model->record(nRow);
		if(rec.value("service_type").toString() == servt)
		{
			return rec.value("cost").toDouble();
		}
	}
	qDebug()<<"FAIL";
	return 10;
}
double FindFirstNorm(QSqlRelationalTableModel* model,QString servt)
{
	QSqlRecord rec;
	for(int nRow =0; nRow<model->rowCount();nRow++)
	{
		rec = model->record(nRow);
		if(rec.value("name").toString() == servt)
		{
			return rec.value("norm_value").toDouble();
		}
	}
	qDebug()<<"FAIL";
	return 5;
}

void Accountant::print(QString pstr)
{
	QTextDocument check;
	check.setHtml(pstr);
	QPrinter printer(QPrinter::HighResolution);
	QPrintDialog dialog(&printer, this);
	dialog.setWindowTitle(tr("Print"));
	if (dialog.exec() == QDialog::Accepted)
	{
		check.print(&printer);
	}
}

void Accountant::on_vehicleView_clicked(const QModelIndex &index)
{
	QString vehicleID = vehicleModel->record(index.row()).value("id").toString();

	QString str = "SELECT id, distance,date "
		"FROM movements_log WHERE vehicle = "+vehicleID+";";
	movements.setQuery(str);
	if (movements.lastError().isValid()) {
		qDebug() << movements.lastError();
	}
	ui->movementsView->setModel(&movements);
	ui->movementsView->resizeColumnsToContents();

	str = "SELECT vehicleservice.id, vehicleservice.money,vehicleservice.date, "
		"vehicleservicetype.name AS 'service_type' "
		"FROM vehicleservice INNER JOIN vehicleservicetype ON vehicleservicetype.id = vehicleservice.service_type "
		"WHERE vehicle = "+vehicleID+";";
	service.setQuery(str);
	if (service.lastError().isValid()) {
		qDebug() << service.lastError();
	}
	ui->serviceView->setModel(&service);
	ui->serviceView->resizeColumnsToContents();
}

void Accountant::on_pushButton_2_clicked()
{
}

void Accountant::on_tpushButton_clicked()
{
	tariffModel->setFilter("cost "\
	+ui->tmlCBox->currentText()\
	+" '"\
	+ui->tvSBox->text()+"'");
qDebug()<<tariffModel->select();
}

void Accountant::on_npushButton_clicked()
{
	normModel->setFilter("norm_value "\
	+ui->nmlCBox->currentText()\
	+" '"\
	+ui->nvSBox->text()+"'");
qDebug()<<normModel->select();
}

void Accountant::on_apushButton_clicked()
{
	accountModel->setFilter("money "\
	+ui->amlCBox_2->currentText()\
	+" '"\
	+ui->avSBox_2->text()+"'");
qDebug()<<normModel->select();
}

void Accountant::on_aspushButton_clicked()
{
	repairModel->setFilter("sum "\
	+ui->asmlCBox->currentText()\
	+" '"\
	+ui->asvSBox->text()+"'");
qDebug()<<repairModel->select();
}

void Accountant::on_aspushButton_2_clicked()
{
	cashier_log->setFilter("ABS(new_money - old_money) "\
			+ui->asmlCBox_2->currentText()\
			+" '"\
			+ui->asvSBox_2->text()+"'");
		qDebug()<<cashier_log->select();
}

void Accountant::on_pushButton_3_clicked()
{
	QString str = "SELECT CONCAT(cashier_log.account,':',accounts.fio) AS 'acc',"
	"SUM(ABS(cashier_log.new_money-cashier_log.old_money)) AS 'money' "
	"FROM cashier_log "
	"INNER JOIN accounts on accounts.id = cashier_log.account "
	"WHERE TO_DAYS(NOW()) - TO_DAYS(date) <= 366 "
	"GROUP BY account;";
	QSqlQuery q;
	q.exec(str);

	QString strF ="<table cellspacing=\"0\" border=\"1\" cellpadding=\"5\">"
	"<caption>Годовой отчёт</caption>"
	"<tr><th>Собственник</th><th>Сумма за год</th></tr>";
	while(q.next())
	{
		strF.append("<tr><td>"+q.value("acc").toString()+"</td><td>"\
			+q.value("money").toString()+" р.</td></tr>");
	}
	"</table>";
	this->print(strF);
}

void Accountant::on_tariff_reportButton_clicked()
{
	QString str = "SELECT DISTINCT "
	"service_type.name, "
	"AVG(tariff.cost) AS 'tariff', "
	"measures.measure "
	"FROM tariff "
	"INNER JOIN service_type on service_type.id = tariff.service_type "
	"INNER JOIN measures on measures.id = service_type.measure "
	"GROUP BY service_type;";
	QSqlQuery q;
	q.exec(str);

	QString strF ="<table cellspacing=\"0\" border=\"1\" cellpadding=\"5\">"
	"<caption>Среднее значение тарифа</caption>"
	"<tr><th>Вид услуг</th><th>Среднее значение</th><th>Ед. изм.</th></tr>";
	while(q.next())
	{
		strF.append("<tr><td>"+q.value("name").toString()+"</td><td>"\
			+q.value("tariff").toString()+" р.</td><td>"\
			+q.value("measure").toString()+"</td></tr>");
	}
	"</table>";
	this->print(strF);
}

void Accountant::on_norm_reportButton_clicked()
{
	QString str = "SELECT DISTINCT "
	"service_type.name, "
	"AVG(norm.norm_value) AS 'norm', "
	"measures.measure "
	"FROM norm "
	"INNER JOIN service_type on service_type.id = norm.service_type "
	"INNER JOIN measures on measures.id = service_type.measure "
	"GROUP BY service_type;";
	QSqlQuery q;
	q.exec(str);

	QString strF ="<table cellspacing=\"0\" border=\"1\" cellpadding=\"5\">"
	"<caption>Среднее значение нормы</caption>"
	"<tr><th>Вид услуг</th><th>Среднее значение</th><th>Ед. изм.</th></tr>";
	while(q.next())
	{
		strF.append("<tr><td>"+q.value("name").toString()+"</td><td>"\
			+q.value("norm").toString()+" р.</td><td>"\
			+q.value("measure").toString()+"</td></tr>");
	}
	"</table>";
	this->print(strF);
}

void Accountant::on_repair_report_PButton_clicked()
{
	QString str = "SELECT DISTINCT "
		"repair_state.name AS 'repair_state', "
		"DATE_FORMAT(repair_tickets.date,'%Y-%m-%d') AS 'date_date', "
		"repair_tickets.sum, "
		"works_type.name AS 'works_type', "
		"houses.address, "
		"COUNT(*) "
	"FROM repair_tickets "
	"INNER JOIN flats ON flats.id = repair_tickets.flat "
	"INNER JOIN houses ON houses.id = flats.address "
	"INNER JOIN repair_state ON repair_state.id = repair_tickets.state "
	"INNER JOIN works_type ON works_type.id = repair_tickets.works_type "
	"GROUP BY "+ui->group_repairCBox->currentData().toString()+";";
	QSqlQuery q;
	q.exec(str);

	QString strF ="<table cellspacing=\"0\" border=\"1\" cellpadding=\"5\">"
	"<caption>Количество заявок. Столбец группировки: "+ui->group_repairCBox->currentText()+"</caption>"
	"<tr><th>Количество заявок</th><th>Состояние</th><th>Дата</th><th>Тип работ</th><th>Адрес</th></tr>";
	while(q.next())
	{
		strF.append("<tr><td>"+q.value("COUNT(*)").toString()+"</td><td>"\
			+q.value("repair_state").toString()+"</td><td>"\
			+q.value("date_date").toString()+"</td><td>"\
			+q.value("works_type").toString()+"</td><td>"\
			+q.value("address").toString()+"</td></tr>");
	}
	"</table>";
	this->print(strF);
}

void Accountant::on_repair_report_PButton_2_clicked()
{
	QString str = "SELECT DISTINCT "
	"repair_state.name AS 'repair_state',  "
	"DATE_FORMAT(repair_tickets.date,'%Y-%m-%d') AS 'date_date', "
	"SUM(repair_tickets.sum) as 'all_sum', "
	"works_type.name AS 'works_type', "
	"houses.address,"
	"GROUP_CONCAT(repair_tickets.id) AS 'repairticketsid' "
	"FROM repair_tickets "
	"INNER JOIN flats ON flats.id = repair_tickets.flat "
	"INNER JOIN houses ON houses.id = flats.address "
	"INNER JOIN repair_state ON repair_state.id = repair_tickets.state "
	"INNER JOIN works_type ON works_type.id = repair_tickets.works_type "
	"GROUP BY "+ui->group_repairCBox_2->currentData().toString()+";";
	QSqlQuery q;
	q.exec(str);

	QString strF ="<table cellspacing=\"0\" border=\"1\" cellpadding=\"5\">"
	"<caption>Затраты на заявку (с учётом затрат на транспорт). Столбец группировки: "+ui->group_repairCBox->currentText()+"</caption>"
	"<tr><th>Состояние</th><th>Дата</th><th>Тип работ</th><th>Адрес</th><th>Стоимость</th></tr>";
	while(q.next())
	{
		strF.append("<tr><td>"+q.value("repair_state").toString()+"</td><td>"\
			+q.value("date_date").toString()+"</td><td>"\
			+q.value("works_type").toString()+"</td><td>"\
			+q.value("address").toString()+"</td><td>");

		double value_sum = 0;
		value_sum += q.value("all_sum").toDouble();
		QStringList reptickid = q.value("repairticketsid").toString().split(",");
		foreach (QString rtid, reptickid) {
			QString str2 = "SELECT "
			"movings_list.mov, "
			"(SELECT distancetomoney(vehicle.consumption,movements_log.distance)) AS 'summ' "
			"FROM movings_list "
			"INNER JOIN movements_log ON movements_log.id = movings_list.mov "
			"INNER JOIN vehicle ON vehicle.id = movements_log.vehicle "
			"WHERE repair_tickets_id = "+rtid+";";
			QSqlQuery sub_q;
			sub_q.exec(str2);
			while(sub_q.next())
			{
				value_sum += sub_q.value("summ").toDouble();
			}
		}
		strF.append(QString::number(value_sum)+"</td></tr>");
	}
	"</table>";
	this->print(strF);
}

void Accountant::on_vehicle_repairPButton_clicked()
{
	QString str = "SELECT DISTINCT "
	"movements_log.vehicle AS 'vehicle_id', "
	"vehicle.license_number, "
	"movements_log.distance, "
	"SUM((SELECT distancetomoney(vehicle.consumption,movements_log.distance))) AS 'travel_costs' "
	"FROM movements_log "
	"INNER JOIN vehicle ON vehicle.id = movements_log.vehicle "
	"WHERE movements_log.date between STR_TO_DATE('%1', '%d.%m.%Y') and STR_TO_DATE('%2', '%d.%m.%Y') "
	"GROUP BY vehicle_id;";
	str = str.arg(ui->firstdateEdit->text(),ui->lastdateEdit_2->text());
	QSqlQuery q;
	q.exec(str);

	QString strF ="<table cellspacing=\"0\" border=\"1\" cellpadding=\"5\">"
	"<caption>Затраты на проезд. С "+ui->firstdateEdit->text()+" по "+ui->lastdateEdit_2->text()+"</caption>"
	"<tr><th>ИД транспорта</th><th>Номер</th><th>Дистанция</th><th>Сумма</th></tr>";
	while(q.next())
	{
		strF.append("<tr><td>"+q.value("vehicle_id").toString()+"</td><td>"\
			+q.value("license_number").toString()+"</td><td>"\
			+q.value("distance").toString()+"</td><td>"\
			+q.value("travel_costs").toString()+"р.</td></tr>");
	}
	"</table>";
	this->print(strF);
}

void Accountant::on_vehicle_repairPButton_2_clicked()
{
	QString str = "SELECT DISTINCT "
		"vehicleservice.vehicle AS 'vehicle_id',  "
		"vehicle.license_number, "
		"SUM(vehicleservice.money) AS 'sum_money', "
		"vehicleservicetype.name AS 'servicetype' "
	"FROM vehicleservice "
	"INNER JOIN vehicle ON vehicle.id = vehicleservice.vehicle "
	"INNER JOIN vehicleservicetype ON vehicleservicetype.id = vehicleservice.service_type "
	"WHERE vehicleservice.date between STR_TO_DATE('%1', '%d.%m.%Y') and STR_TO_DATE('%2', '%d.%m.%Y') "
	"GROUP BY servicetype;";
	str = str.arg(ui->firstdateEdit->text(),ui->lastdateEdit_2->text());
	QSqlQuery q;
	q.exec(str);

	QString strF ="<table cellspacing=\"0\" border=\"1\" cellpadding=\"5\">"
	"<caption>Затраты на обслуживание. С "+ui->firstdateEdit->text()+" по "+ui->lastdateEdit_2->text()+"</caption>"
	"<tr><th>Сумма</th><th>Тип обслуживания</th></tr>";
	while(q.next())
	{
		strF.append("<tr><td>"+q.value("sum_money").toString()+"р.</td><td>"\
			+q.value("servicetype").toString()+"</td></tr>");
	}
	"</table>";
	this->print(strF);
}




