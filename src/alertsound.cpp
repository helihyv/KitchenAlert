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



AlertSound::AlertSound(QObject *parent) :
    QObject(parent)
{


    pSound_ = Phonon::createPlayer(Phonon::MusicCategory, Phonon::MediaSource("/home/user/MyDocs/KitchenAlertTestSound1.wav"));
}

void AlertSound::play()
{

    pSound_->play();
    qDebug() << "Sound should be played now";
}

void AlertSound::stop()
{

    pSound_->stop();
}
