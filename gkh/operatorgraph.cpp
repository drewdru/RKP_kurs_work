#include "OperatorBD.h"
#include "ui_OperatorBD.h"

void OperatorBD::yAxisRangeChangedI(const QCPRange &newRange)
{
	ui->payCPlot->yAxis->setRange(0, ui->payCPlot->yAxis->range().upper);// y軸の下限を0に固定
}
void OperatorBD::xAxisRangeChangedI(const QCPRange &newRange)
{
	ui->payCPlot->xAxis->setRange(-300, ui->payCPlot->xAxis->range().upper);// y軸の下限を0に固定
}

void OperatorBD::on_graphPayPButton_clicked()
{
	initPayPlotI();
}

void OperatorBD::on_graphClearFilterPButton_clicked()
{
	ui->StartDate->setDateTime(QDateTime::fromString("01.01.2000 00:00","dd.MM.yyyy hh:mm"));
	ui->EndDate->setDateTime(QDateTime::currentDateTime());
	initPayPlotI();
}

void OperatorBD::initPayPlotI()
{
	connect(ui->payCPlot->yAxis, SIGNAL(rangeChanged(const QCPRange &)), this, SLOT(yAxisRangeChangedI(const QCPRange &)));
	connect(ui->payCPlot->xAxis, SIGNAL(rangeChanged(const QCPRange &)), this, SLOT(xAxisRangeChangedI(const QCPRange &)));

	ui->payCPlot->addGraph();
	ui->payCPlot->setInteraction(QCP::iRangeDrag, true);
	ui->payCPlot->setInteraction(QCP::iSelectItems, true);
	ui->payCPlot->setInteraction(QCP::iRangeZoom, true);

	QString strQuery = "SELECT ABS(house_counter.value - house_counter.old_value) AS 'value', "
	"house_counter.date_readings,houses.address "
	"FROM house_counter "
	"INNER JOIN houses ON houses.id = house_counter.house_id "
	"WHERE date_readings BETWEEN STR_TO_DATE('%1','%d.%m.%Y %H:%i') "\
	"AND STR_TO_DATE('%2', '%d.%m.%Y %H:%i');";
	strQuery = strQuery.arg(ui->StartDate->text(),ui->EndDate->text());
	QSqlQuery q;
	q.exec(strQuery);

	QMap<QString,int> addressLst;
	while(q.next())
	{
			addressLst[q.value("address").toString().split(",")[0]] +=q.value("value").toDouble();
	}
	ui->payCPlot->clearPlottables();
	QCPBars *fossil = new QCPBars(ui->payCPlot->xAxis, ui->payCPlot->yAxis);
	ui->payCPlot->addPlottable(fossil);

	// set names and colors:
	QPen pen;
	pen.setWidthF(1.2);
	fossil->setName("Значение счётчика");
	pen.setColor(QColor(255, 131, 0));
	fossil->setPen(pen);
	fossil->setBrush(QColor(255, 131, 0, 50));
	fossil->setWidth(500);
	// prepare x axis with country labels:
	QVector<double> ticks;
	QVector<QString> labels;
	int j =0;
	for(;j<addressLst.count();j++)
	{
		ticks.append(j*500*4);
	}
	labels = addressLst.keys().toVector();

	ui->payCPlot->xAxis->setAutoTicks(false);
	ui->payCPlot->xAxis->setAutoTickLabels(false);
	ui->payCPlot->xAxis->setTickVector(ticks);
	ui->payCPlot->xAxis->setTickVectorLabels(labels);
	ui->payCPlot->xAxis->setTickLabelRotation(60);
	ui->payCPlot->xAxis->setSubTickCount(0);
	ui->payCPlot->xAxis->setTickLength(0, 4);
	ui->payCPlot->xAxis->grid()->setVisible(true);
	ui->payCPlot->xAxis->setRange(-300, j*500*4-1000);


	// prepare y axis:
	ui->payCPlot->yAxis->setRange(0, 57000);
	//ui->customPlot_2->yAxis->setRange(0, 0);
	ui->payCPlot->yAxis->setPadding(5); // a bit more space to the left border
	ui->payCPlot->yAxis->setLabel("Статистика потребления ИПУ");
	ui->payCPlot->yAxis->grid()->setSubGridVisible(true);
	QPen gridPen;
	gridPen.setStyle(Qt::SolidLine);
	gridPen.setColor(QColor(0, 0, 0, 25));
	ui->payCPlot->yAxis->grid()->setPen(gridPen);
	gridPen.setStyle(Qt::DotLine);
	ui->payCPlot->yAxis->grid()->setSubGridPen(gridPen);

	// Add data:
	QVector<double> fossilData;//, nuclearData, regenData;
	QMapIterator<QString, int> i(addressLst);
	while (i.hasNext()) {
		i.next();
		//qDebug() << i.key() << ": " << i.value() << endl;
		fossilData.append(i.value());
	}

	fossil->setData(ticks, fossilData);

	// setup legend:
	ui->payCPlot->legend->setVisible(true);
	ui->payCPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignHCenter);
	ui->payCPlot->legend->setBrush(QColor(255, 255, 255, 200));
	QPen legendPen;
	legendPen.setColor(QColor(130, 130, 130, 200));
	ui->payCPlot->legend->setBorderPen(legendPen);
	QFont legendFont = font();
	legendFont.setPointSize(10);
	ui->payCPlot->legend->setFont(legendFont);
	ui->payCPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
	ui->payCPlot->replot();
	ui->payCPlot->axisRect()->setupFullAxesBox();
}


