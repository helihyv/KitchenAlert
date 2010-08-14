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



/*! The timer class of KitchenAlert'

  @author Heli Hyvättinen
  @date 2010-08-12
  @version 0.1.1

The timer class of KitchenAlert.

*/

class Timer : public QObject
{
    Q_OBJECT
public:
    explicit Timer(QObject *parent = 0);

    int getOriginalTimeInSeconds();
    void setOriginalTimeInSeconds(int seconds);

    int getRemainingTimeInSeconds();

    QString getAlertText();
    void setAlertText(QString text);

    bool isAlerting();


signals:

    void remainingTimeChanged();
    void alert(QModelIndex indexOfAlerter);

public slots:
    void secondPassed();
    void start();
    void stop();
    void snooze();



private:
    int _originalTime; //seconds!
    int _remainingTime; //seconds!
    QString _alertText;
    QTimer _actualTimer;
    bool alerting_;

    QModelIndex whereAmI();

};

#endif // TIMER_H
