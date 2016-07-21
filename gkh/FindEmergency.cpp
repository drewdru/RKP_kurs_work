#include "FindEmergency.h"
#include "ui_FindEmergency.h"

FindEmergency::FindEmergency(QWidget *parent) :
QWidget(parent),
ui(new Ui::FindEmergency)
{
	ui->setupUi(this);
}

FindEmergency::~FindEmergency()
{
	delete ui;
}