void OperatorBD::yAxisRangeChangedO(const QCPRange &newRange)
{
	ui->payCPlot_2->yAxis->setRange(0, ui->payCPlot_2->yAxis->range().upper);// y軸の下限を0に固定
}
void OperatorBD::xAxisRangeChangedO(const QCPRange &newRange)
{
	ui->payCPlot_2->xAxis->setRange(-300, ui->payCPlot_2->xAxis->range().upper);// y軸の下限を0に固定
}

void OperatorBD::on_graphPayPButton_2_clicked()
{
	initPayPlotO();
}

void OperatorBD::on_graphClearFilterPButton_2_clicked()
{
	ui->StartDate_2->setDateTime(QDateTime::fromString("01.01.2000 00:00","dd.MM.yyyy hh:mm"));
	ui->EndDate_2->setDateTime(QDateTime::currentDateTime());
	initPayPlotO();
}

void OperatorBD::initPayPlotO()
{
	connect(ui->payCPlot_2->yAxis, SIGNAL(rangeChanged(const QCPRange &)), this, SLOT(yAxisRangeChangedO(const QCPRange &)));
	connect(ui->payCPlot_2->xAxis, SIGNAL(rangeChanged(const QCPRange &)), this, SLOT(xAxisRangeChangedO(const QCPRange &)));

	ui->payCPlot_2->addGraph();
	ui->payCPlot_2->setInteraction(QCP::iRangeDrag, true);
	ui->payCPlot_2->setInteraction(QCP::iSelectItems, true);
	ui->payCPlot_2->setInteraction(QCP::iRangeZoom, true);

	QString strQuery = "SELECT ABS(flat_counter.value - flat_counter.old_value) AS 'value', "
	"flat_counter.date_readings,houses.address "
	"FROM flat_counter "
	"INNER JOIN flats ON flats.id = flat_counter.adress "
	"INNER JOIN houses ON houses.id = flats.address "
	"WHERE date_readings BETWEEN STR_TO_DATE('%1','%d.%m.%Y %H:%i') "\
	"AND STR_TO_DATE('%2', '%d.%m.%Y %H:%i');";
	strQuery = strQuery.arg(ui->StartDate_2->text(),ui->EndDate_2->text());
	QSqlQuery q;
	q.exec(strQuery);

	QMap<QString,int> addressLst;
	while(q.next())
	{
			addressLst[q.value("address").toString().split(",")[0]] +=q.value("value").toDouble();
	}
	ui->payCPlot_2->clearPlottables();
	QCPBars *fossil = new QCPBars(ui->payCPlot_2->xAxis, ui->payCPlot_2->yAxis);
	ui->payCPlot_2->addPlottable(fossil);

	// set names and colors:
	QPen pen;
	pen.setWidthF(1.2);
	fossil->setName("Значение счётчика");
	pen.setColor(QColor(255, 131, 0));
	fossil->setPen(pen);
	fossil->setBrush(QColor(255, 131, 0, 50));
	fossil->setWidth(500);
	// prepare x axis with country labels:
	QVector<double> ticks;
	QVector<QString> labels;
	int j =0;
	for(;j<addressLst.count();j++)
	{
		ticks.append(j*500*4);
	}
	labels = addressLst.keys().toVector();

	ui->payCPlot_2->xAxis->setAutoTicks(false);
	ui->payCPlot_2->xAxis->setAutoTickLabels(false);
	ui->payCPlot_2->xAxis->setTickVector(ticks);
	ui->payCPlot_2->xAxis->setTickVectorLabels(labels);
	ui->payCPlot_2->xAxis->setTickLabelRotation(60);
	ui->payCPlot_2->xAxis->setSubTickCount(0);
	ui->payCPlot_2->xAxis->setTickLength(0, 4);
	ui->payCPlot_2->xAxis->grid()->setVisible(true);
	ui->payCPlot_2->xAxis->setRange(-300, j*500*4-1000);


	// prepare y axis:
	ui->payCPlot_2->yAxis->setRange(0, 100000);
	//ui->customPlot_2->yAxis->setRange(0, 0);
	ui->payCPlot_2->yAxis->setPadding(5); // a bit more space to the left border
	ui->payCPlot_2->yAxis->setLabel("Статистика потребления ОПУ");
	ui->payCPlot_2->yAxis->grid()->setSubGridVisible(true);
	QPen gridPen;
	gridPen.setStyle(Qt::SolidLine);
	gridPen.setColor(QColor(0, 0, 0, 25));
	ui->payCPlot_2->yAxis->grid()->setPen(gridPen);
	gridPen.setStyle(Qt::DotLine);
	ui->payCPlot_2->yAxis->grid()->setSubGridPen(gridPen);

	// Add data:
	QVector<double> fossilData;//, nuclearData, regenData;
	QMapIterator<QString, int> i(addressLst);
	while (i.hasNext()) {
		i.next();
		//qDebug() << i.key() << ": " << i.value() << endl;
		fossilData.append(i.value());
	}

	fossil->setData(ticks, fossilData);

	// setup legend:
	ui->payCPlot_2->legend->setVisible(true);
	ui->payCPlot_2->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignHCenter);
	ui->payCPlot_2->legend->setBrush(QColor(255, 255, 255, 200));
	QPen legendPen;
	legendPen.setColor(QColor(130, 130, 130, 200));
	ui->payCPlot_2->legend->setBorderPen(legendPen);
	QFont legendFont = font();
	legendFont.setPointSize(10);
	ui->payCPlot_2->legend->setFont(legendFont);
	ui->payCPlot_2->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
	ui->payCPlot_2->replot();
	ui->payCPlot_2->axisRect()->setupFullAxesBox();
}

