/**************************************************************************
        KitchenAlert v.0.01

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





#ifndef CHOOSETIMERSEQUENCEDIALOG_H
#define CHOOSETIMERSEQUENCEDIALOG_H

#include <QDialog>
#include "timerschedule.h"

namespace Ui {
    class ChooseTimerSequenceDialog;
}

class ChooseTimerSequenceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChooseTimerSequenceDialog(QWidget *parent = 0);
    ~ChooseTimerSequenceDialog();


    //Tarvitsee funktion joka palauttaa ajastinlistan!

    TimerSchedule giveTimers();


protected:
    void changeEvent(QEvent *e);

private:
    Ui::ChooseTimerSequenceDialog *ui;
};

#endif // CHOOSETIMERSEQUENCEDIALOG_H
