#include "FindEmergencyCBox.h"
#include "ui_FindEmergencyCBox.h"

FindEmergencyCBox::FindEmergencyCBox(QWidget *parent) :
QWidget(parent),
ui(new Ui::FindEmergencyCBox)
{
	ui->setupUi(this);
	ui->StartDate->setDate(QDate::currentDate());
	ui->StartDate->setTime(QTime::fromString("00:00"));
	ui->EndDate->setDateTime(QDateTime::currentDateTime());
	ui->StartDate->setDisplayFormat("dd.MM.yyyy hh:mm");
	ui->EndDate->setDisplayFormat("dd.MM.yyyy hh:mm");
}

FindEmergencyCBox::~FindEmergencyCBox()
{
	delete ui;
}

void FindEmergencyCBox::SetModels(QSqlRelationalTableModel* UserModel)
{
	EmergencyModel = UserModel;
}

void FindEmergencyCBox::recieveData(const QModelIndex &index)
{
	currentIndex = index;
}

void FindEmergencyCBox::recieveState(const QString &str)
{
	this->state = str;
}

void FindEmergencyCBox::on_FindFirstButton_clicked()
{
	QString searchColumn = "date";
	QSqlRecord rec;
	for(int nRow =0; nRow<EmergencyModel->rowCount();nRow++)
	{
		rec = EmergencyModel->record(nRow);
		if(rec.value(searchColumn).toDate()
			== ui->FindDate->date())
		{
			emit sendCurrentIndex(EmergencyModel->index(nRow,4));
			break;
		}
	}
}

void FindEmergencyCBox::on_FindPreviousButton_clicked()
{
	emit getCurrentIndex();
	QString searchColumn = "date";//ui->typeSearchCBox->currentData().toString();
	QSqlRecord rec;
	for(int nRow =currentIndex.row()-1;nRow>=0;nRow--)
	{
		rec = EmergencyModel->record(nRow);
		if(rec.value(searchColumn).toDate()
			== ui->FindDate->date())
		{
			emit sendCurrentIndex(EmergencyModel->index(nRow,4));
			return;
		}
	}
	emit on_FindLastButton_clicked();
}

void FindEmergencyCBox::on_FindNextButton_clicked()
{
	emit getCurrentIndex();
	QString searchColumn = "date";//ui->typeSearchCBox->currentData().toString();
	QSqlRecord rec;
	for(int nRow =currentIndex.row()+1; nRow<EmergencyModel->rowCount();nRow++)
	{
		rec = EmergencyModel->record(nRow);
		if(rec.value(searchColumn).toDate()
			== ui->FindDate->date())
		{
			emit sendCurrentIndex(EmergencyModel->index(nRow,4));
			return;
		}
	}
	emit on_FindFirstButton_clicked();
}

void FindEmergencyCBox::on_FindLastButton_clicked()
{
	QString searchColumn = "date";//ui->typeSearchCBox->currentData().toString();
	QSqlRecord rec;
	for(int nRow =0; nRow<EmergencyModel->rowCount();nRow++)
	{
		rec = EmergencyModel->record(nRow);
		if(rec.value(searchColumn).toDate()
			== ui->FindDate->date())
		{
			emit sendCurrentIndex(EmergencyModel->index(nRow,4));
		}
	}
}

void FindEmergencyCBox::on_pushButton_clicked()
{
	QString str ="date BETWEEN STR_TO_DATE('%1','%d.%m.%Y %H:%i')"\
	" AND STR_TO_DATE('%2', '%d.%m.%Y %H:%i') AND %3";
	str = str.arg(ui->StartDate->text(),ui->EndDate->text(),state);
	EmergencyModel->setFilter(str);
	EmergencyModel->select();
}

void FindEmergencyCBox::on_pushButton_2_clicked()
{
	EmergencyModel->setFilter(state);
	EmergencyModel->select();
}
