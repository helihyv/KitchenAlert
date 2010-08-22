/**************************************************************************

        KitchenAlert

        Copyright (C) 2010  Heli Hyvättinen

        This file is part of KitchenAlert.

        Kitchen Alert is free software: you can redistribute it and/or modify
        it under the terms of the GNU General Public License as published by
        the Free Software Foundation, either version 3 of the License, or
        (at your option) any later version.

        This program is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU General Public License for more details.

        You should have received a copy of the GNU General Public License
        along with this program.  If not, see <http://www.gnu.org/licenses/>.

**************************************************************************/





#include "kitchenalertmainwindow.h"
#include "ui_kitchenalertmainwindow.h"

#include <QString>
#include <QList>


#include "createtimersequencedialog.h"
#include "selectsounddialog.h"



#include <QDebug>

#include <QAction>
#include <QMenuBar>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QIcon>



KitchenAlertMainWindow::KitchenAlertMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::KitchenAlertMainWindow)
    {
    ui->setupUi(this);

    setWindowIcon(QIcon(":/icon64.png"));

  //load sticky alerts to the model here...



  //alerts' tableview setup


  ui->ComingAlertsTableView->setModel(&model_);
  ui->ComingAlertsTableView->setSelectionMode(QAbstractItemView::SingleSelection);
  ui->ComingAlertsTableView->setSelectionBehavior(QAbstractItemView::SelectRows);


  //Commented out for testing their potebtial effect for slowdown:

  //  ui->ComingAlertsTableView->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
//  ui->ComingAlertsTableView->horizontalHeader()->resizeSection(0,460);
//  ui->ComingAlertsTableView->horizontalHeader()->resizeSection(1,140);
//  ui->ComingAlertsTableView->horizontalHeader()->resizeSection(2,100);

//  ui->ComingAlertsTableView->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);




//  ui->ComingAlertsTableView->verticalHeader()->setDefaultSectionSize(40); //Needed with fixed cell height only




  ui->ComingAlertsTableView->horizontalHeader()->hide();
  ui->ComingAlertsTableView->setWordWrap(true);


  //Buttons used when a timer is selected are disabled by default and enabled upon selection

  disableSelectionDependentButtons();

  connect(ui->ComingAlertsTableView->selectionModel(),SIGNAL(selectionChanged(QItemSelection,QItemSelection)),this,SLOT(timerSelected(QItemSelection,QItemSelection)));

  //connect buttons to respective functions
  connect(ui->CreateNewScheduleButton, SIGNAL (pressed()), this, SLOT (newTimerSequence()));
  connect(ui->DoneButton,SIGNAL(clicked()),this,SLOT(stop()));
  connect(ui->RestartButton,SIGNAL(clicked()),this,SLOT(restart()));
  connect(ui->SnoozeButton,SIGNAL(clicked()),this, SLOT(snooze()));
  connect(ui->RemoveButton,SIGNAL(clicked()),this,SLOT(remove()));
  connect(ui->SaveButton,SIGNAL(clicked()),this,SLOT(saveTimer()));
  connect(ui->loadButton,SIGNAL(clicked()),this,SLOT(loadTimer()));

  // menu setup

  QAction * p_SelectSoundAction = new QAction(tr("Select alert sound"),this);
  connect(p_SelectSoundAction, SIGNAL(triggered()), this, SLOT(openSelectSoundDialog()));
  menuBar()->addAction(p_SelectSoundAction);

  QAction * p_AboutAction = new QAction(tr("About"),this);
  connect(p_AboutAction,SIGNAL(triggered()),this,SLOT(openAbout()));
  menuBar()->addAction(p_AboutAction);
    }

KitchenAlertMainWindow::~KitchenAlertMainWindow()
{
    delete ui;
}

void KitchenAlertMainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;

    }

}


