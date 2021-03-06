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





#ifndef SELECTSOUNDDIALOG_H
#define SELECTSOUNDDIALOG_H

#include <QDialog>
#include <QMediaPlayer>

namespace Ui {
    class SelectSoundDialog;
}

/*! The class for sound selection dialog of KitchenAlert'

  @author Heli Hyvättinen
  @date 2011-04-27
  @version 0.3.0

The class for sound selection dialog in KitchenAlert.

*/

class SelectSoundDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectSoundDialog(QWidget *parent = 0);
    ~SelectSoundDialog();

    QString getSoundFileName();
    /*! returns the filename for custom sound the user has chosen in the dialog */

    bool isDefaultSoundChecked();
    /*! Returns whether "use default sound" was checked by the user in the dialog */

public slots:
    void browse();
    void testSound();

private:
    Ui::SelectSoundDialog *ui;

    QMediaPlayer player;
    bool isTesting_;
};

#endif // SELECTSOUNDDIALOG_H
