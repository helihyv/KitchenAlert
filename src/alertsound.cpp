/**************************************************************************
        This file is part of KitchenAlert v.0.09

        Copyright (C) 2010  Heli Hyvättinen

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





#include "alertsound.h"

#include <QDebug>
#include <QSettings>



AlertSound::AlertSound(QObject *parent) :
    QObject(parent)
{

    //THIS NEEDS TESTING: DOES IT REALLY CHANGE TUNE WHEN RESTARTING THE APPLICATION?

    defaultsound_ = "/home/opt/KitchenAlert/Doorbell-old-tring-modified-multiplied-low-quality.mp3";
    QString filename;

    QSettings settings("KitchenAlert","KitchenAlert");

   // settings.clear(); //REMOVE THIS AFTER TESTING!!!!!!

    bool useDefaultSound = settings.value("UseDefaultSound",true).toBool();
//    qDebug() << "In AlertSound constructor UseDefaultSound is " << useDefaultSound;
    if (useDefaultSound == true)
    {
        filename = defaultsound_;
    }
    else
    {
        filename = settings.value("soundfile",defaultsound_).toString();
    }
    pSound_ = Phonon::createPlayer(Phonon::MusicCategory, Phonon::MediaSource(filename));
}

AlertSound::~AlertSound()
{

    if (pSound_ != NULL)
    {
        delete pSound_;
    }
}

void AlertSound::play()
{
    pSound_->stop(); //Just testing if stopping the previous alert will prevent the jammming of the sound (only partially, but since it helped some, keeping it even if the problem was solved otherwise)
    pSound_->play();
    qDebug() << "Sound should be played now";
}

void AlertSound::stop()
{

    pSound_->stop();
    qDebug() << pSound_->state();
    qDebug() << "Sound stopped by AlertSound.";
}



void AlertSound::setSound(QString filename)
{
   QSettings settings("KitchenAlert","KitchenAlert");
   settings.setValue("UseDefaultSound",false);
   settings.setValue("soundfile",filename);
   pSound_->setCurrentSource(filename);
}

void AlertSound::setDefaultSound()
{
    QSettings settings ("KitchenAlert","KitchenAlert");
    settings.setValue("UseDefaultSound",true);
    pSound_->setCurrentSource(defaultsound_);
}
