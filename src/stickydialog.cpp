#include "stickydialog.h"
#include <QVBoxLayout>
#include <QSettings>
#include <QPushButton>
#include <QFileDialog>
#include <QDebug>

StickyDialog::StickyDialog(QString defaultDirectory, QWidget *parent) :
    QDialog(parent)
{
    defaultDirectory_ = defaultDirectory;


    QVBoxLayout* pMainLayout = new QVBoxLayout(this);

    QSettings settings;
    QStringList stickies = settings.value("sticky").toStringList();

//    stickies.append("Test String");

    pStickiesModel_ = new QStringListModel;
    pStickiesModel_->setStringList(stickies);

    pStickiesView_ = new QListView;
    pStickiesView_->setModel(pStickiesModel_);
    pStickiesView_->setSelectionMode(QAbstractItemView::SingleSelection);
    pMainLayout->addWidget(pStickiesView_);

    QHBoxLayout * pButtonLayout = new QHBoxLayout;
    pMainLayout->addLayout(pButtonLayout);

    QPushButton *  pAddButton = new QPushButton (tr("Add"));
    connect(pAddButton,SIGNAL(clicked()),this,SLOT(add()));
    pButtonLayout->addWidget(pAddButton);

    QPushButton * pRemoveButton = new QPushButton(tr("Remove"));
    connect(pRemoveButton,SIGNAL(clicked()),this,SLOT(remove()));
    pButtonLayout->addWidget(pRemoveButton);

    QPushButton * pOkButton = new QPushButton(tr("OK"));
    connect (pOkButton,SIGNAL(clicked()),this,SLOT(accept()));
    pButtonLayout->addWidget(pOkButton);
}

void StickyDialog::add()
{
    QString startDirectory;

    if (QFile(defaultDirectory_).exists())
    {
        startDirectory = defaultDirectory_;
    }
    else
    {
        startDirectory = "/home/user/";
        qDebug () << "default save directory not found";
    }


    QString filename = QFileDialog::getOpenFileName(this,tr("KitchenAlert - Choose a timer to add to stickied"),startDirectory,tr("KitchenAlert timer files (*.kitchenalert)"));

    if (filename.isEmpty()) // user cancelled the dialog
        return;


    QStringList tempList = pStickiesModel_->stringList();
    tempList.append(filename);
    pStickiesModel_->setStringList(tempList);

}

void StickyDialog::remove()
{
    QItemSelectionModel* pSelected = pStickiesView_->selectionModel();
    QModelIndex index = pSelected->currentIndex(); //Only single selection allowed, so we only need to care about current item


    if (!index.isValid())
        return;         //Nothing selected, nothing to remove...

    pStickiesModel_->removeRows(index.row(),1);

}

QStringList StickyDialog::getStickyList()
{
    return pStickiesModel_->stringList();
}
