#include "EmergencyService.h"
#include "ui_EmergencyService.h"

void EmergencyService::yAxisRangeChangedV(const QCPRange &newRange)
{
	ui->VcustomPlot->yAxis->setRange(0, ui->VcustomPlot->yAxis->range().upper);// y軸の下限を0に固定
}
void EmergencyService::xAxisRangeChangedV(const QCPRange &newRange)
{
	ui->VcustomPlot->xAxis->setRange(-300, ui->VcustomPlot->xAxis->range().upper);// y軸の下限を0に固定
}
void EmergencyService::on_graphFilterCBox_currentIndexChanged(int index)
{
	if(index == -1 || index == 0)
	{
		ui->graphFilterCBox->blockSignals(true);
		ui->graphFilterCBox->setCurrentIndex(0);
		ui->graphFilterCBox->blockSignals(false);
		initPayPlotV();
	}
	else
		initPayPlotV("WHERE vehicle_type = "+ui->graphFilterCBox->currentData().toString());

}

void EmergencyService::initPayPlotV(QString str)
{
	connect(ui->VcustomPlot->yAxis, SIGNAL(rangeChanged(const QCPRange &)), this, SLOT(yAxisRangeChangedV(const QCPRange &)));
	connect(ui->VcustomPlot->xAxis, SIGNAL(rangeChanged(const QCPRange &)), this, SLOT(xAxisRangeChangedV(const QCPRange &)));

	ui->VcustomPlot->addGraph();
	ui->VcustomPlot->setInteraction(QCP::iRangeDrag, true);
	ui->VcustomPlot->setInteraction(QCP::iSelectItems, true);
	ui->VcustomPlot->setInteraction(QCP::iRangeZoom, true);

	QString strQuery = "SELECT SUM((SELECT distancetomoney(vehicle.consumption,movements_log.distance))) AS 'moneytogasolin', "
	"houses.address "
	"FROM movings_list "
	"INNER JOIN movements_log ON movements_log.id = movings_list.mov "
	"INNER JOIN vehicle ON vehicle.id = movements_log.vehicle "
	"INNER JOIN repair_tickets ON repair_tickets.id = movings_list.repair_tickets_id "
	"INNER JOIN flats ON flats.id = repair_tickets.flat "
	"INNER JOIN houses ON houses.id = flats.address ";
	strQuery.append(str+" ");
	strQuery.append("GROUP BY repair_tickets_id;");
	QSqlQuery q;
	q.exec(strQuery);

	QMap<QString,int> addressLst;
	while(q.next())
	{
			addressLst[q.value("address").toString().split(",")[0]] +=q.value("moneytogasolin").toDouble();
	}
	ui->VcustomPlot->clearPlottables();
	QCPBars *fossil = new QCPBars(ui->VcustomPlot->xAxis, ui->VcustomPlot->yAxis);
	ui->VcustomPlot->addPlottable(fossil);

	// set names and colors:
	QPen pen;
	pen.setWidthF(1.2);
	fossil->setName("Количество денег (р.)");
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

	ui->VcustomPlot->xAxis->setAutoTicks(false);
	ui->VcustomPlot->xAxis->setAutoTickLabels(false);
	ui->VcustomPlot->xAxis->setTickVector(ticks);
	ui->VcustomPlot->xAxis->setTickVectorLabels(labels);
	ui->VcustomPlot->xAxis->setTickLabelRotation(60);
	ui->VcustomPlot->xAxis->setSubTickCount(0);
	ui->VcustomPlot->xAxis->setTickLength(0, 4);
	ui->VcustomPlot->xAxis->grid()->setVisible(true);
	ui->VcustomPlot->xAxis->setRange(-300, j*500*4-1000);


	// prepare y axis:
	ui->VcustomPlot->yAxis->setRange(0, 1200);
	//ui->customPlot_2->yAxis->setRange(0, 0);
	ui->VcustomPlot->yAxis->setPadding(5); // a bit more space to the left border
	ui->VcustomPlot->yAxis->setLabel("Расходы на бензин за 2015 год");
	ui->VcustomPlot->yAxis->grid()->setSubGridVisible(true);
	QPen gridPen;
	gridPen.setStyle(Qt::SolidLine);
	gridPen.setColor(QColor(0, 0, 0, 25));
	ui->VcustomPlot->yAxis->grid()->setPen(gridPen);
	gridPen.setStyle(Qt::DotLine);
	ui->VcustomPlot->yAxis->grid()->setSubGridPen(gridPen);

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
	ui->VcustomPlot->legend->setVisible(true);
	ui->VcustomPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignHCenter);
	ui->VcustomPlot->legend->setBrush(QColor(255, 255, 255, 200));
	QPen legendPen;
	legendPen.setColor(QColor(130, 130, 130, 200));
	ui->VcustomPlot->legend->setBorderPen(legendPen);
	QFont legendFont = font();
	legendFont.setPointSize(10);
	ui->VcustomPlot->legend->setFont(legendFont);
	ui->VcustomPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
	ui->VcustomPlot->replot();
	ui->VcustomPlot->axisRect()->setupFullAxesBox();
}


