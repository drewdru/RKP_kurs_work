#include "Admin.h"
#include "ui_Admin.h"
#include <QDebug>
#include <QtWidgets>
#include "AddNewHouse.h"
#include <QModelIndex>
Admin::Admin(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::Admin)
{
	ui->setupUi(this);
	this->setWindowFlags(Qt::Window);
	this->setWindowTitle("Администратор");
	ui->frame->hide();
	ui->frame_2->hide();

	/*rec = repairModel->record();
	repairModel->setHeaderData( rec.indexOf("id"), Qt::Horizontal, QObject::tr("№") );
	repairModel->setHeaderData( rec.indexOf("license_number"), Qt::Horizontal, QObject::tr("Номер транспорта") );
	repairModel->setHeaderData( rec.indexOf("vehicletype"), Qt::Horizontal, QObject::tr("Тип транспорта") );
	repairModel->setHeaderData( rec.indexOf("garage"), Qt::Horizontal, QObject::tr("Адрес гаража") );
	ui->repairView->resizeColumnsToContents();*/

	/*UserModel->relationModel(столбец)->index(строка,колонкавстолбце).data().toInt();*/
	UserModel = new QSqlRelationalTableModel(this,QSqlDatabase::database());
	UserModel->setTable("user");
	qDebug()<< UserModel->select();
	QSqlRecord rec = UserModel->record();
	UserModel->setSort(rec.indexOf("fio"),Qt::AscendingOrder);
	UserModel->setRelation(rec.indexOf("type_user"), QSqlRelation("usertype", "id", "typeuser"));
	UserModel->setRelation(rec.indexOf("office"), QSqlRelation("work_space", "id", "address"));
	qDebug()<< UserModel->select();
	UserModel->setHeaderData( 6, Qt::Horizontal, QObject::tr("office") );
	ui->tw->setModel(UserModel);

	accountModel = new QSqlRelationalTableModel(this,QSqlDatabase::database());
	accountModel->setTable("CashierViewAdmin");
	qDebug()<< accountModel->select();
	rec = accountModel->record();
	accountModel->setSort(rec.indexOf("fio"),Qt::AscendingOrder);
	qDebug()<< accountModel->select();
	ui->tableView->setModel(accountModel);
	ui->tableView->resizeColumnsToContents();

	//ui->tw->setColumnHidden(rec.indexOf("id"),true);
	ui->tw->resizeColumnsToContents();
	ui->tw->horizontalHeader()->moveSection(rec.indexOf("fio"),\
											rec.indexOf("pass"));

	UserTypeModel = new QSqlTableModel(this,QSqlDatabase::database());
	UserTypeModel->setTable("usertype");
	qDebug()<< UserTypeModel->select();
	UserTypeModel->setHeaderData(1, Qt::Horizontal, \
		"Фильтр по типу");
	for(int nRow =0; nRow<UserTypeModel->rowCount();nRow++)
	{
		rec = UserTypeModel->record(nRow);
		ui->user_type->addItem(rec.value("typeuser").toString(),\
								rec.value("id").toInt());

		ui->graphfiltertypeuserCBox->blockSignals(true);
		ui->graphfiltertypeuserCBox->addItem(rec.value("typeuser").toString(),\
			rec.value("id").toInt());
		ui->graphfiltertypeuserCBox->blockSignals(false);
	}
	ui->graphfiltertypeuserCBox->setCurrentIndex(-1);
	ui->widget->SetModels(UserModel,UserTypeModel);
	connect(this, SIGNAL(sendData(QModelIndex)), ui->widget,\
			SLOT(recieveData(QModelIndex)));
	connect(ui->widget, SIGNAL(getCurrentIndex()), this,\
			SLOT(onFindButtonClick()));
	connect(ui->widget, SIGNAL(sendCurrentIndex(QModelIndex)), this,\
			SLOT(recieveCurrentIndex(QModelIndex)));

	ui->widget_2->SetModels(accountModel);
	connect(this, SIGNAL(sendAccountData(QModelIndex)), ui->widget_2,\
			SLOT(recieveData(QModelIndex)));
	connect(ui->widget_2, SIGNAL(getCurrentIndex()), this,\
			SLOT(onAccountFindButtonClick()));
	connect(ui->widget_2, SIGNAL(sendCurrentIndex(QModelIndex)), this,\
			SLOT(recieveAccountCurrentIndex(QModelIndex)));

	updateaddress();
	ui->comboBox->addItem("Все");
	ui->comboBox->addItem("Без льгот");
	ui->comboBox->addItem("Льготники");
	//ui->comboBox->setCurrentIndex(-1);
}



