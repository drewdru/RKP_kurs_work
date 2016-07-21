#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Admin.h"
#include "Cashier.h"
#include "EmergencyService.h"
#include "OperatorBD.h"
#include "Accountant.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	this->setWindowTitle("Авторизация");

	db = QSqlDatabase::addDatabase("QMYSQL");
	/*
	db.setHostName("127.0.0.1");
	db.setPort(3306);
	db.setDatabaseName("gkhdb");
	db.setUserName("root");
	db.setPassword("qwer");*/
	QFile file("./db.txt");
	if(!file.open(QIODevice::ReadOnly))
	{
		qDebug() << "error opening file: " << file.error();
		return;
	}
	QTextStream instream(&file);
	QString line = instream.readLine();
	db.setHostName(line);
	db.setPort(instream.readLine().toInt());
	db.setDatabaseName(instream.readLine());
	db.setUserName(instream.readLine());
	db.setPassword(instream.readLine());
	file.close();

	if (!db.open())
	{
		//Error *error = new Error();
		//error->show();
		qDebug()<<"Ошибка соединения";
	}
	QSqlQuery usertypeQuery;
	usertypeQuery.exec("SELECT * FROM usertype");
	while(usertypeQuery.next())
	{
		ui->usertype->addItem(usertypeQuery.value("typeuser").toString(),\
								usertypeQuery.value("id").toInt());
	}
}

MainWindow::~MainWindow()
{
	QSqlDatabase::removeDatabase("QMYSQL");
	delete ui;
}

void MainWindow::on_pushButton_clicked()
{
	QSqlQuery q;
	q.exec("SELECT user.id, usertype.typeuser FROM user, usertype\
			WHERE usertype.id = user.type_user AND user.id = '"\
			+ ui->user->currentData().toString() + "' AND user.pass = '"\
			+ QString(QCryptographicHash::hash(\
				ui->passEdit->text().toUtf8(),\
				QCryptographicHash::Md5).toHex())+"'");

	q.next();
	int userID = q.value("id").toInt();
	QString typeUser = q.value(1).toString();
	qDebug()<<userID<<" "<<typeUser;
	qDebug()<<q.lastError();

	/*db.close();
	db = QSqlDatabase::addDatabase("QMYSQL");
	db.setHostName("127.0.0.1");
	db.setPort(3306);
	db.setDatabaseName("gkhdb");*/

	this->hide();	
	if(typeUser == "администратор")
	{
		/*db.setUserName("root");
		db.setPassword("qwer");
		db.open();*/
		Admin* a = new Admin;
		a->showMaximized();
		connect(this,SIGNAL(sendUser(int)),a,SLOT(recieveUserID(int)));
		emit sendUser(userID);
		a->exec();
		a->deleteLater();
	}
	else if(typeUser == "кассир")
	{
		/*db.setUserName("cashier");
		db.setPassword("1234");
		db.open();*/
		Cashier* c = new Cashier;
		c->showMaximized();
		connect(this,SIGNAL(sendUser(int)),c,SLOT(recieveUserID(int)));
		emit sendUser(userID);
		c->exec();
		c->deleteLater();
	}
	else if(typeUser == "аварийная служба")
	{
		/*db.setUserName("root");
		db.setPassword("qwer");
		db.open();*/
		EmergencyService* es = new EmergencyService;
		es->showMaximized();
		connect(this,SIGNAL(sendUser(int)),es,SLOT(recieveUserID(int)));
		emit sendUser(userID);
		es->exec();
		es->deleteLater();
	}
	else if(typeUser == "оператор")
	{
		/*db.setUserName("root");
		db.setPassword("qwer");
		db.open();*/
		OperatorBD* hrd = new OperatorBD;
		hrd->showMaximized();
		connect(this,SIGNAL(sendUser(int)),hrd,SLOT(recieveUserID(int)));
		emit sendUser(userID);
		hrd->exec();
		hrd->deleteLater();
	}
	else if(typeUser == "бухгалтер")
	{
		/*db.setUserName("root");
		db.setPassword("qwer");
		db.open();*/
		Accountant* ac = new Accountant;
		ac->showMaximized();
		connect(this,SIGNAL(sendUser(int)),ac,SLOT(recieveUserID(int)));
		emit sendUser(userID);
		ac->exec();
		ac->deleteLater();
	}
	else
	{
		/*db.setUserName("root");
		db.setPassword("qwer");
		db.open();*/
		QMessageBox* errorLogin = new QMessageBox(this);
		errorLogin->setText("Неверный логин или пароль");
		errorLogin->exec();
		errorLogin->deleteLater();
	}
	this->show();
	/*db.close();
	db.setUserName("root");
	db.setPassword("qwer");
	db.open();*/

}

void MainWindow::on_usertype_currentIndexChanged(int index)
{
	ui->user->clear();
	QSqlQuery userQuery;
	userQuery.exec(	"SELECT * FROM LoginView "\
					"WHERE type_user = "\
					+ui->usertype->currentData().toString());
	while(userQuery.next())
	{
		ui->user->addItem(userQuery.value("fioinn").toString(),\
								userQuery.value("id").toInt());
	}
	ui->user->setCurrentIndex(-1);
}
