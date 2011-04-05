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





#ifndef CURRENTALERTSTABLEMODEL_H
#define CURRENTALERTSTABLEMODEL_H

#include <QAbstractTableModel>
#include <QMainWindow>
#include "timer.h"


/*! Class that contains the model that holds the timers'

  @author Heli Hyvättinen
  @date 2011-03-29
  @version 0.2.1

Class that contains the model that holds the timers

*/


class CurrentAlertsTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit CurrentAlertsTableModel(QObject *parent = 0);

    /*!
    Returns the number of rows in the model.
    Used by the view.
    */
    int rowCount(const QModelIndex &parent) const;

    /*!
      Returns the (fixed) number of columns in the model.
      Used by the view.
    */
    int columnCount(const QModelIndex &parent) const;

    /*!
    Returns the queried contents of the cell
    Used by the view.
    */
    QVariant data(const QModelIndex &index, int role) const;

    /*!
      Returns the queried header data.
      As no headers are wanted, it always returns an empty QVariant.
      Used by the view.
      */
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    /*!
      Returns the index in the model of the given timer
      */
    QModelIndex giveIndexForTimer(Timer * ptimer);

    /*!
      Returns whether the timer in the given position is currently alerting.
      @param index Any cell from the row of the alert is good here.
      */
    bool isThisTimerAlerting(QModelIndex index);


signals:

public slots:

    /*!
       Adds the timers to the model
    @param timers List of timers to be added
    @param startImmediately If true, the timers will be started after adding to the model.
    */
    void addTimers(QList <Timer *> timers, bool startImmediately = true);

    /*!
     Tells the view to refresh all information in the time and status columns.
     (The time and status change without user inervention unlike the alert text and thus
     need to be refreshed.)
      */
    void refreshTimeAndStatusColumns ();

    /*!
      Passes the start command to the timer in the given index.
      @param index Any cell from the row of the alert is good here.
      */
    void startTimer(QModelIndex index);

    /*!
      Passes the snooze command to the timer in the given index.
      @param index Any cell from the row of the alert is good here.
      */
    void snoozeTimer(QModelIndex index);


    /*!
      Passes the stop command to the timer in the given index.
      @param index Any cell from the row of the alert is good here.
      */
    void stopTimer(QModelIndex index);

/*!
Sets whether the view should be kept up to date.
*/
    void setUpdateViewOnChanges(bool update);

/*!
  Removes a timer from the model
  */

    void removeTimer(QModelIndex index);



private:
    QList <Timer * > currentTimers_; /*! Holds the timers */

    static const int numberOfColumns_ = 3; /*! The fixed number of columns in the model */
    static const int alertTextColumnNumber_ = 0; /*! Tells which column contains the alert text */
    static const int timeRemainingColumnNumber_ = 1; /*! Tells which column contains the time remaining until alert */
    static const int statusColumnNumber_ = 2; /*! Tells which column contains status information, such as that the timer is alerting  */



    bool updateViewOnChanges_; /*! Keeps track on whether the view should be kept up to date. */

};

#endif // CURRENTALERTSTABLEMODEL_H
