/**************************************************************************

        KitchenAlert

        Copyright (C) 2010-2011  Heli Hyvättinen

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
#include <QSettings>
#include <QFileDialog>




KitchenAlertMainWindow::KitchenAlertMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::KitchenAlertMainWindow)
    {
      
  defaultSaveDirectory_ = "/home/user/KitchenAlert";    

  ui->setupUi(this);

  setWindowIcon(QIcon(":/kitchenalert.png"));

  //alerts' tableview setup

  ui->ComingAlertsTableView->setModel(&model_);
  ui->ComingAlertsTableView->setSelectionMode(QAbstractItemView::SingleSelection);
  ui->ComingAlertsTableView->setSelectionBehavior(QAbstractItemView::SelectRows);

  ui->ComingAlertsTableView->horizontalHeader()->hide();
//  ui->ComingAlertsTableView->verticalHeader()->setVisible(true);

  ui->ComingAlertsTableView->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
  ui->ComingAlertsTableView->horizontalHeader()->resizeSection(0,535);
  ui->ComingAlertsTableView->horizontalHeader()->resizeSection(1,140);
  ui->ComingAlertsTableView->horizontalHeader()->resizeSection(2,100);

  ui->ComingAlertsTableView->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);


  //Buttons used to reacting to an alarm are hidden by default

  disableSelectionDependentButtons();


  connect(ui->ComingAlertsTableView->selectionModel(),SIGNAL(selectionChanged(QItemSelection,QItemSelection)),this,SLOT(timerSelected(QItemSelection,QItemSelection)));
  connect(ui->CreateNewScheduleButton, SIGNAL (clicked()), this, SLOT (newTimerSequence()));
  connect(ui->DoneButton,SIGNAL(clicked()),this,SLOT(stop()));
  connect(ui->RestartButton,SIGNAL(clicked()),this,SLOT(restart()));
  connect(ui->SnoozeButton,SIGNAL(clicked()),this, SLOT(snooze()));
  connect(ui->RemoveButton,SIGNAL(clicked()),this,SLOT(remove()));
  connect(ui->SaveButton,SIGNAL(clicked()),this,SLOT(saveTimer()));
  connect(ui->OpenButton,SIGNAL(clicked()),this,SLOT(loadTimer()));

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


       QList<Timer *>  alltimers = createdialog.getTimers();  //get user input from the dialog

       Timer* timer1 = alltimers.at(0); // take first timer (currently the only one!)



       connect(timer1,SIGNAL(alert(QModelIndex)),this,SLOT(alert(QModelIndex)));



       connect(this,SIGNAL(defaultSoundEnabled()),timer1,SLOT(enableDefaultSound()));
       connect(this,SIGNAL(soundChanged(QString)),timer1,SLOT(changeAlertSound(QString)));



        model_.addTimers(alltimers); // give timers to the model, they are started automatically by default

 //       ui->ComingAlertsTableView->resizeColumnsToContents();


        //Disable buttons, as selection is cleared when view is refreshed to show the new timer
        //But only if the timer has not already alerted and thus been selected

        if (!selectedRow().isValid())
            disableSelectionDependentButtons();



    }
    // if cancelled, do nothing



}





void KitchenAlertMainWindow::alert(QModelIndex indexOfAlerter)
{

    // The program is brought to front and activated when alerted


    activateWindow();

// removing everything below does not solve the bug #6752!

    raise();  //this may be unnecessary

    // The alerting timer is selected
    ui->ComingAlertsTableView->selectionModel()->select(QItemSelection(indexOfAlerter,indexOfAlerter),QItemSelectionModel::SelectCurrent | QItemSelectionModel::Rows );

    //Scrolls the view so that the alerting timer is visible
    ui->ComingAlertsTableView->scrollTo(indexOfAlerter);

   // qDebug() << "Should be selected now";


    //Snooze button is enabled


    ui->SnoozeButton->setEnabled(true);
//qDebug ("Snooze on when alerting");

}


void KitchenAlertMainWindow::timerSelected(QItemSelection selected,QItemSelection)
{
    ui->DoneButton->setEnabled(true);
    ui->RestartButton->setEnabled(true);
    ui->RemoveButton->setEnabled(true);
    ui->SaveButton->setEnabled(true);


    //snooze button enabled only when alerting
    QModelIndexList indexes = selected.indexes();

    //the selection model only allows selecting one row at the time & we only need to know the row, so we can just take the first one
    QModelIndex index = indexes.value(0);
    if (index.isValid())
    {
        if (model_.isThisTimerAlerting(index) == true)
        {
             ui->SnoozeButton->setEnabled(true);
//qDebug() << "Snooze on";
        }
        else
        {
            ui->SnoozeButton->setDisabled(true);
//qDebug() << "Snooze off";
        }
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


}

void KitchenAlertMainWindow::restart()
{
    QModelIndex row = selectedRow(); //If there was no row selected invalid row was returned
    if (row.isValid())
    {

        model_.startTimer(row);
    }


   if (model_.isThisTimerAlerting(row) == false) //This has to be checked, because 00:00:00 alerts alert *before* the program execution reaches here
    {
        ui->SnoozeButton->setDisabled(true);
    }
 //   qDebug () << "disabled snooze because of restart";


}

void KitchenAlertMainWindow::stop()
{
    QModelIndex row = selectedRow();
    if (row.isValid()) //If there was no row selected invalid row was returned
    {
        model_.stopTimer(row);
    }
    ui->SnoozeButton->setDisabled(true);

}

QModelIndex KitchenAlertMainWindow::selectedRow()
{
    //Returns the cells in row 0 that have the whole row selected (the selection mode used allows only selecting whole rows

    QModelIndexList chosenRows = ui->ComingAlertsTableView->selectionModel()->selectedRows();

    //The selection mode used allows only one row to be selected at time, so we just take the first


    return chosenRows.value(0); //gives an invalid QModelIndex if the list is empty
}

void KitchenAlertMainWindow::openSelectSoundDialog()
{

    SelectSoundDialog dialog;
   if ( dialog.exec() == QDialog::Accepted) //if user pressed OK
    {
       QSettings settings ("KitchenAlert","KitchenAlert");
      
       if (dialog.isDefaultSoundChecked() == true)
       { 
	 
	   settings.setValue("UseDefaultSound",true); 
           emit defaultSoundEnabled();
       }   
      else
       {
           QString filename = dialog.getSoundFileName();
           settings.setValue("UseDefaultSound",false);
           settings.setValue("soundfile",filename);
           emit soundChanged(filename);
       }

    }

}

void KitchenAlertMainWindow::openAbout()
{
    QMessageBox::about(this,tr("About KitchenAlert"),tr("<p>Version %1"
                                                        "<p>Copyright &copy; Heli Hyv&auml;ttinen 2010-2011"
                                                         "<p>License: General Public License v3"
                                                         "<p>Web page: http://kitchenalert.garage.maemo.org/"
                                                         "<p>Bugtracker: https://garage.maemo.org/projects/kitchenalert/").arg(QApplication::applicationVersion()));
}

bool KitchenAlertMainWindow::event(QEvent *event)
{


    switch (event->type())
    {
        case QEvent::WindowActivate:

            model_.setUpdateViewOnChanges(true);
//            ui->debugLabel->setText("Returned to the application!");
            break;

       case QEvent::WindowDeactivate:
            model_.setUpdateViewOnChanges(false);
//            ui->debugLabel->setText("");
            break;

       default:
            break;

    }

    return QMainWindow::event(event); // Send the event to the base class implementation (also when handling the event in this function): necessary for the program to work!
}

void KitchenAlertMainWindow::disableSelectionDependentButtons()
{
    ui->DoneButton->setDisabled(true);
    ui->SnoozeButton->setDisabled(true);
    ui->RestartButton->setDisabled(true);
    ui->RemoveButton->setDisabled(true);
    ui->SaveButton->setDisabled(true);


}

void KitchenAlertMainWindow::initializeAlertSound()
{
    QSettings settings;

    bool useDefaultSound = settings.value("UseDefaultSound",true).toBool();
    QString filename = settings.value("soundfile","").toString();

    if (useDefaultSound == true)
    {
        openSelectSoundDialog();
    }
    else if (filename.isEmpty())
    {
        openSelectSoundDialog();
    }

   QString currentFilename = settings.value("soundfile","").toString();

   if (currentFilename.isEmpty())
   {
        ui->debugLabel->setText("<FONT color=red>No alert sound file set. Alert sound will not be played!</FONT>");

   }

}

void KitchenAlertMainWindow::remove()
{
    QModelIndex row = selectedRow();
    if (row.isValid()) //If there was no row selected invalid row was returned
    {
        QString text = tr("Are you sure you want to remove this timer from the list:\n");
        text.append((row.data().toString()));
        if (QMessageBox::question(this,tr("Confirm timer removal"),text,QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
        {
            model_.removeTimer(row);
            ui->ComingAlertsTableView->clearSelection();
            disableSelectionDependentButtons();

        }
    }

}

void KitchenAlertMainWindow::saveTimer()
{

    QModelIndex row = selectedRow();

    if (row.isValid() == false) //If there was no row selected invalid row was returned
        return;


    //file name is asked. As the filename will be appended, there's no point in confirming owerwrite here
    QString filename = QFileDialog::getSaveFileName(this, "", defaultSaveDirectory_, "*.kitchenalert",NULL,QFileDialog::DontConfirmOverwrite);



//    qDebug() << filename;

    if (filename.isEmpty()) //user cancelled the dialog (or gave an empty name)
    {
        return;
    }

    if (!filename.endsWith(".kitchenalert"))
    {
        filename.append(".kitchenalert");

    }

  //  qDebug() << "filename appended to " << filename;


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

// If the default save directory does not exist use /home/user instead (as that's what the save dialog will use)
// This avoids a situation where save directs to a folder that cannot be accessed with open...

    QString startDirectory;

    if (QFile(defaultSaveDirectory_).exists())
    {
        startDirectory = defaultSaveDirectory_;
    }
    else
    {
        startDirectory = "/home/user/";
        qDebug () << "default save directory not found";
    }


 //Get the filename to open with a dialog

    QString filename = QFileDialog::getOpenFileName(this,"",startDirectory,tr("KitchenAlert timer files (*.kitchenalert)"));
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


//connect change sound functions

connect(this,SIGNAL(defaultSoundEnabled()),p_timer,SLOT(enableDefaultSound()));
connect(this,SIGNAL(soundChanged(QString)),p_timer,SLOT(changeAlertSound(QString)));


//Disable buttons, as selection is cleared when view is refreshed to show the new timer

disableSelectionDependentButtons();


// give timers to the model (model wants list of timers now..)

QList<Timer *> timerList;

timerList.append(p_timer);
model_.addTimers(timerList,true); //timer gets started in the model

}



