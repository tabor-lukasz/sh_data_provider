#ifndef SENSORDATA_H
#define SENSORDATA_H

#include <QString>
#include <QDateTime>

enum class SensorType {
    Temperature = 1
};

struct SensorConfig {
    int id;
    QString identifier;
    SensorType type;
};

struct SensorData {
    int sensor_id;
    int value;
    QDateTime ts;
};

#endif // SENSORDATA_H
