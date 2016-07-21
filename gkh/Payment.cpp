#include "Payment.h"
#include "ui_Payment.h"
#include <QDebug>
#include <QPrinter>
#include <QPrintDialog>

Payment::Payment(QWidget *parent) :
QDialog(parent),
ui(new Ui::Payment)
{
	ui->setupUi(this);
	AccountModel = new QSqlTableModel(this,QSqlDatabase::database());
}

Payment::~Payment()
{
	delete ui;
}

void Payment::recieveUserID(int user)
{
	this->systemUserID = user;
}

void Payment::recieveAccountID(int account)
{
	AccountID = account;
	AccountModel->setTable("CashierView");
	AccountModel->setFilter("id='"+QString::number(AccountID)+"'");
	qDebug()<<AccountModel->select();
	ui->tableView->setModel(AccountModel);
	QSqlRecord rec = AccountModel->record(0);
	ui->tableView->setCurrentIndex(AccountModel->index(0,0));
	ui->money->setValue(rec.value("money").toDouble());
}

void Payment::on_payMoney_valueChanged(double arg1)
{
	ui->moneytopay->setMaximum(ui->payMoney->value());
	ui->moneytopay->setValue(ui->payMoney->value());
}

void Payment::on_moneytopay_valueChanged(double arg1)
{
	double delivery = ui->payMoney->value() - arg1;
	ui->delivery->setValue(delivery);
}

void Payment::on_pushButton_clicked()
{
	QSqlQuery query;
    query.prepare("UPDATE accounts SET money = money - :value1, last_cashier = :value3 "
					"WHERE id=:value2;");
	query.bindValue( ":value1", ui->moneytopay->value());
	query.bindValue( ":value2", AccountID);
    query.bindValue( ":value3", systemUserID);
	if (!query.exec()) {
        qDebug()<<query.lastQuery();

		qDebug() << query.lastError();
	}
	this->print();
	this->close();
}

void Payment::on_pushButton_2_clicked()
{
	this->close();
}

void Payment::print()
{

	QSqlRecord rec = AccountModel->record(ui->tableView->currentIndex().row());
	QTextDocument check;
	QString strF("<H4>УК \"Солнышко\"<br>ДОБРО ПОЖАЛОВАТЬ!</H4>"
	"<table>"
	"<tr><td>УК: \"Солнышко\"</td></tr>"
	"<tr><td>ЛИЦЕВОЙ СЧЁТ: %1</td></tr>"
	"<tr><td>ФИО: %2</td></tr>"
	"<tr><td>Адр: %3, кв. %4</td></tr>"
	"<tr><td style=\"float:left\">%5</td>"
	"<td style=\"float:right\">КАССИР:%6</td></tr>"
	"<tr><td>ПРОДАЖА</td></tr>"
	"<tr><td style=\"float:left\">Услуги ЖКХ</td>"
	"<td style=\"float:right\">=%7</td></tr>"
	"<tr><td style=\"font-size: 12pt;float:left;font-weight:bold\">ИТОГ</td>"
	"<td style=\"font-size: 12pt;float:right;font-weight:bold\">=%7</td></tr>"
	"<tr><td style=\"float:left\">НАЛИЧНЫМИ</td>"
	"<td style=\"float:right\">=%8</td></tr>"
	"<tr><td style=\"float:left\">СДАЧА</td>"
	"<td style=\"float:right\">=%9</td></tr>"
	"</table>");
	QString str = strF.arg(rec.value("id").toString(),
		rec.value("fio").toString(),
		rec.value("address").toString(),
		rec.value("flat_number").toString(),
		QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm"),
		QString::number(systemUserID),
		ui->moneytopay->text(),
		ui->payMoney->text(),
		ui->delivery->text());
	check.setHtml(str);
	QPrinter printer(QPrinter::HighResolution);
	QPrintDialog dialog(&printer, this);
	dialog.setWindowTitle(tr("Print check"));
	if (dialog.exec() == QDialog::Accepted)
	{
		check.print(&printer);
	}
}

void Payment::on_money_valueChanged(double arg1)
{
	ui->payMoney->setMaximum(999999);
}
