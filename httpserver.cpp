#include "httpserver.h"
#include <QtNetwork/QTcpSocket>

HttpServer::HttpServer(QObject *parent) : QObject(parent)
{
    connect(&m_server,&QTcpServer::newConnection,this,&HttpServer::onNewConection);
}

void HttpServer::run()
{
    if (!m_server.listen(QHostAddress::Any,8008)) {
        qDebug() << m_server.serverError();
    }
}

void HttpServer::onNewConection()
{
    auto socket = m_server.nextPendingConnection();
    if (!socket) return;
    m_socket2data[socket] = QByteArray();
    connect(socket,&QTcpSocket::readyRead,this,&HttpServer::onSocketReadyRead);
    connect(socket,&QTcpSocket::disconnected,this,&HttpServer::onSocketDisconnected);
}

void HttpServer::onSocketReadyRead()
{
    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    m_socket2data[socket].append(socket->readAll());
    qDebug() << "c";
    if (m_socket2data[socket].contains("\r\n\r\n")){
        qDebug() << "a";
        if (m_socket2data[socket].startsWith("GET")) {
            qDebug() << "b";
            QString request = m_socket2data[socket];
            auto list = request.split(' ');
            qDebug() << request;
            processGetRequest(socket,list[1]);
        }
    }
}

void HttpServer::onSocketDisconnected()
{
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    if(socket)
    {
        m_socket2data.remove(socket);
        socket->deleteLater();
    }
    else
    {
        qDebug() << "WTF";
    }
}

void HttpServer::processGetRequest(QTcpSocket *socket, QString endpoint)
{
    qDebug() << "processing GET:" << endpoint;
    if (endpoint.compare("/live") == 0){
        qDebug() << socket->write("{\"aaa\": \"1\", \"bbb\": \"4\"}");
    }
}
