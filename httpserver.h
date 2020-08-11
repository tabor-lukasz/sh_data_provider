#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <QObject>
#include <QtNetwork/QTcpServer>
#include "sensordata.h"
#include <QMap>

class HttpServer : public QObject
{
    Q_OBJECT
public:
    explicit HttpServer(QObject *parent = nullptr);
    void run();

signals:

public slots:
    void onDataRead(const QList<SensorData>& data) {m_live_data = data;}

private slots:
    void onNewConection();
    void onSocketReadyRead();
    void onSocketDisconnected();

private:
    QTcpServer m_server;
    QList<SensorData> m_live_data;
    QMap<QTcpSocket*,QByteArray> m_socket2data;

    void processGetRequest(QTcpSocket *socket, QString endpoint);

};

#endif // WEBSERVER_H