void Admin::updateaddress()
{
	ui->address->clear();
	ui->address->blockSignals(1);
	ui->address->addItem("Добавить новый дом");
	QSqlQueryModel modeladdressCBox;
	modeladdressCBox.setQuery("SELECT id,address FROM Houses ");
	if (modeladdressCBox.lastError().isValid()) {
		qDebug() << modeladdressCBox.lastError();
	}
	QSqlRecord rec = modeladdressCBox.record();
	for(int nRow =0; nRow<modeladdressCBox.rowCount();nRow++)
	{
		rec = modeladdressCBox.record(nRow);
		ui->address->addItem(rec.value("address").toString(),\
								rec.value("id").toInt());
	}
	ui->address->setCurrentIndex(-1);
	ui->address->blockSignals(0);
}

void Admin::recieveCurrentIndex(const QModelIndex &index)
{
	ui->tw->setCurrentIndex(index);
	ui->tw->setFocus();
}
void Admin::recieveUserID(int user)
{
	this->systemUserID = user;
}
void Admin::onFindButtonClick()
{
	emit sendData(ui->tw->currentIndex());
}

void Admin::recieveAccountCurrentIndex(const QModelIndex &index)
{
	ui->tableView->setCurrentIndex(index);
	//ui->tableView->setFocus();
}
void Admin::onAccountFindButtonClick()
{
	emit sendAccountData(ui->tableView->currentIndex());	
}

Admin::~Admin()
{
	delete ui;
}

//запрет закрытия при нажатии ESC
void Admin::keyPressEvent(QKeyEvent *e) {
	if(e->key() != Qt::Key_Escape)
		QDialog::keyPressEvent(e);
}

void Admin::on_add_button_clicked()
{
	ui->frame->show();
	ui->insert_button->setText("Добавить");
}

void Admin::on_insert_button_clicked()
{
	int ind = 0;
	if(ui->insert_button->text() == "Добавить")
	{
		UserModel->insertRow(ind);
	}
	else if(ui->insert_button->text() == "Изменить")
	{
		ind = ui->tw->currentIndex().row();
	}

	QSqlRecord rec = UserModel->record();

	UserModel->setData(UserModel->index(ind, rec.indexOf("pass")),\
		QString(QCryptographicHash::hash(ui->pass->text().toUtf8(),\
		QCryptographicHash::Md5).toHex()));
	UserModel->setData(UserModel->index(ind, rec.indexOf("typeuser")),\
						ui->user_type->currentData().toInt());
	UserModel->setData(UserModel->index(ind, rec.indexOf("fio")),\
						ui->fio->text());
	UserModel->setData(UserModel->index(ind, rec.indexOf("inn")),\
						ui->inn->text());
	UserModel->setData(UserModel->index(ind, rec.indexOf("registration")),\
						ui->reg->text());
	if (!UserModel->submitAll()) {
		qDebug() << "Insertion error!";
	}
	ui->frame->hide();

	QModelIndex index= ui->tw->currentIndex();
	UserModel->select();
	ui->tw->setCurrentIndex(index);


	ui->pass->clear();
	ui->user_type->setCurrentIndex(0);
	ui->fio->clear();
	ui->inn->clear();
	ui->reg->clear();
}

void Admin::on_edit_button_clicked()
{
	ui->frame->show();
	ui->insert_button->setText("Изменить");

	int ind = ui->tw->currentIndex().row();
	QSqlRecord rec = UserModel->record(ind);
	ui->pass->setText(rec.value("pass").toString());
	ui->user_type->setCurrentIndex(ui->user_type->findText(
									rec.value("typeuser").toString()));
	ui->fio->setText(rec.value("fio").toString());
	ui->inn->setText(rec.value("inn").toString());
	ui->reg->setText(rec.value("registration").toString());
}

