/**************************************************************************
        KitchenAlert v.0.019

        Copyright (C) 2010  Heli Hyvättinen

        This program is free software: you can redistribute it and/or modify
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





#include "currentalertstablemodel.h"


#include <QBrush>


#include <qdebug.h>

CurrentAlertsTableModel::CurrentAlertsTableModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}


int CurrentAlertsTableModel::rowCount(const QModelIndex &parent) const
{

//No need to mind about the parameter, it has no meaning for table models.

    qDebug () << "rowCount asked";

    qDebug () << currentTimers_.length();

    return currentTimers_.length();
}


int CurrentAlertsTableModel::columnCount(const QModelIndex &parent) const
{

    //No need to mind about the parameter, it has no meaning for table models.
    return numberOfColumns_;

}


QVariant CurrentAlertsTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    QString timeAsText; //here, because seems not to be allowed inside switch
    int allseconds;     //likewise
    QString hoursOnly;
    QString minutesOnly;
    QString secondsOnly;



    switch(role)
    {
        case Qt::TextAlignmentRole :

            switch (index.column())
            {

            case alertTextColumnNumber_:

                return int (Qt::AlignLeft || Qt::AlignVCenter);


            case timeRemainingColumnNumber_:

                return int (Qt::AlignRight || Qt::AlignVCenter);

            case statusColumnNumber_:

                return int (Qt::AlignLeft || Qt::AlignVCenter);

            }

            break;

        case Qt::DisplayRole :

            switch (index.column())
            {
                case alertTextColumnNumber_:



                    return currentTimers_.at(index.row())->getAlertText();


                case timeRemainingColumnNumber_:



                    allseconds = currentTimers_.at(index.row())->getRemainingTimeInSeconds();


                    if (allseconds < 0)
                    {
                        timeAsText = tr("-", "negative sign");
                        allseconds = -allseconds;

                    }

                    hoursOnly.setNum( allseconds/(60*60));


                    minutesOnly.setNum((allseconds%(60*60))/60);


                    secondsOnly.setNum (allseconds%60);


                    timeAsText += tr("%1:%2:%3", "%1 is hours, %2 is minutes and % 3 is seconds. Time remaining to alert, not time of day.").arg (hoursOnly,2,'0').arg(minutesOnly,2,'0').arg(secondsOnly,2,'0');




                    qDebug () << timeAsText;

                    return timeAsText;


                case statusColumnNumber_:

                    return QVariant();  //TO BE REPLACED WITH SENDING STATUS !


            }

            break;

      case Qt::ForegroundRole :

            //No need to care for the column number, all have the same color

            if (currentTimers_.at(index.row())->getRemainingTimeInSeconds() > 0)
                return QBrush (QColor(Qt::white));
            else return QBrush (QColor(Qt::red)); //change this to black if backgroundrole starts to work!



      case Qt::BackgroundRole :

            //For some reason, these have no effect at all!!! They are asked by the view though.

            //No need to care for the column number, all have the same color

            qDebug() << "BackgroundRole asked";

            if (currentTimers_.at(index.row())->getRemainingTimeInSeconds() > 0)
            {
                qDebug() << "black background";
                return QBrush (QColor(Qt::black));
            }
            else
            {
                qDebug() << "red background";
                return QBrush (QColor(Qt::red));
            }
        default:
            return QVariant();

    }



}




void CurrentAlertsTableModel::addTimers(QList <Timer *> timers)
{
    foreach (Timer* timer, timers)
    {
        connect (timer,SIGNAL(remainingTimeChanged()),this,SLOT(refreshTimeColumn()));
        qDebug() << "timer connected";
        timer->setParent(this); //The model becomes the timers parent giving the timer access to model
    }
    currentTimers_.append(timers);
    qDebug() << "Timers should be appended";
    reset();
}


void CurrentAlertsTableModel::refreshTimeColumn()
{
    qDebug() << "Refresh time column";



    emit dataChanged(createIndex(0,1),createIndex(rowCount(QModelIndex())-1,1));  //Entire time column refreshed

}


void CurrentAlertsTableModel::startTimer(QModelIndex index)
{
    currentTimers_.at(index.row())->start();
    refreshTimeColumn();
}

void CurrentAlertsTableModel::stopTimer(QModelIndex index)
{
  currentTimers_.at(index.row())->stop();
   refreshTimeColumn();
}

void CurrentAlertsTableModel::snoozeTimer(QModelIndex index)
{

    currentTimers_.at(index.row())->snooze();
    refreshTimeColumn();
}

QModelIndex CurrentAlertsTableModel::giveIndexForTimer(Timer * ptimer)
{
    int row = currentTimers_.indexOf(ptimer);
    if (row < -1) // if not found
        return QModelIndex(); //return invalid index


    return createIndex(row,0); //return index to the timer row's first column

}
