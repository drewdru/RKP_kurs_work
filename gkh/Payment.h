#ifndef PAYMENT_H
#define PAYMENT_H

#include <QDialog>
#include <QtSql>
#include <QtWidgets>

namespace Ui {
	class Payment;
}

class Payment : public QDialog
{
	Q_OBJECT
	int AccountID;
	QSqlTableModel* AccountModel;
	int systemUserID;
public:	
	explicit Payment(QWidget *parent = 0);
	~Payment();
public slots:
	void recieveAccountID(int account);
	void recieveUserID(int user);
private slots:

	void on_payMoney_valueChanged(double arg1);

	void on_moneytopay_valueChanged(double arg1);

	void on_pushButton_clicked();

	void on_pushButton_2_clicked();

	void on_money_valueChanged(double arg1);

private:
	Ui::Payment *ui;
	void print();
};

#endif // PAYMENT_H
