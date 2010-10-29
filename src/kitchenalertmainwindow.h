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



#ifndef KITCHENALERTMAINWINDOW_H
#define KITCHENALERTMAINWINDOW_H

#include <QMainWindow>
#include "timer.h"
#include "currentalertstablemodel.h"
#include <QItemSelection>
#include "alertsound.h"


namespace Ui {
    class KitchenAlertMainWindow;
}


/*! The main window class of KitchenAlert'

  @author Heli Hyvättinen
  @date 2010-09-27
  @version 0.2.0

Operates the UI.

*/

class KitchenAlertMainWindow : public QMainWindow
{
    Q_OBJECT 




public:
    explicit KitchenAlertMainWindow(QWidget *parent = 0);
    ~KitchenAlertMainWindow();


public slots:
   /*!
   Opens a dialog for creating a new timer
   Connects the new timer's alert and adds it to the model, starting the timer
    */
        void newTimerSequence();

   /*!
   Opens a dialog for choosing the alert sound
   Gives the sound filename to AlertSound
    */
   void openSelectSoundDialog();

    /*!
   Shows the application's about box
   */
    void openAbout();

/*! Sounds the alert sound and selects the alarming timer
  Also bring the application to top and activates it
  Used by connecting it to the timer's alert signal
  @param indexOfAlerted The index of the alerting timer in the model.

  */
    void alert(QModelIndex indexOfAlerted);

/*!
    Reacts to selecting of timer in the view.
    Needs to be connected to the corresponding signal.
    Sets enabling of buttons as necessary.
    Does not receive information of clearing the selection (because no signal is sent).
    Disabling selection dependent buttons must be done manually by everything that clears the selection.
    It cannot be cleared by the user with the selection policy used.

    @param selected Selection that contains all selected cells (all cells of the row selected).
*/
    void timerSelected(QItemSelection selected,QItemSelection);

    /*! Snoozes the timer that is currently selected.
      Needs to be connected to the associated button.
    */
    void snooze();

    /*! Restarts the timer selected.
      Needs to be connected to the associated button.
    */
    void restart();

    /*! Stops the timer selected.
      Needs to be connected to the associated button.
    */
    void stop();

    /*! Removes the timer selected.
      Needs to be connected to the associated button.
       */
    void remove();

signals:

    void defaultSoundEnabled();

    void soundChanged(QString filename);

protected:
    void changeEvent(QEvent *e);

    /*!
    Reimplemented to catch WindowActivate and WindowDeactivate events.
    Stops refreshing the view when the window is deactivated and resumes it when reactivated.
    */
    bool event(QEvent *event);

    /*!
    Disables all buttons that should only be enabled when a timer is selected.
    */
    void disableSelectionDependentButtons();

private:

    Ui::KitchenAlertMainWindow *ui;

    CurrentAlertsTableModel model_; /*! The model that stores the alerts */

    /*!
    Returns a QMoldelIndex pointing to the cell in the column 0 of the row that is currently selected.
    */
    QModelIndex selectedRow();

    AlertSound alertSound_; /*! Takes care of alert sound */ //This has been moved to the timers themselves

    /*!
    Not used. Would allow getting rid of the default sound if used.
    */
    void initializeAlertSound();


};

#endif // KITCHENALERTMAINWINDOW_H
