#include <iostream>
#include <QDebug>
#include <QCoreApplication>

#include "sensorreader.h"
#include "dbfeeder.h"
#include "httpserver.h"

using namespace std;

int main(int argc, char*argv[])
{
    QCoreApplication app(argc,argv);

    SensorReader reader;
    DBFeeder feeder;
    HttpServer server;

    QObject::connect(&reader,&SensorReader::dataRead,&feeder,&DBFeeder::onDataReady);
    QObject::connect(&feeder,&DBFeeder::sensorsConfigRead,&reader,&SensorReader::onSensorConfig);
    QObject::connect(&feeder,&DBFeeder::sensorsConfigRead,&server,&HttpServer::onSensorConfig);
    QObject::connect(&reader,&SensorReader::dataRead,&server,&HttpServer::onDataRead);

    if (feeder.connect()) {
        reader.start();
    }
    server.run();

    return app.exec();
}
