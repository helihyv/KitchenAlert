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





#ifndef ALERTSOUND_H
#define ALERTSOUND_H

#include <QObject>



#include <Phonon>


/*! Class for playing the alert sound'

  @author Heli Hyvättinen
  @date 2010-06-26
  @version 0.09

Class for playing (and stopping) the alert sound.

*/

class AlertSound : public QObject
{
    Q_OBJECT
public:
    explicit AlertSound(QObject *parent = 0);

signals:

public slots:
    /*! Plays the sound */
    void play();

    /*! Stops the sound from playing */
    void stop();

    /*! Changes the sound file (both current and saved in settings) */
    void setSound(QString filename);

    void setDefaultSound();

private:



    Phonon::MediaObject *pSound_;
    QString defaultsound_;


};

#endif // ALERTSOUND_H
