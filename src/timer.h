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





#ifndef TIMER_H
#define TIMER_H

#include <QObject>
#include <QString>
#include <QTimer>


#include <QModelIndex>

#include "alertsound.h"

/*! The timer class of KitchenAlert'

  @author Heli Hyvättinen
  @date 2011-02-10
  @version 0.2.0

The timer class of KitchenAlert.

*/

class Timer : public QObject
{
    Q_OBJECT
public:
    explicit Timer(QObject *parent = 0);

    /*! Returns the original time in seconds
        When the alert restarts, it is set to this time.
    */
     int getOriginalTimeInSeconds();

    /*! Sets the original time in seconds */
    void setOriginalTimeInSeconds(int seconds);

    /*! Returns the time remaining before the alert in seconds       */
    int getRemainingTimeInSeconds();

    /*! Returns the alert text */
    QString getAlertText();

    /*! Sets the alert text */
    void setAlertText(QString text);

    /*! Returns whether the timer is alerting */
    bool isAlerting();


signals:
    /*! Emitted when the remaining time in the timer has changed */
    void remainingTimeChanged();

    /*! Emitted when the timer alerts */
    void alert(QModelIndex indexOfAlerter);

public slots:
    /*! The internal QTimer is connected to this slot */
    void secondPassed();

    /*! Start the timer */
    void start();

    /*! Stop the timer*/
    void stop();

    /*! Snooze the timer
        Currently sets the timer to alert again in two minutes.
    */
    void snooze();

    /*! Switches to use the default sound (and saves it to settings */
    void enableDefaultSound();

    /*! Changes the sound file used and saves it to settings */
    void changeAlertSound(QString filename);




private:
    int _originalTime; //seconds!
    int _remainingTime; //seconds!
    QString _alertText;
    QTimer _actualTimer;
    bool alerting_;

    QModelIndex whereAmI();

    AlertSound alertSound_;

};

#endif // TIMER_H
