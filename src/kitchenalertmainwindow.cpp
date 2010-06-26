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

#include "choosetimersequencedialog.h"
#include "createtimersequencedialog.h"



#include <QDebug>
#include <QMessageBox>



KitchenAlertMainWindow::KitchenAlertMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::KitchenAlertMainWindow)
    {
    ui->setupUi(this);

  connect(ui->LoadTimerScheduleButton, SIGNAL (pressed()), this, SLOT (openTimerSequence()));
  connect(ui->CreateNewScheduleButton, SIGNAL (pressed()), this, SLOT (newTimerSequence()));

  ui->ComingAlertsTableView->setModel(&model_);
  ui->ComingAlertsTableView->setSelectionMode(QAbstractItemView::SingleSelection);
  ui->ComingAlertsTableView->setSelectionBehavior(QAbstractItemView::SelectRows);


  //Buttons used to reacting an alarm are hidden by default

  ui->DoneButton->setDisabled(true);
  ui->SnoozeButton->setDisabled(true);
  ui->RestartButton->setDisabled(true);


  connect(ui->ComingAlertsTableView->selectionModel(),SIGNAL(selectionChanged(QItemSelection,QItemSelection)),this,SLOT(timerSelected(QItemSelection,QItemSelection)));

  connect(ui->DoneButton,SIGNAL(clicked()),this,SLOT(stop()));
  connect(ui->RestartButton,SIGNAL(clicked()),this,SLOT(restart()));
  connect(ui->SnoozeButton,SIGNAL(clicked()),this, SLOT(snooze()));


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

void KitchenAlertMainWindow::openTimerSequence()
{
    ChooseTimerSequenceDialog opendialog;
    opendialog.exec();



}

void KitchenAlertMainWindow::newTimerSequence()
{
    CreateTimerSequenceDialog createdialog;


    if (createdialog.exec() == QDialog::Accepted) //if user pressed OK
    {


       QList<Timer *>  alltimers = createdialog.getTimers();  //get user input from the dialog

       Timer* timer1 = alltimers.at(0); // take first timer (currently the only one!)




//       ui->nextAlertText->setText( timer1->getAlertText());


       timer1->start();
       connect(timer1,SIGNAL(alert(QModelIndex)),this,SLOT(alert(QModelIndex)));



        model_.addTimers(alltimers); // give timers to the model
//        ui->ComingAlertsTableView->update();



    }
    // if cancelled, do nothing



}


void KitchenAlertMainWindow::updateTime(int seconds)
{


//    ui->hoursLcdNumber->display(seconds/360);
//    ui->minutesLcdNumber->display((seconds%360)/60);
//    ui->secondsLcdNumber->display((seconds%60));



}

void KitchenAlertMainWindow::alert(QModelIndex indexOfAlerter)
{
 //   QMessageBox::information ( NULL, "KitchenAlert","Alert!!!");

    // The program is brought to front and activated when alerted

    raise();
    activateWindow();

    // The alerting timer is selected
    ui->ComingAlertsTableView->selectionModel()->select(QItemSelection(indexOfAlerter,indexOfAlerter),QItemSelectionModel::SelectCurrent | QItemSelectionModel::Rows );
    qDebug() << "Should be selected now";



    //Snooze button is enabled


    ui->SnoozeButton->setEnabled(true);

    //The alert sound is played




        //TODO

    alertSound_.play();

}


void KitchenAlertMainWindow::timerSelected(QItemSelection,QItemSelection)
{
    ui->DoneButton->setEnabled(true);
    ui->RestartButton->setEnabled(true);

}

void KitchenAlertMainWindow::snooze()
{

    model_.snoozeTimer(selectedRow());
    ui->SnoozeButton->setDisabled(true);
    alertSound_.stop();

}

void KitchenAlertMainWindow::restart()
{

    model_.startTimer(selectedRow());
    ui->SnoozeButton->setDisabled(true);
    alertSound_.stop();

}

void KitchenAlertMainWindow::stop()
{
    model_.stopTimer(selectedRow());
    ui->SnoozeButton->setDisabled(true);
    alertSound_.stop();
}

QModelIndex KitchenAlertMainWindow::selectedRow()
{
    QModelIndexList chosenRows = ui->ComingAlertsTableView->selectionModel()->selectedRows();

    //The selection mode used allows only one row to be selected at time, so we just take the first
    //There are indexes for all coloumns in the row in the list, but as we only use the row, it does not matter which one we take

    return chosenRows.takeFirst();
}
