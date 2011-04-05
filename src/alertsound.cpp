/**************************************************************************
        This file is part of KitchenAlert

        Copyright (C) 2010-2011  Heli Hyvättinen

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

    defaultsound_ = "/home/opt/KitchenAlert/DoorbellModifiedFinal.mp3";
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



      pSound_ = new QMediaPlayer;
      pSound_->setMedia(QUrl::fromLocalFile(filename));
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
     pSound_->play();
  //  qDebug() << "Sound should be played now";
}

void AlertSound::stop()
{

    pSound_->stop();
//    qDebug() << pSound_->state();
//    qDebug() << "Sound stopped by AlertSound.";
}



void AlertSound::setSound(QString filename)
{

   pSound_->setMedia(QUrl::fromLocalFile(filename));
}

void AlertSound::setDefaultSound()
{

    pSound_->setMedia(QUrl::fromLocalFile(defaultsound_));
}
