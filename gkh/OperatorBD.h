#ifndef OPERATORBD_H
#define OPERATORBD_H

#include <QDialog>
#include <QtSql>
#include <QtWidgets>
#include "qcustomplot.h"
namespace Ui {
	class OperatorBD;
}

class OperatorBD : public QDialog
{
	Q_OBJECT
	int systemUserID;
	QSqlRelationalTableModel* OperatorBDModel;
	QSqlRelationalTableModel* workerModel;
	QSqlTableModel* workerTypeModel;
	QSqlRelationalTableModel* flat_counterModel;
	QSqlRelationalTableModel* house_counterModel;
public:
	explicit OperatorBD(QWidget *parent = 0);
	~OperatorBD();
	void keyPressEvent(QKeyEvent *e);
private:
	Ui::OperatorBD *ui;
	void initPayPlotI();
	void initPayPlotO();
	void initworkerPlot(QString str = "");
	void initvehiclePlot(QString str = "");

public slots:
	void recieveUserID(int user);
	void onFindButtonClick();
	void yAxisRangeChangedI(const QCPRange &newRange);
	void xAxisRangeChangedI(const QCPRange &newRange);
	void yAxisRangeChangedO(const QCPRange &newRange);
	void xAxisRangeChangedO(const QCPRange &newRange);
	void yAxisRangeChangedW(const QCPRange &newRange);
	void xAxisRangeChangedW(const QCPRange &newRange);
	void yAxisRangeChangedV(const QCPRange &newRange);
	void xAxisRangeChangedV(const QCPRange &newRange);
private slots:
	void onOperatorBDFindButtonClick();
	void recieveOperatorBDCurrentIndex(const QModelIndex &index);

	void recieveCurrentIndex(const QModelIndex &index);

	void on_approveButton_clicked();

	void on_addButton_clicked();

	void on_editButton_clicked();

	void on_addressCBox_currentIndexChanged(int index);

	void on_cancel_insert_Button_clicked();

	void on_insertButton_clicked();

	void on_delete_button_clicked();

	void on_deleteButton_clicked();

	void on_add_button_clicked();

	void on_edit_button_clicked();

	void on_insert_button_clicked();

	void on_insert_ipuButton_clicked();

	void on_insert_opuButton_clicked();

	void on_update_opuButton_clicked();

	void on_address_counterCBox_currentIndexChanged(int index);

	void on_cancel_counterButton_clicked();

	void on_ok_counterButton_clicked();

	void on_fpushButton_clicked();

	void on_hpushButton_2_clicked();

	void on_hpushButto_clicked();

	void on_graphPayPButton_clicked();

	void on_graphClearFilterPButton_clicked();

	void on_graphPayPButton_2_clicked();

	void on_graphClearFilterPButton_2_clicked();

	void on_filtergraphworktypeCBox_currentIndexChanged(int index);

	void on_filtergraphvehicletypeCBox_2_currentIndexChanged(int index);

signals:
	void sendOperatorBDID(const int account);
	void sendOperatorBDData(const QModelIndex &index);
	void sendUser(const int user);
	void sendOperatorBDStates(QString);

	void sendData(const QModelIndex &index);

};

#endif // OPERATORBD_H
