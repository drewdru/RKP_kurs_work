#ifndef FINDEMERGENCYCBOX_H
#define FINDEMERGENCYCBOX_H

#include <QWidget>
#include <QtSQL>
namespace Ui {
	class FindEmergencyCBox;
}

class FindEmergencyCBox : public QWidget
{
	Q_OBJECT
	QSqlRelationalTableModel* EmergencyModel;
	QModelIndex currentIndex;
	QString state;
public:
	explicit FindEmergencyCBox(QWidget *parent = 0);
	~FindEmergencyCBox();
	void SetModels(QSqlRelationalTableModel* EmergencyModel);
private:
	Ui::FindEmergencyCBox *ui;
public slots:
	void recieveData(const QModelIndex &index);
	void recieveState(const QString &str);
signals:
	void getCurrentIndex();
	void sendCurrentIndex(const QModelIndex &index);
private slots:
	void on_FindFirstButton_clicked();
	void on_FindPreviousButton_clicked();
	void on_FindNextButton_clicked();
	void on_FindLastButton_clicked();
	void on_pushButton_clicked();
	void on_pushButton_2_clicked();
};

#endif // FINDEMERGENCYCBOX_H
