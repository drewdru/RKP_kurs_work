#include "FindUserCBox.h"
#include "ui_FindUserCBox.h"
#include <QMessageBox>
FindUserCBox::FindUserCBox(QWidget *parent) :
QWidget(parent),
ui(new Ui::FindUserCBox)
{
	ui->setupUi(this);

	ui->typeSearchCBox->addItem("ФИО","fio");
	ui->typeSearchCBox->addItem("ИНН","inn");
	ui->typeSearchCBox->addItem("Прописка","registration");
	ui->typeSearchCBox->addItem("Офис","office");

}

void FindUserCBox::SetModels(QSqlRelationalTableModel* UserModel,\
							QSqlTableModel* UserTypeModel,bool flag)
{
	userModel = UserModel;
	userTypeModel = UserTypeModel;
	ui->master->setModel(userTypeModel);
	ui->master->setColumnHidden(0,true);
	this->flag = flag;
	if(flag)
	{
		ui->group_button->setVisible(false);
		ui->group_concat_button->setVisible(false);
	}
}
void FindUserCBox::recieveData(const QModelIndex &index)
{
	currentIndex = index;
}

FindUserCBox::~FindUserCBox()
{
	delete ui;
}

void FindUserCBox::on_reset_filter_clicked()
{
	userModel->setFilter("");
	//userModel->setTable("user");
    userModel->select();
}

void FindUserCBox::on_master_clicked(const QModelIndex &index)
{
	QSqlRecord rec = userTypeModel->record(index.row());
	if(flag)
		userModel->setFilter("worker_type LIKE '"+rec.value("name").toString()+"'");
	else
		userModel->setFilter("type_user = "+rec.value("id").toString());
	qDebug()<<userModel->select();
	qDebug()<<userModel->lastError();
	/*userModel->setFilter("fio LIKE '%"+ui->search_name1->text()+"%'");
	userModel->select();*/
}


void FindUserCBox::on_search_fio_button_clicked()
{
	userModel->setFilter("fio LIKE '%"+ui->search_name1->text()+"%'");
    userModel->select();
}

void FindUserCBox::on_search_two_column_clicked()
{
	userModel->setFilter("fio LIKE '%"+ui->search_name_2->text()+"%'"\
						+"AND registration LIKE '%"\
						+ui->search_adress->text()+"%'");
    userModel->select();
}

void FindUserCBox::on_search_name_3_textEdited(const QString &arg1)
{
	emit on_FindFirstButton_clicked();
}

void FindUserCBox::on_FindFirstButton_clicked()
{
	QString searchColumn = ui->typeSearchCBox->currentData().toString();
	QSqlRecord rec;
	for(int nRow =0; nRow<userModel->rowCount();nRow++)
	{
		rec = userModel->record(nRow);
		if(rec.value(searchColumn).toString().contains(\
			ui->search_name_3->text(), Qt::CaseInsensitive))
		{
			emit sendCurrentIndex(userModel->index(nRow,rec.indexOf(searchColumn)));
			break;
		}
	}
}

void FindUserCBox::on_FindPreviousButton_clicked()
{
	emit getCurrentIndex();
	QString searchColumn = ui->typeSearchCBox->currentData().toString();
	QSqlRecord rec;
	for(int nRow =currentIndex.row()-1;nRow>=0;nRow--)
	{
		rec = userModel->record(nRow);
		if(rec.value(searchColumn).toString().contains(\
			ui->search_name_3->text(), Qt::CaseInsensitive))
		{
			emit sendCurrentIndex(userModel->index(nRow,rec.indexOf(searchColumn)));
			return;
		}
	}
	emit on_FindLastButton_clicked();
}

void FindUserCBox::on_FindNextButton_clicked()
{
	emit getCurrentIndex();
	QString searchColumn = ui->typeSearchCBox->currentData().toString();
	QSqlRecord rec;
	for(int nRow =currentIndex.row()+1; nRow<userModel->rowCount();nRow++)
	{
		rec = userModel->record(nRow);
		if(rec.value(searchColumn).toString().contains(\
			ui->search_name_3->text(), Qt::CaseInsensitive))
		{
			emit sendCurrentIndex(userModel->index(nRow,rec.indexOf(searchColumn)));
			return;
		}
	}
	emit on_FindFirstButton_clicked();
}

void FindUserCBox::on_FindLastButton_clicked()
{
	QString searchColumn = ui->typeSearchCBox->currentData().toString();
	QSqlRecord rec;
	for(int nRow =0; nRow<userModel->rowCount();nRow++)
	{
		rec = userModel->record(nRow);
		if(rec.value(searchColumn).toString().contains(\
			ui->search_name_3->text(), Qt::CaseInsensitive))
		{
			emit sendCurrentIndex(userModel->index(nRow,rec.indexOf(searchColumn)));
		}
	}
}

void FindUserCBox::on_group_button_clicked()
{

    userModel->setTable("groupusertypes");
    userModel->select();
}

void FindUserCBox::on_group_concat_button_clicked()
{

    userModel->setTable("groupconcat_usertypes");
    userModel->select();
}

void FindUserCBox::on_search_name_3_textChanged(const QString &arg1)
{

}

void FindUserCBox::on_search_name_3_editingFinished()
{

}
