/**************************************************************************
        KitchenAlert

        Copyright (C) 2010-2011  Heli Hyvättinen

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





#include "createtimersequencedialog.h"
#include "ui_createtimersequencedialog.h"

CreateTimerSequenceDialog::CreateTimerSequenceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateTimerSequenceDialog)
{
    ui->setupUi(this);
}

CreateTimerSequenceDialog::~CreateTimerSequenceDialog()
{
    delete ui;
}

void CreateTimerSequenceDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}


QList<Timer*> CreateTimerSequenceDialog::getTimers()
{
    QList<Timer *> timers;
    Timer* ptimer =new Timer();
    int timeInSeconds = (ui->Timer1HoursSpinBox->value() * 60 * 60)  + (ui->timer1minutesSpinBox->value() * 60) + ui->timer1SecondsSpinBox->value();
    ptimer->setOriginalTimeInSeconds(timeInSeconds);
    ptimer->setAlertText(ui->timer1AlertText->text());
    timers.append(ptimer);

    return timers;


}
