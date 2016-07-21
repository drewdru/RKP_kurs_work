#ifndef FINDEMERGENCY_H
#define FINDEMERGENCY_H

#include <QWidget>

namespace Ui {
	class FindEmergency;
}

class FindEmergency : public QWidget
{
	Q_OBJECT

public:
	explicit FindEmergency(QWidget *parent = 0);
	~FindEmergency();

private:
	Ui::FindEmergency *ui;
};

#endif // FINDEMERGENCY_H
