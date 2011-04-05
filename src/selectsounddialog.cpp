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





#include "selectsounddialog.h"
#include "ui_selectsounddialog.h"
#include <QFileDialog>
#include <QSettings>
#include <QDebug>

SelectSoundDialog::SelectSoundDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectSoundDialog)
{
    ui->setupUi(this);
    connect(ui->browseButton,SIGNAL(clicked()),this,SLOT(browse()));

    QSettings settings("KitchenAlert","KitchenAlert");
    QString filename = settings.value("soundfile").toString();
    ui->lineEdit->setText(filename);

    bool useDefaultSoundFile = settings.value("UseDefaultSound",true).toBool();
    if (useDefaultSoundFile == true)
    {
        ui->DefaultSoundRadioButton->setChecked(true);
        ui->browseButton->setDisabled(true);
        ui->lineEdit->setDisabled(true);
    }
    else ui->CustomSoundRadioButton->setChecked(true);
//    qDebug() << "UseDefaultSoundfile is " << useDefaultSoundFile;
}

SelectSoundDialog::~SelectSoundDialog()
{
    delete ui;
}

void SelectSoundDialog::browse()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Choose a sound file"),"/home/user/"); //Filters: to use or not...? //MAEMO specific dir here...
    if (!filename.isEmpty()) //Empty string returned by the dialog if user pressed cancel...
    {
        ui->lineEdit->setText(filename);
    }
}

QString SelectSoundDialog::getSoundFileName()
{
    if (ui->CustomSoundRadioButton->isChecked() == true)
        return ui->lineEdit->displayText();
    else return QString();
}

bool SelectSoundDialog::isDefaultSoundChecked()
{
    return ui->DefaultSoundRadioButton->isChecked();
}
