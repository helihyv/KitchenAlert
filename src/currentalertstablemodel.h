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
  @date 2010-07-20
  @version 0.1

Class that contains the model that holds the timers

*/


class CurrentAlertsTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit CurrentAlertsTableModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const;

    int columnCount(const QModelIndex &parent) const;

    QVariant data(const QModelIndex &index, int role) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    QModelIndex giveIndexForTimer(Timer * ptimer);

    bool isThisTimerAlerting(QModelIndex index);

signals:

public slots:

    void addTimers(QList <Timer *> timers);

    void refreshTimeColumn ();

    void startTimer(QModelIndex index);
    void snoozeTimer(QModelIndex index);
    void stopTimer(QModelIndex index);


    void setUpdateViewOnChanges(bool update);

private:
    QList <Timer * > currentTimers_;

    static const int numberOfColumns_ = 3;
    static const int alertTextColumnNumber_ = 0;
    static const int timeRemainingColumnNumber_ = 1;
    static const int statusColumnNumber_ = 2;



    bool updateViewOnChanges_;

};

#endif // CURRENTALERTSTABLEMODEL_H
