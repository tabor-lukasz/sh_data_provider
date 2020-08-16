#include "dbfeeder.h"
#include <QtDebug>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

DBFeeder::DBFeeder(QObject *parent) : QObject(parent)
{
    m_db = QSqlDatabase::addDatabase("QMYSQL");
    m_db.setHostName("localhost");
    m_db.setDatabaseName("smart_home");
    m_db.setUserName("lt");
    m_db.setPassword("Leninek83");
}

DBFeeder::~DBFeeder()
{
    disconnect();
}

bool DBFeeder::connect()
{
    if (m_db.isOpen()) {
        qDebug() << "DB already opened";
        return false;
    }
    bool ok = m_db.open();
    if (!ok) {
        qDebug() << m_db.lastError();
    } else {
        qDebug() << "Connected to db";
        emit sensorsConfigRead(getSensorsCfg());
    }

    return ok;
}

void DBFeeder::disconnect()
{
    if (m_db.isOpen()) {
        m_db.close();
    }
}

void DBFeeder::onDataReady(QList<SensorData> data)
{
    if (m_db.isOpen()) {
        QSqlQuery query("INSERT INTO readings (sensor_id,value,ts,location_id) VALUES (?, ?, ?, ?)",m_db);
        QVariantList ids, values, tss, loc;
        for (const auto& d : data) {
            ids << d.sensor_id;
            values << d.value;
            tss << d.ts;
            loc << m_sensor2location[d.sensor_id];
            qDebug() << "querry: " << d.sensor_id << d.value << d.ts << m_sensor2location[d.sensor_id];
        }
        query.addBindValue(ids);
        query.addBindValue(values);
        query.addBindValue(tss);
        query.addBindValue(loc);
        if (!query.execBatch()) {
            qDebug() << query.lastError();
        }
    } else {
        qDebug() << "DB connection down";
    }
}

QVector<SensorConfig> DBFeeder::getSensorsCfg()
{
    QVector<SensorConfig> rval;
    if (m_db.isOpen()) {
        QSqlQuery query("SELECT sensor_id,type,identifier,location_id FROM sensors",m_db);
        while (query.next()) {
            int id = query.value(0).toInt();
            SensorType type = static_cast<SensorType>(query.value(1).toInt());
            QString ident = query.value(2).toString();
            int loc = query.value(3).toInt();
            rval.append({id,ident,type});
            m_sensor2location.insert(id,loc);
        }
    }
    return rval;
}
