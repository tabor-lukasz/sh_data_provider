#ifndef DBFEEDER_H
#define DBFEEDER_H

#include <QObject>
#include <QVector>
#include <QtSql/QSqlDatabase>
#include <QTimer>
#include <QMap>

#include "sensordata.h"

class DBFeeder : public QObject
{
    Q_OBJECT
public:
    explicit DBFeeder(QObject *parent = nullptr);
    virtual ~DBFeeder();

    bool connect();
    void disconnect();

signals:
    void sensorsConfigRead(QVector<SensorConfig> sensor_cfgs);

public slots:
    void onDataReady(QList<SensorData> data);

private:
    QSqlDatabase m_db;
    QVector<SensorConfig> getSensorsCfg();
    QMap<int,int> m_sensor2location;
};

#endif // DBFEEDER_H
