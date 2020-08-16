#include "sensorreader.h"
#include <QDebug>
#include <fstream>


SensorReader::SensorReader(QObject *parent) : QObject(parent)
{
    connect(&m_timer,SIGNAL(timeout()),this,SLOT(onTimer()));
}

void SensorReader::start()
{
    m_timer.setSingleShot(true);
    QTime time = QTime::currentTime();
    QDate date = QDate::currentDate();
    QDateTime ts;
    if (time.second() >= 10) {
        time.setHMS(time.hour(),time.minute(),10);
        ts = QDateTime(date,time);
        ts = ts.addSecs(60);
    } else {
        time.setHMS(time.hour(),time.minute(),10);
        ts = QDateTime(date,time);
    }
    m_timer.start(QDateTime::currentDateTime().msecsTo(ts));
}

void SensorReader::stop()
{
    m_timer.stop();
}

void SensorReader::onSensorConfig(QVector<SensorConfig> sensor_cfgs)
{
    m_timer.stop();
    m_sensor_cfgs = sensor_cfgs;
}

void SensorReader::onTimer()
{
    QList<SensorData> data;
    QDateTime ts = QDateTime::currentDateTime();
    for (const auto& cfg : m_sensor_cfgs) {
        int value;
        if (getValue(cfg,value)) {
            data.push_back({cfg.id,value,ts});
        } else {
            qDebug() << "Get sensor data failed: " << cfg.identifier;
        }
    }
    if (data.size()) {
        emit dataRead(data);
    }

    QTime time = QTime::currentTime();
    QDate date = QDate::currentDate();
    time.setHMS(time.hour(),time.minute(),10);
    ts = QDateTime(date,time);
    ts = ts.addSecs(60);
    m_timer.start(QDateTime::currentDateTime().msecsTo(ts));
}

bool SensorReader::getValue(const SensorConfig& sensor_cfg, int &dest)
{
    std::ifstream file((m_sensorPath + sensor_cfg.identifier + m_sensorDataSuffix).toStdString());
    std::string sdata;
    while (file.good()) {
        std::string line;
        std::getline(file,line);
        sdata += line;
    }
    size_t pos = sdata.rfind("t=");
    if (pos != std::string::npos) {
        QString valstr(sdata.substr(pos+2).c_str());
        bool ok;
        dest = valstr.toInt(&ok);
        if (ok) {
            return true;
        }
    }
    return false;
}
