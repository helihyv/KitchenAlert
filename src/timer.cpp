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
#include <QFile>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

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
        qDebug() << "alerted";
    }

    emit remainingTimeChanged(); //after alerting in case of alert so that status gets updated immediately
}

void Timer::start()
{
    _remainingTime = _originalTime;
    _actualTimer.start();

    alerting_ = false;

    if (_originalTime == 0) //has to be checked here, since 00:00:00 alert will already be negative when checked next time
    //THIS ALERTS EVERY SECOND TIME THE TIMER IS STARTED! //This bug disappeared without explanation...
    {
        alerting_ = true;
        emit alert(whereAmI());
        qDebug () << "Alerted for 00:00:00 alert";
    }
}


void Timer::stop()
{
    _actualTimer.stop();
    _remainingTime = 0; //Stopped timer shows 00:00:00

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

bool Timer::save(QString filename)
{
    QFile file(filename);

    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
       return false;
    }

    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("kitchenalert");
    xmlWriter.writeStartElement("timer");
    xmlWriter.writeAttribute("alert_text",_alertText);
    xmlWriter.writeAttribute("time_in_seconds", QString().setNum(_originalTime));
    xmlWriter.writeEndDocument(); //this should close all open elements

    return true;
}

bool Timer::load(QString filename)
{
    QFile file (filename);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        return false;
    }

    QXmlStreamReader reader;
    reader.setDevice(&file);

    reader.readNextStartElement();

    if (reader.name() != "kitchenalert")
        return false;

    reader.readNextStartElement();
    if (reader.name() != "timer")
        return false;


    _alertText = reader.attributes().value("alert_text").toString();
    _originalTime = reader.attributes().value("time_in_seconds").toString().toInt();
    return true;




}

