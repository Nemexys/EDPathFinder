#pragma once

#include <QDialog>
#include <QLineEdit>

#ifndef M_PI
#ifdef Q_OS_WIN
#define _USE_MATH_DEFINES
#endif

#include <math.h>
#endif

namespace Ui {
class BearingCalculator;
}

class BearingCalculator : public QDialog {
Q_OBJECT


public:
    explicit BearingCalculator(QWidget *parent = nullptr);

    ~BearingCalculator() override;

    void setDestination(double lat, double lon, double radius);

public slots:
    void calculateBearingAndDistance();

private:
    struct Degrees {
        explicit Degrees(double val_) : val(val_) {}

        double toRadians() const {
            return val * M_PI / 180.0;
        }

        double toDegrees() const {
            return val * 180.0 / M_PI;
        }
        Degrees operator-(const Degrees &other) {
            return Degrees(val - other.val);
        }
        double val;
    };

    struct LatLong {
        LatLong() : lat(0), lon(0) {}

        Degrees lat, lon;
    };


    Ui::BearingCalculator *_ui;

    const LatLong endCoords(bool *ok);

    const LatLong startCoords(bool *ok);

    const LatLong coordsFor(QLineEdit *lat, QLineEdit *lon, bool *ok);
};
