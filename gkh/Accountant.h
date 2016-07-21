#ifndef ACCOUNTANT_H
#define ACCOUNTANT_H

#include <QDialog>
#include <QtSQL>
#include <QKeyEvent>
#include <QTextDocument>
#include "qcustomplot.h"
namespace Ui {
	class Accountant;
}

class Accountant : public QDialog
{
	Q_OBJECT
	int systemUserID;
	QSqlRelationalTableModel* tariffModel;
	QSqlRelationalTableModel* normModel;
	QSqlRelationalTableModel* accountModel;
	QSqlRelationalTableModel* repairModel;
	QSqlRelationalTableModel* vehicleModel;
	QSqlRelationalTableModel* cashier_log;
	QSqlQueryModel movements;
	QSqlQueryModel service;

public:
	explicit Accountant(QWidget *parent = 0);
	~Accountant();
	void keyPressEvent(QKeyEvent *e);

public slots:
	void recieveUserID(int user);
	void yAxisRangeChanged(const QCPRange &newRange);
	void xAxisRangeChanged(const QCPRange &newRange);
private slots:
	void on_tariff_updateButton_clicked();

	void on_norm_updateButton_clicked();

	void on_pushButton_clicked();

	void on_vehicleView_clicked(const QModelIndex &index);

	void on_pushButton_2_clicked();

	void on_tpushButton_clicked();

	void on_npushButton_clicked();

	void on_apushButton_clicked();

	void on_aspushButton_clicked();

	void on_aspushButton_2_clicked();

	void on_pushButton_3_clicked();

	void on_tariff_reportButton_clicked();

	void on_norm_reportButton_clicked();

	void on_repair_report_PButton_clicked();

	void on_repair_report_PButton_2_clicked();

	void on_vehicle_repairPButton_clicked();

	void on_vehicle_repairPButton_2_clicked();

	void on_graphPayPButton_clicked();

	void on_graphClearFilterPButton_clicked();

private:
	Ui::Accountant *ui;
	void print(QString pstr);
	void initPayPlot();
};

#endif // ACCOUNTANT_H