void OperatorBD::xAxisRangeChangedW(const QCPRange &newRange)
{
	ui->workerCPlot_3->xAxis->setRange(-300, ui->workerCPlot_3->xAxis->range().upper);// y軸の下限を0に固定
}
void OperatorBD::yAxisRangeChangedW(const QCPRange &newRange)
{
	ui->workerCPlot_3->yAxis->setRange(0, ui->workerCPlot_3->yAxis->range().upper);// y軸の下限を0に固定
}
void OperatorBD::on_filtergraphworktypeCBox_currentIndexChanged(int index)
{
	if(index == -1 || index == 0)
	{
		ui->filtergraphworktypeCBox->blockSignals(true);
		ui->filtergraphworktypeCBox->setCurrentIndex(0);
		ui->filtergraphworktypeCBox->blockSignals(false);
		initworkerPlot();
	}
	else
		initworkerPlot("WHERE worker_type = "+ui->filtergraphworktypeCBox->currentData().toString());

}
void OperatorBD::initworkerPlot(QString str)
{
	connect(ui->workerCPlot_3->yAxis, SIGNAL(rangeChanged(const QCPRange &)), this, SLOT(yAxisRangeChangedW(const QCPRange &)));
	connect(ui->workerCPlot_3->xAxis, SIGNAL(rangeChanged(const QCPRange &)), this, SLOT(xAxisRangeChangedW(const QCPRange &)));

	ui->workerCPlot_3->addGraph();
	ui->workerCPlot_3->setInteraction(QCP::iRangeDrag, true);
	ui->workerCPlot_3->setInteraction(QCP::iSelectItems, true);
	ui->workerCPlot_3->setInteraction(QCP::iRangeZoom, true);

	QString strQuery = "SELECT work_space.address "
	"FROM worker "
	"INNER JOIN work_space ON work_space.id = worker.office ";
	strQuery.append(str+";");

	QSqlQuery q;
	q.exec(strQuery);

	QMap<QString,int> addressLst;
	while(q.next())
	{
			addressLst[q.value("address").toString().split(",")[0]] ++;
	}
	ui->workerCPlot_3->clearPlottables();
	QCPBars *fossil = new QCPBars(ui->workerCPlot_3->xAxis, ui->workerCPlot_3->yAxis);
	ui->workerCPlot_3->addPlottable(fossil);

	// set names and colors:
	QPen pen;
	pen.setWidthF(1.2);
	fossil->setName("Количество работников");
	pen.setColor(QColor(255, 131, 0));
	fossil->setPen(pen);
	fossil->setBrush(QColor(255, 131, 0, 50));
	fossil->setWidth(500);
	// prepare x axis with country labels:
	QVector<double> ticks;
	QVector<QString> labels;
	int j =0;
	for(;j<addressLst.count();j++)
	{
		ticks.append(j*500*4);
	}
	labels = addressLst.keys().toVector();

	ui->workerCPlot_3->xAxis->setAutoTicks(false);
	ui->workerCPlot_3->xAxis->setAutoTickLabels(false);
	ui->workerCPlot_3->xAxis->setTickVector(ticks);
	ui->workerCPlot_3->xAxis->setTickVectorLabels(labels);
	ui->workerCPlot_3->xAxis->setTickLabelRotation(60);
	ui->workerCPlot_3->xAxis->setSubTickCount(0);
	ui->workerCPlot_3->xAxis->setTickLength(0, 4);
	ui->workerCPlot_3->xAxis->grid()->setVisible(true);
	ui->workerCPlot_3->xAxis->setRange(-300, j*500*4-1000);


	// prepare y axis:
	ui->workerCPlot_3->yAxis->setRange(0, 8);
	//ui->customPlot_2->yAxis->setRange(0, 0);
	ui->workerCPlot_3->yAxis->setPadding(5); // a bit more space to the left border
	ui->workerCPlot_3->yAxis->setLabel("Количество работников");
	ui->workerCPlot_3->yAxis->grid()->setSubGridVisible(true);
	QPen gridPen;
	gridPen.setStyle(Qt::SolidLine);
	gridPen.setColor(QColor(0, 0, 0, 25));
	ui->workerCPlot_3->yAxis->grid()->setPen(gridPen);
	gridPen.setStyle(Qt::DotLine);
	ui->workerCPlot_3->yAxis->grid()->setSubGridPen(gridPen);

	// Add data:
	QVector<double> fossilData;//, nuclearData, regenData;
	QMapIterator<QString, int> i(addressLst);
	while (i.hasNext()) {
		i.next();
		//qDebug() << i.key() << ": " << i.value() << endl;
		fossilData.append(i.value());
	}

	fossil->setData(ticks, fossilData);

	// setup legend:
	ui->workerCPlot_3->legend->setVisible(true);
	ui->workerCPlot_3->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignHCenter);
	ui->workerCPlot_3->legend->setBrush(QColor(255, 255, 255, 200));
	QPen legendPen;
	legendPen.setColor(QColor(130, 130, 130, 200));
	ui->workerCPlot_3->legend->setBorderPen(legendPen);
	QFont legendFont = font();
	legendFont.setPointSize(10);
	ui->workerCPlot_3->legend->setFont(legendFont);
	ui->workerCPlot_3->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
	ui->workerCPlot_3->replot();
	ui->workerCPlot_3->axisRect()->setupFullAxesBox();
}


