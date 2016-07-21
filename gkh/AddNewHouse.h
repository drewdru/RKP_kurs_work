#ifndef ADDNEWHOUSE_H
#define ADDNEWHOUSE_H

#include <QDialog>

namespace Ui {
	class AddNewHouse;
}

class AddNewHouse : public QDialog
{
	Q_OBJECT

public:
	explicit AddNewHouse(QWidget *parent = 0);
	~AddNewHouse();

private slots:
	void on_pushButton_5_clicked();

private:
	Ui::AddNewHouse *ui;
};

#endif // ADDNEWHOUSE_H
