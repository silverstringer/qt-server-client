#include "clientsocketadapter.h"
#include <QVector>
#include <QTcpSocket>
#include <QDataStream>
#include <chrono>
#include <thread>


ClientSocketAdapter::ClientSocketAdapter(QObject *parent,const QString inHost,const quint16 inPort): SocketAdapter(parent),
    hostName(inHost),port(inPort)
{
    m_ptcpSocket->connectToHost(hostName, port);
    connect(m_ptcpSocket, SIGNAL(disconnected()), this, SLOT(slot_reconnect()));
}

bool ClientSocketAdapter::isConnected(){

    bool connected = ( m_ptcpSocket->state() == QTcpSocket::ConnectedState);
    return connected;
}

void ClientSocketAdapter::slot_reconnect()
{
//     m_ptcpSocket->disconnectFromHost();
     qDebug()<<"Reconnect to server";
     emit reconnect();
       while(isConnected()!=true) {
           std::this_thread::sleep_for(std::chrono::milliseconds(800));
           m_ptcpSocket->abort();
           m_ptcpSocket->connectToHost(hostName, port);
           m_ptcpSocket->waitForConnected(1000);
       }
       emit connected();
}
