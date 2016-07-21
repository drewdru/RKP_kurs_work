#ifndef FINDACCOUNDCBOX_H
#define FINDACCOUNDCBOX_H

#include <QWidget>
#include <QtSQL>
namespace Ui {
	class FindAccoundCBox;
}

class FindAccoundCBox : public QWidget
{
	Q_OBJECT

	QSqlRelationalTableModel* accountModel;
	QModelIndex currentIndex;
public:
	explicit FindAccoundCBox(QWidget *parent = 0);
	~FindAccoundCBox();
	void SetModels(QSqlRelationalTableModel* AccountModel);
private:
	Ui::FindAccoundCBox *ui;

public slots:
	void recieveData(const QModelIndex &index);
signals:
	void getCurrentIndex();
	void sendCurrentIndex(const QModelIndex &index);
private slots:
	void on_FindFirstButton_clicked();
	void on_FindPreviousButton_clicked();
	void on_FindNextButton_clicked();
	void on_FindLastButton_clicked();
	void on_search_two_column_clicked();
	void on_reset_filter_clicked();
	void on_search_id_textEdited(const QString &arg1);
	void on_search_name_3_textEdited(const QString &arg1);
    void on_pushButton_clicked();
};

#endif // FINDACCOUNDCBOX_H
