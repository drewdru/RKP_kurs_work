#ifndef ADMIN_H
#define ADMIN_H

#include <QDialog>
#include <QtSQL>
#include "qcustomplot.h"
namespace Ui {
class Admin;
}

class Admin : public QDialog
{
    Q_OBJECT
	QSqlRelationalTableModel* UserModel;
	QSqlRelationalTableModel* accountModel;
	QSqlTableModel* UserTypeModel;
	int systemUserID;
public:

    explicit Admin(QWidget *parent = 0);
    ~Admin();
    void RefreshTable();
    void keyPressEvent(QKeyEvent *e);
	void updateaddress();

private slots:
    void on_add_button_clicked();

    void on_insert_button_clicked();

    void on_edit_button_clicked();

	void on_delete_button_clicked();

	void on_cancel_insert_button_clicked();

	void onFindButtonClick();
	void recieveCurrentIndex(const QModelIndex &index);

	void onAccountFindButtonClick();
	void recieveAccountCurrentIndex(const QModelIndex &index);

	void on_edit_button_2_clicked();

	void on_cancel_insert_button_2_clicked();

	void on_insert_button_2_clicked();

	void on_address_currentIndexChanged(int index);


	void on_graphfiltertypeuserCBox_currentIndexChanged(int index);

	void on_comboBox_currentIndexChanged(int index);

public slots:
	void recieveUserID(int user);
	void yAxisRangeChanged(const QCPRange &newRange);
	void xAxisRangeChanged(const QCPRange &newRange);
signals:
	void sendData(const QModelIndex &index);
	void sendAccountData(const QModelIndex &index);

private:
    Ui::Admin *ui;
	bool notify ( QObject * o, QEvent * e );
	void initPlotWidget(QString str= "");
	void initPlotWidget_2(QString str= "");
};

#endif // ADMIN_H
