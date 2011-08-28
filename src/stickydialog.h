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

#ifndef STICKYDIALOG_H
#define STICKYDIALOG_H

#include <QDialog>
#include <QListView>
#include <QStringListModel>

class StickyDialog : public QDialog


        /*! The class for dialog for selecting sticky timers of KitchenAlert'

          @author Heli Hyvättinen
          @date 2011-08-14
          @version 0.4.0

        The class for dialog for selecting sticky timers of KitchenAlert.

        */
{
    Q_OBJECT
public:
    explicit StickyDialog(QString defaultDirectory,  QWidget *parent = 0);

    QStringList getStickyList();


signals:

public slots:

    void add();
    void remove();

protected:

    QListView * pStickiesView_;
    QStringListModel * pStickiesModel_;

    QString defaultDirectory_;



};

#endif // STICKYDIALOG_H