void Admin::on_delete_button_clicked()
{
	UserModel->removeRow(ui->tw->currentIndex().row());
	qDebug()<<UserModel->lastError();
	QModelIndex ind= ui->tw->currentIndex();
	UserModel->select();
	ui->tw->setCurrentIndex(ind);
}

void Admin::on_cancel_insert_button_clicked()
{
	ui->frame->hide();
	ui->pass->clear();
	ui->user_type->setCurrentIndex(0);
	ui->fio->clear();
	ui->inn->clear();
	ui->reg->clear();
}


void Admin::on_edit_button_2_clicked()
{
	ui->frame_2->show();
	int ind = ui->tableView->currentIndex().row();
	QSqlRecord rec = accountModel->record(ind);
	ui->login_2->setText(rec.value("login").toString());
	ui->pass_2->setText(rec.value("pass").toString());
	ui->fio_2->setText(rec.value("fio").toString());
	ui->address->setCurrentText(rec.value("address").toString());
	ui->flat_number->setText(rec.value("flat_number").toString());
}

void Admin::on_cancel_insert_button_2_clicked()
{
	ui->frame_2->hide();
	ui->login_2->clear();
	ui->pass_2->clear();
	ui->fio_2->clear();
	updateaddress();
}

void Admin::on_insert_button_2_clicked()
{
	int ind = ui->tableView->currentIndex().row();
	QSqlRecord rec = accountModel->record();

	accountModel->setData(accountModel->index(ind, rec.indexOf("login")),\
						ui->login_2->text());
	accountModel->setData(accountModel->index(ind, rec.indexOf("pass")),\
		QString(QCryptographicHash::hash(ui->pass_2->text().toUtf8(),\
		QCryptographicHash::Md5).toHex()));
	accountModel->setData(accountModel->index(ind, rec.indexOf("fio")),\
						ui->fio_2->text());
	if (!accountModel->submitAll())
	{
		qDebug()<<accountModel->lastError();
	}

	QSqlQuery query;
	query.prepare("UPDATE flats SET address =:value1  WHERE id=:value2;");
	query.bindValue( ":value1", ui->address->currentData().toString());
	query.bindValue( ":value2", accountModel->index(ui->tableView->\
		currentIndex().row(),rec.indexOf("idflat")).data().toString());
	if (!query.exec()) {
		qDebug() << query.lastError();
	}

	query.prepare("UPDATE flats SET flat_number = :value1 WHERE id=:value2;");
	query.bindValue( ":value1", ui->flat_number->text());
	query.bindValue( ":value2", accountModel->index(ui->tableView->\
		currentIndex().row(),rec.indexOf("idflat")).data().toString());
	if (!query.exec()) {
		qDebug() << query.lastError();
	}

	QModelIndex index= ui->tableView->currentIndex();
	accountModel->select();
	ui->tableView->setCurrentIndex(index);
	/*if (!accountModel->submitAll())
	{
		qDebug()<<accountModel->lastError();
		if(accountModel->lastError().number()==1393)
		{
			int a = QMessageBox::question(this,\
				"Введённый адрес не существует",
				"Введённый адрес не существует.\n"
				"Добавить новый адрес?",\
				"ДА","Нет");
			if(a == 0)
			{
				AddNewFlatHouse NewFH;
				NewFH.exec();
				updateaddress();
				//return;
			}
		}
		else
			qDebug()<<accountModel->lastError();
	}*/
	ui->frame_2->hide();
	ui->login_2->clear();
	ui->pass_2->clear();
	ui->fio_2->clear();
	updateaddress();
}

void Admin::on_address_currentIndexChanged(int index)
{
	if(index == 0)
	{
		AddNewHouse NewFH;
		NewFH.exec();
		ui->address->setCurrentIndex(-1);
		updateaddress();
	}
}



