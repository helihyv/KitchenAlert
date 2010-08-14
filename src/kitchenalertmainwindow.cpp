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



KitchenAlertMainWindow::KitchenAlertMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::KitchenAlertMainWindow)
    {
    ui->setupUi(this);


  //alerts' tableview setup


  ui->ComingAlertsTableView->setModel(&model_);
  ui->ComingAlertsTableView->setSelectionMode(QAbstractItemView::SingleSelection);
  ui->ComingAlertsTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
  ui->ComingAlertsTableView->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
  ui->ComingAlertsTableView->horizontalHeader()->resizeSection(0,460);
  ui->ComingAlertsTableView->horizontalHeader()->resizeSection(1,140);
  ui->ComingAlertsTableView->horizontalHeader()->resizeSection(2,100);

  ui->ComingAlertsTableView->verticalHeader()->setResizeMode(QHeaderView::Fixed);
  ui->ComingAlertsTableView->verticalHeader()->setDefaultSectionSize(40);




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
    QMessageBox::about(this,tr("About KitchenAlert"),tr("<p>Version 0.1.1"
                                                        "<p>Copyright &copy; Heli Hyv&auml;ttinen 2010"
                                                         "<p>License: General Public License v3"
                                                         "<p>Bugtracker and project page: https://garage.maemo.org/projects/kitchenalert/"));
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
