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
  @date 2010-06-26
  @version 0.09

Operates the UI.

*/

class KitchenAlertMainWindow : public QMainWindow
{
    Q_OBJECT 




public:
    explicit KitchenAlertMainWindow(QWidget *parent = 0);
    ~KitchenAlertMainWindow();


public slots:
    void openTimerSequence();
    void newTimerSequence();

    void updateTime(int seconds);
    void alert(QModelIndex indexOfAlerted);
    void timerSelected(QItemSelection,QItemSelection);
    void snooze();
    void restart();
    void stop();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::KitchenAlertMainWindow *ui;

    QList <Timer *> currentTimers_;

    CurrentAlertsTableModel model_;

    QModelIndex selectedRow();

    AlertSound alertSound_;
};

#endif // KITCHENALERTMAINWINDOW_H