void OperatorBD::xAxisRangeChangedV(const QCPRange &newRange)
{
	ui->vehicleCPlot_4->xAxis->setRange(-300, ui->vehicleCPlot_4->xAxis->range().upper);// y軸の下限を0に固定
}
void OperatorBD::yAxisRangeChangedV(const QCPRange &newRange)
{
	ui->vehicleCPlot_4->yAxis->setRange(0, ui->vehicleCPlot_4->yAxis->range().upper);// y軸の下限を0に固定
}

void OperatorBD::on_filtergraphvehicletypeCBox_2_currentIndexChanged(int index)
{
	if(index == -1 || index == 0)
	{
		ui->filtergraphvehicletypeCBox_2->blockSignals(true);
		ui->filtergraphvehicletypeCBox_2->setCurrentIndex(0);
		ui->filtergraphvehicletypeCBox_2->blockSignals(false);
		initvehiclePlot();
	}
	else
		initvehiclePlot("WHERE vehicle_type = "+ui->filtergraphvehicletypeCBox_2->currentData().toString());
}

void OperatorBD::initvehiclePlot(QString str)
{
	connect(ui->vehicleCPlot_4->yAxis, SIGNAL(rangeChanged(const QCPRange &)), this, SLOT(yAxisRangeChangedV(const QCPRange &)));
	connect(ui->vehicleCPlot_4->xAxis, SIGNAL(rangeChanged(const QCPRange &)), this, SLOT(xAxisRangeChangedV(const QCPRange &)));

	ui->vehicleCPlot_4->addGraph();
	ui->vehicleCPlot_4->setInteraction(QCP::iRangeDrag, true);
	ui->vehicleCPlot_4->setInteraction(QCP::iSelectItems, true);
	ui->vehicleCPlot_4->setInteraction(QCP::iRangeZoom, true);

	QString strQuery = "SELECT work_space.address "
	"FROM vehicle "
	"INNER JOIN work_space ON work_space.id = vehicle.garage "
	"INNER JOIN vehicletype ON vehicletype.id = vehicle.vehicle_type ";
	strQuery.append(str+";");

	QSqlQuery q;
	q.exec(strQuery);

	QMap<QString,int> addressLst;
	while(q.next())
	{
		addressLst[q.value("address").toString().split(",")[0]] ++;
	}
	ui->vehicleCPlot_4->clearPlottables();
	QCPBars *fossil = new QCPBars(ui->vehicleCPlot_4->xAxis, ui->vehicleCPlot_4->yAxis);
	ui->vehicleCPlot_4->addPlottable(fossil);

	// set names and colors:
	QPen pen;
	pen.setWidthF(1.2);
	fossil->setName("Количество транспорта");
	pen.setColor(QColor(255, 131, 0));
	fossil->setPen(pen);
	fossil->setBrush(QColor(255, 131, 0, 50));
	fossil->setWidth(500);
	// prepare x axis with country labels:
	QVector<double> ticks;
	QVector<QString> labels;
	int j =0;
	for(;j<addressLst.count();j++)
	{
		ticks.append(j*500*4);
	}
	labels = addressLst.keys().toVector();

	ui->vehicleCPlot_4->xAxis->setAutoTicks(false);
	ui->vehicleCPlot_4->xAxis->setAutoTickLabels(false);
	ui->vehicleCPlot_4->xAxis->setTickVector(ticks);
	ui->vehicleCPlot_4->xAxis->setTickVectorLabels(labels);
	ui->vehicleCPlot_4->xAxis->setTickLabelRotation(60);
	ui->vehicleCPlot_4->xAxis->setSubTickCount(0);
	ui->vehicleCPlot_4->xAxis->setTickLength(0, 4);
	ui->vehicleCPlot_4->xAxis->grid()->setVisible(true);
	ui->vehicleCPlot_4->xAxis->setRange(-300, j*500*4-1000);


	// prepare y axis:
	ui->vehicleCPlot_4->yAxis->setRange(0, 20);
	//ui->customPlot_2->yAxis->setRange(0, 0);
	ui->vehicleCPlot_4->yAxis->setPadding(5); // a bit more space to the left border
	ui->vehicleCPlot_4->yAxis->setLabel("Количество транспорта в гараже");
	ui->vehicleCPlot_4->yAxis->grid()->setSubGridVisible(true);
	QPen gridPen;
	gridPen.setStyle(Qt::SolidLine);
	gridPen.setColor(QColor(0, 0, 0, 25));
	ui->vehicleCPlot_4->yAxis->grid()->setPen(gridPen);
	gridPen.setStyle(Qt::DotLine);
	ui->vehicleCPlot_4->yAxis->grid()->setSubGridPen(gridPen);

	// Add data:
	QVector<double> fossilData;//, nuclearData, regenData;
	QMapIterator<QString, int> i(addressLst);
	while (i.hasNext()) {
		i.next();
		//qDebug() << i.key() << ": " << i.value() << endl;
		fossilData.append(i.value());
	}

	fossil->setData(ticks, fossilData);

	// setup legend:
	ui->vehicleCPlot_4->legend->setVisible(true);
	ui->vehicleCPlot_4->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignHCenter);
	ui->vehicleCPlot_4->legend->setBrush(QColor(255, 255, 255, 200));
	QPen legendPen;
	legendPen.setColor(QColor(130, 130, 130, 200));
	ui->vehicleCPlot_4->legend->setBorderPen(legendPen);
	QFont legendFont = font();
	legendFont.setPointSize(10);
	ui->vehicleCPlot_4->legend->setFont(legendFont);
	ui->vehicleCPlot_4->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
	ui->vehicleCPlot_4->replot();
	ui->vehicleCPlot_4->axisRect()->setupFullAxesBox();
}