void EmergencyService::yAxisRangeChangedW(const QCPRange &newRange)
{
	ui->VcustomPlot_2->yAxis->setRange(0, ui->VcustomPlot_2->yAxis->range().upper);// y軸の下限を0に固定
}
void EmergencyService::xAxisRangeChangedW(const QCPRange &newRange)
{
	ui->VcustomPlot_2->xAxis->setRange(-300, ui->VcustomPlot_2->xAxis->range().upper);// y軸の下限を0に固定
}

void EmergencyService::on_graphFilterCBox_2_currentIndexChanged(int index)
{
	if(index == -1 || index == 0)
	{
		ui->graphFilterCBox_2->blockSignals(true);
		ui->graphFilterCBox_2->setCurrentIndex(0);
		ui->graphFilterCBox_2->blockSignals(false);
		initPayPlotW();
	}
	else
		initPayPlotW("WHERE worker_type = "+ui->graphFilterCBox_2->currentData().toString());
}

void EmergencyService::initPayPlotW(QString str)
{
	connect(ui->VcustomPlot_2->yAxis, SIGNAL(rangeChanged(const QCPRange &)), this, SLOT(yAxisRangeChangedW(const QCPRange &)));
	connect(ui->VcustomPlot_2->xAxis, SIGNAL(rangeChanged(const QCPRange &)), this, SLOT(xAxisRangeChangedW(const QCPRange &)));

	ui->VcustomPlot_2->addGraph();
	ui->VcustomPlot_2->setInteraction(QCP::iRangeDrag, true);
	ui->VcustomPlot_2->setInteraction(QCP::iSelectItems, true);
	ui->VcustomPlot_2->setInteraction(QCP::iRangeZoom, true);

	QString strQuery = "SELECT count(repair_tickets_id) AS 'count_worker', "
	"	houses.address "
	"FROM workers_list "
	"INNER JOIN worker ON worker.id = workers_list.worker_id "
	"INNER JOIN workertype ON workertype.id = worker.worker_type "
	"INNER JOIN repair_tickets ON repair_tickets.id = workers_list.repair_tickets_id "
	"INNER JOIN flats ON flats.id = repair_tickets.flat "
	"INNER JOIN houses ON houses.id = flats.address ";
	strQuery.append(str+" ");
	strQuery.append("GROUP BY repair_tickets_id;");

	QSqlQuery q;
	q.exec(strQuery);

	QMap<QString,int> addressLst;
	while(q.next())
	{
			addressLst[q.value("address").toString().split(",")[0]] +=q.value("count_worker").toDouble();
	}
	ui->VcustomPlot_2->clearPlottables();
	QCPBars *fossil = new QCPBars(ui->VcustomPlot_2->xAxis, ui->VcustomPlot_2->yAxis);
	ui->VcustomPlot_2->addPlottable(fossil);

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

	ui->VcustomPlot_2->xAxis->setAutoTicks(false);
	ui->VcustomPlot_2->xAxis->setAutoTickLabels(false);
	ui->VcustomPlot_2->xAxis->setTickVector(ticks);
	ui->VcustomPlot_2->xAxis->setTickVectorLabels(labels);
	ui->VcustomPlot_2->xAxis->setTickLabelRotation(60);
	ui->VcustomPlot_2->xAxis->setSubTickCount(0);
	ui->VcustomPlot_2->xAxis->setTickLength(0, 4);
	ui->VcustomPlot_2->xAxis->grid()->setVisible(true);
	ui->VcustomPlot_2->xAxis->setRange(-300, j*500*4-1000);


	// prepare y axis:
	ui->VcustomPlot_2->yAxis->setRange(0, 15);
	//ui->customPlot_2->yAxis->setRange(0, 0);
	ui->VcustomPlot_2->yAxis->setPadding(5); // a bit more space to the left border
	ui->VcustomPlot_2->yAxis->setLabel("Количество отправлений работников на выполнение заявки\nмай 2015");
	ui->VcustomPlot_2->yAxis->grid()->setSubGridVisible(true);
	QPen gridPen;
	gridPen.setStyle(Qt::SolidLine);
	gridPen.setColor(QColor(0, 0, 0, 25));
	ui->VcustomPlot_2->yAxis->grid()->setPen(gridPen);
	gridPen.setStyle(Qt::DotLine);
	ui->VcustomPlot_2->yAxis->grid()->setSubGridPen(gridPen);

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
	ui->VcustomPlot_2->legend->setVisible(true);
	ui->VcustomPlot_2->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignHCenter);
	ui->VcustomPlot_2->legend->setBrush(QColor(255, 255, 255, 200));
	QPen legendPen;
	legendPen.setColor(QColor(130, 130, 130, 200));
	ui->VcustomPlot_2->legend->setBorderPen(legendPen);
	QFont legendFont = font();
	legendFont.setPointSize(10);
	ui->VcustomPlot_2->legend->setFont(legendFont);
	ui->VcustomPlot_2->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
	ui->VcustomPlot_2->replot();
	ui->VcustomPlot_2->axisRect()->setupFullAxesBox();
}
