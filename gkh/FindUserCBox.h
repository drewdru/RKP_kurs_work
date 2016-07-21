#ifndef FINDUSERCBOX_H
#define FINDUSERCBOX_H

#include <QtWidgets>
#include <QtSQL>
namespace Ui {
	class FindUserCBox;
}

class FindUserCBox : public QWidget
{
	Q_OBJECT
	QSqlRelationalTableModel* userModel;
	QSqlTableModel* userTypeModel;
	QModelIndex currentIndex;
	bool flag = false;
public:
	explicit FindUserCBox(QWidget *parent = 0);
	~FindUserCBox();
	void SetModels(QSqlRelationalTableModel* UserModel, \
					QSqlTableModel *UserTypeModel, bool flag=false);
private slots:

	void on_reset_filter_clicked();

	void on_master_clicked(const QModelIndex &index);

	void on_search_fio_button_clicked();

	void on_search_two_column_clicked();

	void on_search_name_3_textEdited(const QString &arg1);

	void on_FindPreviousButton_clicked();

	void on_FindNextButton_clicked();

	void on_FindLastButton_clicked();

    void on_group_button_clicked();

    void on_group_concat_button_clicked();

	void on_search_name_3_textChanged(const QString &arg1);

	void on_search_name_3_editingFinished();

public slots:
	void on_FindFirstButton_clicked();
	void recieveData(const QModelIndex &index);
signals:
	void getCurrentIndex();
	void sendCurrentIndex(const QModelIndex &index);

private:
	Ui::FindUserCBox *ui;
};

#endif // FINDUSERCBOX_H
