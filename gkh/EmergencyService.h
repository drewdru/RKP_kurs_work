#ifndef EMERGENCYSERVICE_H
#define EMERGENCYSERVICE_H

#include <QDialog>
#include <QtSql>
#include <QtWidgets>
#include "qcustomplot.h"
namespace Ui {
	class EmergencyService;
}

class EmergencyService : public QDialog
{
	Q_OBJECT
	int systemUserID;	
	QSqlRelationalTableModel* EmergencyModel;

public:
	explicit EmergencyService(QWidget *parent = 0);
	~EmergencyService();
	void keyPressEvent(QKeyEvent *e);

public slots:
	void recieveUserID(int user);
	void yAxisRangeChangedV(const QCPRange &newRange);
	void xAxisRangeChangedV(const QCPRange &newRange);
	void yAxisRangeChangedW(const QCPRange &newRange);
	void xAxisRangeChangedW(const QCPRange &newRange);

private slots:
	void onEmergencyFindButtonClick();
	void recieveEmergencyCurrentIndex(const QModelIndex &index);

	void on_stateButton_clicked();

	void on_changeSButton_clicked();

	void on_pushButton_clicked();

	void on_updatesumButton_clicked();

	void on_graphFilterCBox_currentIndexChanged(int index);

	void on_graphFilterCBox_2_currentIndexChanged(int index);

signals:
	void sendEmergencyID(const int account);
	void sendEmergencyData(const QModelIndex &index);
	void sendEmergencyStates(QString);

	void sendUser(const int user);

private:
	Ui::EmergencyService *ui;
	void initPayPlotV(QString str= "");
	void initPayPlotW(QString str= "");
};

#endif // EMERGENCYSERVICE_H
