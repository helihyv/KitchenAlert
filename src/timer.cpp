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





#include "timer.h"
#include "currentalertstablemodel.h"
#include <qdebug.h>

Timer::Timer(QObject *parent) :
    QObject(parent)
{
    _originalTime = 0;

    _actualTimer.setInterval(1000); // 1 s
    connect(&_actualTimer, SIGNAL(timeout()), this, SLOT(secondPassed()));

    alerting_ = false;
}


int Timer::getOriginalTimeInSeconds()
{
    return _originalTime;
}

void Timer::setOriginalTimeInSeconds(int seconds)
{
_originalTime = seconds;
}

int Timer::getRemainingTimeInSeconds()
{
    return _remainingTime;
}

QString Timer::getAlertText()
{
    return _alertText;
}

void Timer::setAlertText(QString text)
{
    _alertText = text;
}

void Timer::secondPassed()
{
    _remainingTime--;


    if (_remainingTime == 0)
    {
        alerting_ = true;
        emit alert(whereAmI());
        qDebug() << "alerting";
    }

    emit remainingTimeChanged(); //after alerting in case of alert so that status gets updated immediately
}

void Timer::start()
{
    _remainingTime = _originalTime;
    _actualTimer.start();

    alerting_ = false;

}


void Timer::stop()
{
    _actualTimer.stop();
    _remainingTime = 0; //Stopped timer shows 00:00:00 (which unfortunately makes it red...)

    alerting_ = false;
}

void Timer::snooze()
{
    _remainingTime = 120;

    alerting_ = false;
}


bool Timer::isAlerting()
{
    return alerting_;
}

QModelIndex Timer::whereAmI()
{


   QObject * p_parent = parent();


   CurrentAlertsTableModel* p_parentModel = qobject_cast<CurrentAlertsTableModel *> (p_parent);

   if (p_parentModel == NULL) //If no parent or parent is not of right class, return an invalid index
        return QModelIndex();


  return p_parentModel->giveIndexForTimer(this);


}

