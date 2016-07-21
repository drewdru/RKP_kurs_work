#include "Admin.h"
#include "ui_Admin.h"
#include "QTime"
#include <QDebug>

void Admin::on_graphfiltertypeuserCBox_currentIndexChanged(int index)
{
	if(index == -1 || index == 0)
	{
		ui->graphfiltertypeuserCBox->blockSignals(true);
		ui->graphfiltertypeuserCBox->setCurrentIndex(0);
		ui->graphfiltertypeuserCBox->blockSignals(false);
		initPlotWidget();
	}
	else
		initPlotWidget("WHERE typeuser = '"+ui->graphfiltertypeuserCBox->currentText()+"'");
}


void Admin::on_comboBox_currentIndexChanged(int index)
{
	//initPlotWidget_2();
	if(index == 0)
	{
		ui->graphfiltertypeuserCBox->blockSignals(true);
		ui->graphfiltertypeuserCBox->blockSignals(false);
		initPlotWidget_2();
	}
	else if(index == 1)
		initPlotWidget_2("WHERE isrelief = 0");
	else
		initPlotWidget_2("WHERE isrelief = 1");

}

void Admin::initPlotWidget(QString str)
{

	QString strQuery = "SELECT usertype.typeuser,work_space.address "
	"FROM user "
	"INNER JOIN work_space ON work_space.id = user.office "
	"INNER JOIN usertype ON usertype.id = user.type_user ";
	strQuery.append(str+";");
	QSqlQuery q;
	q.exec(strQuery);

	QMap<QString,int> addressLst;
	while(q.next())
	{
		addressLst[q.value("address").toString().split(",")[0]] ++;
	}
	ui->customPlot->clearPlottables();
	QCPBars *fossil = new QCPBars(ui->customPlot->xAxis, ui->customPlot->yAxis);
	ui->customPlot->addPlottable(fossil);
	// set names and colors:
	QPen pen;
	pen.setWidthF(1.2);
	fossil->setName("Пользователи программы");
	pen.setColor(QColor(255, 131, 0));
	fossil->setPen(pen);
	fossil->setBrush(QColor(255, 131, 0, 50));

	// prepare x axis with country labels:
	QVector<double> ticks;
	QVector<QString> labels;

	for(int i =0;i<addressLst.count();i++)
	{
		ticks.append(i+1);
	}
	labels = addressLst.keys().toVector();

	ui->customPlot->xAxis->setAutoTicks(false);
	ui->customPlot->xAxis->setAutoTickLabels(false);
	ui->customPlot->xAxis->setTickVector(ticks);
	ui->customPlot->xAxis->setTickVectorLabels(labels);
	ui->customPlot->xAxis->setTickLabelRotation(60);
	ui->customPlot->xAxis->setSubTickCount(0);
	ui->customPlot->xAxis->setTickLength(0, 4);
	ui->customPlot->xAxis->grid()->setVisible(true);
	ui->customPlot->xAxis->setRange(0, 8);

	// prepare y axis:
	ui->customPlot->yAxis->setRange(0, 12.1);
	ui->customPlot->yAxis->setPadding(5); // a bit more space to the left border
	ui->customPlot->yAxis->setLabel("Количество пользователей системы");
	ui->customPlot->yAxis->grid()->setSubGridVisible(true);
	QPen gridPen;
	gridPen.setStyle(Qt::SolidLine);
	gridPen.setColor(QColor(0, 0, 0, 25));
	ui->customPlot->yAxis->grid()->setPen(gridPen);
	gridPen.setStyle(Qt::DotLine);
	ui->customPlot->yAxis->grid()->setSubGridPen(gridPen);

	// Add data:
	QVector<double> fossilData;//, nuclearData, regenData;
	//fossilData  << 0.86*10.5 << 0.83*5.5 << 0.84*5.5 << 0.52*5.8 << 0.89*5.2 << 0.90*4.2 << 0.67*11.2;
	/*for(int i =0; i<addressLst.count();i++)
		fossilData.append(addressLst[i]);*/
	QMapIterator<QString, int> i(addressLst);
	while (i.hasNext()) {
		i.next();
		//qDebug() << i.key() << ": " << i.value() << endl;
		fossilData.append(i.value());
	}
	/*for(int i =0; i<addressLst.count();i++)
		fossilData.append(addressLst[addressLst.key(i)]);*/
	fossil->setData(ticks, fossilData);

	// setup legend:
	ui->customPlot->legend->setVisible(true);
	ui->customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignHCenter);
	ui->customPlot->legend->setBrush(QColor(255, 255, 255, 200));
	QPen legendPen;
	legendPen.setColor(QColor(130, 130, 130, 200));
	ui->customPlot->legend->setBorderPen(legendPen);
	QFont legendFont = font();
	legendFont.setPointSize(10);
	ui->customPlot->legend->setFont(legendFont);
	ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
	ui->customPlot->replot();
}

