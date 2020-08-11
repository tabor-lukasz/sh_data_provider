#ifndef SENSORREADER_H
#define SENSORREADER_H

#include <QObject>
#include <QTimer>
#include <QVector>
#include <QList>

#include "sensordata.h"

class SensorReader : public QObject
{
    Q_OBJECT
public:
    explicit SensorReader(QObject *parent = nullptr);
    virtual ~SensorReader() {}
    void start();
    void stop();

signals:
    void dataRead(QList<SensorData>);

public slots:
    void onSensorConfig(QVector<SensorConfig> sensor_cfgs);

private slots:
    void onTimer();

private:
    QVector<SensorConfig> m_sensor_cfgs;
    QTimer m_timer;

    bool getValue(const SensorConfig& sensor_cfg, int &dest);

    const QString m_sensorPath = "/sys/bus/w1/devices/";
    const QString m_sensorDataSuffix = "/w1_slave";
};

#endif // SENSORREADER_H