void KitchenAlertMainWindow::newTimerSequence()
{
    CreateTimerSequenceDialog createdialog;


    if (createdialog.exec() == QDialog::Accepted) //if user pressed OK
    {

        //get user input from the dialog


       QList<Timer *>  alltimers = createdialog.getTimers();

       // take first timer (currently the only one!)


       Timer* timer1 = alltimers.at(0);


       //connect alert


       connect(timer1,SIGNAL(alert(QModelIndex)),this,SLOT(alert(QModelIndex)));


       //Disable buttons, as selection is cleared when view is refreshed to show the new timer

       disableSelectionDependentButtons();


       // give timers to the model

       model_.addTimers(alltimers);


       //start the timer when it's safely in the model (consider moving this to the model's addTimers function)


       timer1->start();


    }
    // if cancelled, do nothing



}





void KitchenAlertMainWindow::alert(QModelIndex indexOfAlerter)
{

    // The program is brought to front and activated when alerted

    raise();
    activateWindow();

    // The alerting timer is selected
    ui->ComingAlertsTableView->selectionModel()->select(QItemSelection(indexOfAlerter,indexOfAlerter),QItemSelectionModel::SelectCurrent | QItemSelectionModel::Rows );
//    qDebug() << "Should be selected now";


    //Snooze button is enabled


    ui->SnoozeButton->setEnabled(true);

    //The alert sound is played (consider moving this operation inside timer, as now stopping one alert will silence all alerting alerts)

    alertSound_.play();

}


void KitchenAlertMainWindow::timerSelected(QItemSelection selected,QItemSelection deselected)
{
    ui->DoneButton->setEnabled(true);
    ui->RestartButton->setEnabled(true);
    ui->RemoveButton->setEnabled(true);
    ui->SaveButton->setEnabled(true);


    //enabled only when alerting
    QModelIndexList indexes = selected.indexes();

    //the selection model only allows selecting one row at the time & we only need to know the row, so we can just take the first one
    QModelIndex index = indexes.value(0);
    if (index.isValid())
    {
        if (model_.isThisTimerAlerting(index) == true)
        {
             ui->SnoozeButton->setEnabled(true);
        }
        else ui->SnoozeButton->setDisabled(true);
    }

}

void KitchenAlertMainWindow::snooze()
{
    QModelIndex row = selectedRow();
    if (row.isValid()) //If there was no row selected invalid row was returned
    {
        model_.snoozeTimer(row);
    }
    ui->SnoozeButton->setDisabled(true);
    alertSound_.stop();

}

void KitchenAlertMainWindow::restart()
{
    QModelIndex row = selectedRow(); //If there was no row selected invalid row was returned
    if (row.isValid())
    {

        model_.startTimer(row);
    }
    ui->SnoozeButton->setDisabled(true);
    alertSound_.stop();

}

void KitchenAlertMainWindow::stop()
{
    QModelIndex row = selectedRow();
    if (row.isValid()) //If there was no row selected invalid row was returned
    {
        model_.stopTimer(row);
    }
    ui->SnoozeButton->setDisabled(true);
    alertSound_.stop();
    qDebug() << "Stopped.";
}

QModelIndex KitchenAlertMainWindow::selectedRow()
{
    QModelIndexList chosenRows = ui->ComingAlertsTableView->selectionModel()->selectedRows();

    //The selection mode used allows only one row to be selected at time, so we just take the first
    //There are indexes for all columns in the row in the list, but as we only use the row, it does not matter which one we take

    return chosenRows.value(0); //gives an invalid QModelIndex if the list is empty
}

void KitchenAlertMainWindow::openSelectSoundDialog()
{
    SelectSoundDialog dialog;
   if ( dialog.exec() == QDialog::Accepted) //if user pressed OK
    {
       if (dialog.isDefaultSoundChecked() == true)
           alertSound_.setDefaultSound();
       else
        alertSound_.setSound(dialog.getSoundFileName());

   //opening a dialog clears the selection so the selection dependen buttons must be disabled
    }
    disableSelectionDependentButtons();
}

void KitchenAlertMainWindow::openAbout()
{
    QMessageBox::about(this,tr("About KitchenAlert"),tr("<p>Version %1"
                                                        "<p>Copyright &copy; Heli Hyv&auml;ttinen 2010"
                                                         "<p>License: General Public License v3"
                                                         "<p>Bugtracker and project page: https://garage.maemo.org/projects/kitchenalert/").arg(QApplication::applicationVersion()));
}

