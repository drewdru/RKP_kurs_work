#include "Accountant.h"
#include "ui_Accountant.h"

void Accountant::yAxisRangeChanged(const QCPRange &newRange)
{
	ui->payCPlot->yAxis->setRange(0, ui->payCPlot->yAxis->range().upper);// y軸の下限を0に固定
}
void Accountant::xAxisRangeChanged(const QCPRange &newRange)
{
	ui->payCPlot->xAxis->setRange(-300, ui->payCPlot->xAxis->range().upper);// y軸の下限を0に固定
}
void Accountant::on_graphPayPButton_clicked()
{
	initPayPlot();
}
void Accountant::on_graphClearFilterPButton_clicked()
{
	ui->StartDate->setDateTime(QDateTime::fromString("01.01.2000 00:00","dd.MM.yyyy hh:mm"));
	ui->EndDate->setDateTime(QDateTime::currentDateTime());
	initPayPlot();
}
void Accountant::initPayPlot()
{
	connect(ui->payCPlot->yAxis, SIGNAL(rangeChanged(const QCPRange &)), this, SLOT(yAxisRangeChanged(const QCPRange &)));
	connect(ui->payCPlot->xAxis, SIGNAL(rangeChanged(const QCPRange &)), this, SLOT(xAxisRangeChanged(const QCPRange &)));

	ui->payCPlot->addGraph();
	ui->payCPlot->setInteraction(QCP::iRangeDrag, true);
	ui->payCPlot->setInteraction(QCP::iSelectItems, true);
	ui->payCPlot->setInteraction(QCP::iRangeZoom, true);


	/*QString strQuery = "SELECT houses.address,accounts.money "
	"FROM accounts "
	"INNER JOIN flats ON flats.id = accounts.flat "
	"INNER JOIN houses ON houses.id = flats.address ";*/
	QString strQuery = "SELECT ABS(cashier_log.new_money - cashier_log.old_money) AS 'pay', "
	"	cashier_log.date,houses.address "
	"FROM cashier_log "
	"INNER JOIN accounts ON accounts.id = cashier_log.account "
	"INNER JOIN flats ON flats.id = accounts.flat "
	"INNER JOIN houses ON houses.id = flats.address "
	"WHERE date BETWEEN STR_TO_DATE('%1','%d.%m.%Y %H:%i') "\
	"AND STR_TO_DATE('%2', '%d.%m.%Y %H:%i');";
	strQuery = strQuery.arg(ui->StartDate->text(),ui->EndDate->text());
	QSqlQuery q;
	q.exec(strQuery);

	QMap<QString,int> addressLst;
	while(q.next())
	{
			addressLst[q.value("address").toString().split(",")[0]] +=q.value("pay").toDouble();
	}
	ui->payCPlot->clearPlottables();
	QCPBars *fossil = new QCPBars(ui->payCPlot->xAxis, ui->payCPlot->yAxis);
	ui->payCPlot->addPlottable(fossil);

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
	ui->payCPlot->yAxis->setRange(0, 18000);
	//ui->customPlot_2->yAxis->setRange(0, 0);
	ui->payCPlot->yAxis->setPadding(5); // a bit more space to the left border
	ui->payCPlot->yAxis->setLabel("Оплаченная сумма за " + QDateTime::currentDateTime().toString("yyyy") + " год");
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

