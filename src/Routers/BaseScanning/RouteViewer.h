//
//  Copyright (C) 2016-2017  David Hedbor <neotron@gmail.com>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#pragma once

#include <QMainWindow>
#include <QItemSelection>
#include "System.h"
#include "TSPWorker.h"
#include "RouteTableModel.h"
#include "ImageLoader.h"

namespace Ui {
    class RouteViewer;
}

class RouteViewer : public QMainWindow {
Q_OBJECT

public:
    explicit RouteViewer(const RouteResult &result, QWidget *parent = nullptr);

    ~RouteViewer();

public slots:

    void loadSelectedImage(const QString &image);
    void copySelectedItem();
    void exportAsCSV();
    void exportAsTabNewline();

private:
    Ui::RouteViewer *_ui;

    void updateSettlementInfo();

    RouteTableModel *_routeModel;

    void setFlag(const Settlement &settlement, QString key, SettlementFlags flag);

    ImageLoader *_iconLoader;
    ImageLoader *_imageLoader;

    void loadOverviewImage(const QUrl &url);
};