void Admin::yAxisRangeChanged(const QCPRange &newRange)
{
	ui->customPlot_2->yAxis->setRange(0, ui->customPlot_2->yAxis->range().upper);// y軸の下限を0に固定
}
void Admin::xAxisRangeChanged(const QCPRange &newRange)
{
	ui->customPlot_2->xAxis->setRange(-300, ui->customPlot_2->xAxis->range().upper);// y軸の下限を0に固定
}
void Admin::initPlotWidget_2(QString str)
{
	connect(ui->customPlot_2->yAxis, SIGNAL(rangeChanged(const QCPRange &)), this, SLOT(yAxisRangeChanged(const QCPRange &)));
	connect(ui->customPlot_2->xAxis, SIGNAL(rangeChanged(const QCPRange &)), this, SLOT(xAxisRangeChanged(const QCPRange &)));

	ui->customPlot_2->addGraph();
	ui->customPlot_2->setInteraction(QCP::iRangeDrag, true);
	ui->customPlot_2->setInteraction(QCP::iSelectItems, true);
	ui->customPlot_2->setInteraction(QCP::iRangeZoom, true);


	QString strQuery = "SELECT houses.address,accounts.money "
	"FROM accounts "
	"INNER JOIN flats ON flats.id = accounts.flat "
	"INNER JOIN houses ON houses.id = flats.address ";
	strQuery.append(str+";");
	QSqlQuery q;
	q.exec(strQuery);

	QMap<QString,int> addressLst;
	while(q.next())
	{
		//qDebug()<<q.value("address").toString().split(",")[0];
		addressLst[q.value("address").toString().split(",")[0]] +=q.value("money").toDouble();
	}
	ui->customPlot_2->clearPlottables();
	QCPBars *fossil = new QCPBars(ui->customPlot_2->xAxis, ui->customPlot_2->yAxis);
	ui->customPlot_2->addPlottable(fossil);

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

	ui->customPlot_2->xAxis->setAutoTicks(false);
	ui->customPlot_2->xAxis->setAutoTickLabels(false);
	ui->customPlot_2->xAxis->setTickVector(ticks);
	ui->customPlot_2->xAxis->setTickVectorLabels(labels);
	ui->customPlot_2->xAxis->setTickLabelRotation(60);
	ui->customPlot_2->xAxis->setSubTickCount(0);
	ui->customPlot_2->xAxis->setTickLength(0, 4);
	ui->customPlot_2->xAxis->grid()->setVisible(true);
	ui->customPlot_2->xAxis->setRange(-300, j*500*4-1000);


	// prepare y axis:
	ui->customPlot_2->yAxis->setRange(0, 18000);
	//ui->customPlot_2->yAxis->setRange(0, 0);
	ui->customPlot_2->yAxis->setPadding(5); // a bit more space to the left border
	ui->customPlot_2->yAxis->setLabel("Сумма задолжностей на 2015 год");
	ui->customPlot_2->yAxis->grid()->setSubGridVisible(true);
	QPen gridPen;
	gridPen.setStyle(Qt::SolidLine);
	gridPen.setColor(QColor(0, 0, 0, 25));
	ui->customPlot_2->yAxis->grid()->setPen(gridPen);
	gridPen.setStyle(Qt::DotLine);
	ui->customPlot_2->yAxis->grid()->setSubGridPen(gridPen);

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
	ui->customPlot_2->legend->setVisible(true);
	ui->customPlot_2->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignHCenter);
	ui->customPlot_2->legend->setBrush(QColor(255, 255, 255, 200));
	QPen legendPen;
	legendPen.setColor(QColor(130, 130, 130, 200));
	ui->customPlot_2->legend->setBorderPen(legendPen);
	QFont legendFont = font();
	legendFont.setPointSize(10);
	ui->customPlot_2->legend->setFont(legendFont);
	ui->customPlot_2->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
	ui->customPlot_2->replot();
	ui->customPlot_2->axisRect()->setupFullAxesBox();
}

