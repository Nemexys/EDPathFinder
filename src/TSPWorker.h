//
//  Copyright (C) 2016  David Hedbor <neotron@gmail.com>
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


// Based on Traveling Salesman Sample from Google Optimzation tools (tsp.cpp),
// which was released under the following licensing terms:
//
// Copyright 2010-2014 Google
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//


#pragma once

#include <deque>
#include <QThread>
#include "System.h"
#include "constraint_solver/routing.h"

typedef std::vector<std::vector<QString>> RouteResultMatrix;

struct RouteResult {
    RouteResult(): ly(), route() {}

    QString ly;
    RouteResultMatrix route;
};

namespace operations_research {
    class TSPWorker : public QThread {
    Q_OBJECT

    public:
        TSPWorker(const SystemList &systems, double x, double y, double z, int maxSystemCount)
                : QThread(), _systems(systems), _x(x), _y(y), _z(z), _maxSystemCount(maxSystemCount) { }


        virtual void run();

    signals:
        void taskCompleted(const RouteResult &route);

    private:
        int64 systemDistance(RoutingModel::NodeIndex from, RoutingModel::NodeIndex to);
        SystemList _systems;
        double _x, _y, _z;
    private:
        int _maxSystemCount;

    };
};

using operations_research::TSPWorker;

