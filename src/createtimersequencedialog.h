/**************************************************************************
        KitchenAlert

        Copyright (C) 2010-2011 Heli Hyvättinen

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





#ifndef CREATETIMERSEQUENCEDIALOG_H
#define CREATETIMERSEQUENCEDIALOG_H

#include <QDialog>
#include <QList>
#include "timer.h"

namespace Ui {
    class CreateTimerSequenceDialog;
}


/*! Class for the dialog for creating timers'

  @author Heli Hyvättinen
  @date 2011-03-29
  @version 0.2.1

Class for the dialog for creating timers

*/


class CreateTimerSequenceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateTimerSequenceDialog(QWidget *parent = 0);
    ~CreateTimerSequenceDialog();

    QList<Timer*> getTimers();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::CreateTimerSequenceDialog *ui;
};

#endif // CREATETIMERSEQUENCEDIALOG_H
