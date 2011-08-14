#ifndef STICKYDIALOG_H
#define STICKYDIALOG_H

#include <QDialog>
#include <QListView>
#include <QStringListModel>

class StickyDialog : public QDialog
{
    Q_OBJECT
public:
    explicit StickyDialog(QString defaultDirectory,  QWidget *parent = 0);

    QStringList getStickyList();


signals:

public slots:

    void add();
    void remove();

protected:

    QListView * pStickiesView_;
    QStringListModel * pStickiesModel_;

    QString defaultDirectory_;



};

#endif // STICKYDIALOG_H
