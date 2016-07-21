#include "FindAccoundCBox.h"
#include "ui_FindAccoundCBox.h"


FindAccoundCBox::FindAccoundCBox(QWidget *parent) :
QWidget(parent),
ui(new Ui::FindAccoundCBox)
{
	ui->setupUi(this);

	ui->typeSearchCBox->addItem("ФИО","fio");
	ui->typeSearchCBox->addItem("Ид","id");
	ui->typeSearchCBox->addItem("Адрес","adress");

    ui->FirstField->addItem("ФИО", "fio");
    ui->FirstField->addItem("Средства", "money");
    ui->FirstField->addItem("Адрес", "address");
    ui->SecondField->addItem("Нет", "");
    ui->SecondField->addItem("ФИО", "fio");
    ui->SecondField->addItem("Средства", "money");
    ui->SecondField->addItem("Адрес", "address");
    ui->FirstSortType->addItem("Возраст.", "asc");
    ui->SecondSortType->addItem("Возраст.", "asc");
    ui->FirstSortType->addItem("Убыв.", "desc");
    ui->SecondSortType->addItem("Убыв.", "desc");
}

FindAccoundCBox::~FindAccoundCBox()
{
	delete ui;
}

void FindAccoundCBox::SetModels(QSqlRelationalTableModel* UserModel)
{
	accountModel = UserModel;
}

void FindAccoundCBox::recieveData(const QModelIndex &index)
{
	currentIndex = index;
}

void FindAccoundCBox::on_FindFirstButton_clicked()
{
	QString searchColumn = ui->typeSearchCBox->currentData().toString();
	QSqlRecord rec;
	for(int nRow =0; nRow<accountModel->rowCount();nRow++)
	{
		rec = accountModel->record(nRow);
		if(rec.value(searchColumn).toString().contains(\
			ui->search_name_3->text(), Qt::CaseInsensitive))
		{
			emit sendCurrentIndex(accountModel->index(nRow,4));
			break;
		}
	}
}

void FindAccoundCBox::on_FindPreviousButton_clicked()
{
	emit getCurrentIndex();
	QString searchColumn = ui->typeSearchCBox->currentData().toString();
	QSqlRecord rec;
	for(int nRow =currentIndex.row()-1;nRow>=0;nRow--)
	{
		rec = accountModel->record(nRow);
		if(rec.value(searchColumn).toString().contains(\
			ui->search_name_3->text(), Qt::CaseInsensitive))
		{
			emit sendCurrentIndex(accountModel->index(nRow,4));
			return;
		}
	}
	emit on_FindLastButton_clicked();
}

void FindAccoundCBox::on_FindNextButton_clicked()
{
	emit getCurrentIndex();
	QString searchColumn = ui->typeSearchCBox->currentData().toString();
	QSqlRecord rec;
	for(int nRow =currentIndex.row()+1; nRow<accountModel->rowCount();nRow++)
	{
        rec = accountModel->record(nRow);
		if(rec.value(searchColumn).toString().contains(\
			ui->search_name_3->text(), Qt::CaseInsensitive))
		{
			emit sendCurrentIndex(accountModel->index(nRow,4));
			return;
		}
	}
	emit on_FindFirstButton_clicked();
}

void FindAccoundCBox::on_FindLastButton_clicked()
{
	QString searchColumn = ui->typeSearchCBox->currentData().toString();
	QSqlRecord rec;
	for(int nRow =0; nRow<accountModel->rowCount();nRow++)
	{
		rec = accountModel->record(nRow);
		if(rec.value(searchColumn).toString().contains(\
			ui->search_name_3->text(), Qt::CaseInsensitive))
		{
			emit sendCurrentIndex(accountModel->index(nRow,4));
		}
	}
}

void FindAccoundCBox::on_search_two_column_clicked()
{
	accountModel->setFilter("fio LIKE '%"+ui->search_name_2->text()+"%'"\
                        +"AND address LIKE '%"\
						+ui->search_adress->text()+"%'"\
						+"AND flat_number LIKE '%"\
						+ui->lineEdit->text()+"%'");
    accountModel->select();
}

void FindAccoundCBox::on_reset_filter_clicked()
{
	accountModel->setFilter("");
    accountModel->select();
}

void FindAccoundCBox::on_search_id_textEdited(const QString &arg1)
{
	accountModel->setFilter("id LIKE '%"+ui->search_id->text()+"%'");
}

void FindAccoundCBox::on_search_name_3_textEdited(const QString &arg1)
{
	emit on_FindFirstButton_clicked();
}

void FindAccoundCBox::on_pushButton_clicked()
{
    QString firstField = ui->FirstField->currentData().toString();
    QString firstType = ui->FirstSortType->currentData().toString();
    QString secondField = ui->SecondField->currentData().toString();
    QString secondType = ui->SecondSortType->currentData().toString();
    if (secondField == "")
    {
        accountModel->setFilter("id>0 ORDER BY "+firstField+" "+firstType);
    }
    else
    {
        accountModel->setFilter("id>0 ORDER BY "+firstField+" "+firstType+", "+ secondField+" "+secondType);

    }

    //qDebug()<<accountModel->query().lastQuery();
    accountModel->select();
}
