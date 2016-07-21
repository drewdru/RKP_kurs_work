#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT
	QSqlDatabase db;
public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private slots:
	void on_pushButton_clicked();
	void on_usertype_currentIndexChanged(int index);

signals:
	void sendUser(const int user);

private:
	Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
