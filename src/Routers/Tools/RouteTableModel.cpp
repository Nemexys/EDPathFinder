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

#include <QTableView>
#include <QFileDialog>
#include <src/Settings/Settings.h>
#include "RouteTableModel.h"

RouteTableModel::RouteTableModel(QObject *parent, const RouteResult &result)
        : QAbstractTableModel(parent), _result(result), _resultType(ResultTypeSettlement) {}

int RouteTableModel::rowCount(const QModelIndex &) const {
    return (int) _result.route().size();
}

int RouteTableModel::columnCount(const QModelIndex &) const {
    auto size = (int) _result.route()[0].size();
    return _resultType == ResultTypeValuableSystems || _resultType == ResultTypePresets ? size : size - 3;
}

QVariant RouteTableModel::data(const QModelIndex &index, int role) const {
    auto col = (size_t) index.column();
    auto row = (size_t) index.row();
    auto route = _result.route();
    if(row < route.size() && col < route[row].size()) {
        switch(role) {
        case Qt::DisplayRole:
            return route[row][col];

        case Qt::TextAlignmentRole:
            switch(_resultType) {
            default:
                return col < 3 ? Qt::AlignLeft : Qt::AlignRight;
            case ResultTypePresets:
                return Qt::AlignLeft;
            case ResultTypeValuableSystems:
                return col % 2 ? Qt::AlignRight : Qt::AlignLeft;
            }

        default:
            return QVariant();
        }
    }
    return QVariant();
}

QVariant RouteTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if(role == Qt::DisplayRole) {
        if(orientation == Qt::Horizontal) {
            switch(section) {
            case 0:
                return "System";
            case 1:
                return _resultType == ResultTypeSettlement ? "Planet" : "Distance";
            case 2:
                switch(_resultType) {
                case ResultTypeSettlement:
                    return "Settlement";
                case ResultTypeSystemsOnly:
                    return "";
                case ResultTypeValuableSystems:
                    return "Planets";
                case ResultTypePresets:
                    return "Type";
                }
            case 3:
                switch(_resultType) {
                case ResultTypeSettlement:
                case ResultTypeSystemsOnly:
                    return "";
                case ResultTypeValuableSystems:
                    return "Value";
                case ResultTypePresets:
                    return "Name";
                }
            case 4:
                return _resultType == ResultTypePresets ? "Notes" : "";
            default:
                return "";
            }
        }
    }
    return QVariant();
}

QString RouteTableModel::lastDistance(size_t row) const {
    auto route = _result.route();
    return route[row][3];
}


QString RouteTableModel::totalDistance(size_t row) const {
    auto route = _result.route();
    return route[row][4];
}

void RouteTableModel::exportTableViewToCSV(QTableView *table) {
    QString filters("CSV files (*.csv);;All files (*.*)");
    QString defaultFilter("CSV files (*.csv)");
    QString fileName = QFileDialog::getSaveFileName(nullptr, "Save file", Settings::restoreSavePath(), filters, &defaultFilter);

    exportTableView(table, fileName, true);

}

void RouteTableModel::exportTableViewToTabNewline(QTableView *table) {
    QString filters("Text files (*.txt);;All files (*.*)");
    QString defaultFilter("Text files (*.txt)");
    QString fileName = QFileDialog::getSaveFileName(nullptr, "Save file", Settings::restoreSavePath(), filters, &defaultFilter);

    exportTableView(table, fileName, false);
}

void RouteTableModel::exportTableView(QTableView *table, const QString &fileName, bool isCSV) {
    if(fileName.isEmpty()) {
        return;
    }
    Settings::saveSavePath(fileName);
    QFile file(fileName);
    if(!file.open(QFile::WriteOnly | QFile::Truncate)) {
        return;
    }

    QAbstractItemModel *model = table->model();
    QString sep(isCSV ? ";" : "\t");
    QTextStream data(&file);
    QStringList strList;
    for(int i = 0; i < model->columnCount(); i++) {
        const auto header = model->headerData(i, Qt::Horizontal, Qt::DisplayRole).toString();
        if(header.length() > 0) {
            strList.append(isCSV ? "\"" + header + "\"" : header);
        } else {
            strList.append("");
        }
    }
    data << strList.join(sep) << endl;
    for(int i = 0; i < model->rowCount(); i++) {
        strList.clear();
        for(int j = 0; j < model->columnCount(); j++) {
            auto field = model->data(model->index(i, j)).toString();
            if(field.length() > 0) {
                field = field.replace("\n", ", ");
                strList.append(isCSV ? "\"" + field + "\"" : field);
            } else {
                strList.append("");
            }
        }
        data << strList.join(sep) << endl;
    }
    file.close();
}

RouteTableModel::ResultType RouteTableModel::resultType() const {
    return _resultType;
}
