#ifndef CASHIER_H
#define CASHIER_H

#include <QDialog>
#include <QtSql>
#include <QtWidgets>

namespace Ui {
	class Cashier;
}

class Cashier : public QDialog
{
	Q_OBJECT

	QSqlRelationalTableModel* AccountModel;
	int systemUserID;
public:
	explicit Cashier(QWidget *parent = 0);
	~Cashier();
	void keyPressEvent(QKeyEvent *e);

public slots:
	void recieveUserID(int user);

private slots:
	void on_tw_doubleClicked(const QModelIndex &index);
	void onAccountFindButtonClick();
	void recieveAccountCurrentIndex(const QModelIndex &index);

signals:
	void sendAccountID(const int account);
	void sendAccountData(const QModelIndex &index);
	void sendUser(const int user);

private:
	Ui::Cashier *ui;
};

#endif // CASHIER_H
