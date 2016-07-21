#ifndef EMERGENCYBID_H
#define EMERGENCYBID_H

#include <QDialog>
#include <QtSql>

namespace Ui {
	class EmergencyBid;
}

class EmergencyBid : public QDialog
{
	Q_OBJECT
	int EmergencyID;
	int systemUserID;
	QSqlRelationalTableModel* EmergencyModel;
	QSqlRelationalTableModel* WorkerModel;
	QSqlRelationalTableModel* VehicleModel;	
public:
	explicit EmergencyBid(QWidget *parent = 0);
	~EmergencyBid();
public slots:
	void recieveEmergencyID(int account);
	void recieveUserID(int user);
	void recieveHTMLData(QString str);
private slots:
	void on_workerTypeCBox_currentIndexChanged(int index);

	void on_vehicleTypeCBox_currentIndexChanged(int index);

	void on_addWorker_clicked();

	void on_deleteWorker_clicked();

	void on_addVehicle_clicked();

	void on_deleteVehicle_clicked();

private:
	Ui::EmergencyBid *ui;

	void addMovLog(float dist);
};

#endif // EMERGENCYBID_H