bool KitchenAlertMainWindow::event(QEvent *event)
{
    QMainWindow::event(event);

    switch (event->type())
    {
        case QEvent::WindowActivate:

            model_.setUpdateViewOnChanges(true);
              break;

       case QEvent::WindowDeactivate:
            model_.setUpdateViewOnChanges(false);
            break;

       default:
            break;
    }
}

void KitchenAlertMainWindow::disableSelectionDependentButtons()
{
    ui->DoneButton->setDisabled(true);
    ui->SnoozeButton->setDisabled(true);
    ui->RestartButton->setDisabled(true);
    ui->RemoveButton->setDisabled(true);
    ui->SaveButton->setDisabled(true);
}

void KitchenAlertMainWindow::remove()
{
   QModelIndex row = selectedRow();
   if (row.isValid())
   {
    model_.removeTimer(row);
    alertSound_.stop();
    ui->ComingAlertsTableView->clearSelection();
    disableSelectionDependentButtons();
   }
}

void KitchenAlertMainWindow::saveTimer()
{

    QModelIndex row = selectedRow();

    if (row.isValid() == false) //If there was no row selected invalid row was returned
        return;


    //file name is asked. As the filename will be appended, there's no point in confirming owerwrite here
    QString filename = QFileDialog::getSaveFileName(this, "", "", "*.kitchenalert",NULL,QFileDialog::DontConfirmOverwrite);

    disableSelectionDependentButtons();

    qDebug() << filename;

    if (filename.isEmpty()) //user cancelled the dialog (or gave an empty name)
    {
        return;
    }

    if (!filename.endsWith(".kitchenalert"))
    {
        filename.append(".kitchenalert");

    }

    qDebug() << "filename appended to " << filename;


    //MANUAL CONFIRMATION OF OWERWRITE

    if ( QFile::exists(filename))
    {
         //ASK FOR CONFIRMATION

        QString overwriteQuestion ("File ");
        overwriteQuestion.append(filename);
        overwriteQuestion.append(" already exists. Do you want to overwrite it?");
        if (QMessageBox::question(this,"Confirm overwrite?", overwriteQuestion,QMessageBox::Yes | QMessageBox::No,QMessageBox::No) != QMessageBox::Yes)
        {
            return;
        }
    }





    QString errorMessage(tr("Cannot write to file "));
    errorMessage.append(filename);

    if (!model_.saveTimer(row,filename)) //Save the file, if not successful give an error message
    {
        QMessageBox::critical(this,tr("Save timer failed!"), errorMessage);
    }


}

void KitchenAlertMainWindow::loadTimer()
{
    QString filename = QFileDialog::getOpenFileName(this,"","",tr("KitchenAlert timer files (*.kitchenalert)"));
    if (!filename.isEmpty())
    {

//        if (!filename.endsWith(".kitchenalert"))      //not needed, the dialog won't let the user to select files not ending with ".kitchenalert"
//        {
//            filename.append(".kitchenalert");
//        }

        QString errorTitle(tr("Failed to load file "));
        errorTitle.append(filename);

        Timer * p_timer = new Timer();
        if (!p_timer->load(filename))
        {
            QMessageBox::critical(this,errorTitle,tr("Unable to open file or not a valid KitchenAlert timer file."));
            delete p_timer;
            return;
        }

        initializeTimer(p_timer);
    }
}


void KitchenAlertMainWindow::initializeTimer(Timer *p_timer)
{

//connect alert


connect(p_timer,SIGNAL(alert(QModelIndex)),this,SLOT(alert(QModelIndex)));


//Disable buttons, as selection is cleared when view is refreshed to show the new timer

disableSelectionDependentButtons();


// give timers to the model (model wants list of timers now..)

QList<Timer *> timerList;

timerList.append(p_timer);
model_.addTimers(timerList);


//start the timer when it's safely in the model (consider moving this to the model's addTimers function)


p_timer->start();
}
