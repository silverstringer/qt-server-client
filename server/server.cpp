#include "server.h"
#include "serversocketadapter.h"

#include <QTcpServer>
#include <QTcpSocket>
#include <QString>
#include <thread>
#include <future>

namespace RBT {

    Server::Server(int nPort, QObject *parent) : QObject(parent), m_ptcpServer(new QTcpServer(this)) {

        connect(m_ptcpServer, SIGNAL(newConnection()), SLOT(addClient()));

        if (m_ptcpServer->listen(QHostAddress::Any, nPort) == false) {
            m_ptcpServer->close();
            throw m_ptcpServer->errorString();
        }

        qDebug() << "Start server to port: " << nPort;

        init();
    }

    void Server::addClient() {

        qDebug() << "new connection \n";

        QTcpSocket *pclientSock = m_ptcpServer->nextPendingConnection();
        ISocketAdapter *pSockHandle = new ServerSocketAdapter(pclientSock);

        m_clients.push_back(pSockHandle);

        pSockHandle->sendString("connect");

        connect(pSockHandle, SIGNAL(disconnected()), SLOT(removeClient()));
        connect(pSockHandle, SIGNAL(message(QString)), SLOT(on_message(QString)));
        qDebug() << "Connection clients" << m_clients.size() << "\n";
    }

    void Server::removeClient() {

        qDebug() << "client disconnected \n";
        ISocketAdapter *client = s_cast(ServerSocketAdapter, sender);
        Q_ASSERT(client != nullptr);
        m_clients.removeOne(client);
        delete client;
    }

    void Server::on_message(QString msg) {

        m_package->deserialize(msg);

        auto[uid, type, command, data] = m_package->getData();

        qDebug() << "ID CLIENTS: " << uid << "Command:" << command << "DATA:" << data;

        package_log(m_package);

        handlerPackage();

    }

    void Server::broadcast(const QString &msg) {
        for (auto &sock:m_clients)
            sock->sendString(msg);
    }


    void Server::responseClient(const QString &msg) {

        if (std::find(m_clients.begin(), m_clients.end(), s_cast(ServerSocketAdapter, sender)) != m_clients.end()) {
            s_cast(ServerSocketAdapter, sender)->sendString(msg);
        }

    }


    void Server::handlerPackage() {

        auto &data = container.get(m_package->getData().data.toInt());
        auto type_package = m_package->getData().type.toInt();

        if (type_package == typePackage::COMMAND_READ) {

            std::packaged_task<int(int)> task([&](int _index) {
                auto result = container.get(_index);
                return result;
            });

            std::future<int> result = task.get_future();

           // Pass the packaged_task to thread to run asynchronously
            std::thread th(std::move(task), data);
            th.join();
            responseClient(QString::number(result.get()));
        }


        if (type_package == typePackage::COMMAND_WRITE) {
//        auto key = Helper::generateRandomNumber(CAPACITY,CAPACITY *2);
            bool value;
            std::thread t1([&](bool xx) { xx = container.add(data); }, value);
            t1.join();

//        qDebug()<<"Write to container" <<value;
//        responseClient(QString::number(fillContainer(container, key, data)));

        }
    }


    void Server::init() {
        qDebug() << "Max connections to server:" << m_ptcpServer->maxPendingConnections();

        std::string current_filename = "server_package_log.csv";
        std::string dump_dir = std::string(getenv("HOME")) + "/.incoma";
        log.settings(true, dump_dir, current_filename);
        HeaderType heading{"timestamp", "Client UID", "Type Package", "Command", "Data"};
        log.addDatainRow(heading.begin(), heading.end());

        qDebug() << "Log to path: " << QString(dump_dir.c_str());
        m_package = std::make_shared<Package>();


        for (auto i = 0; i < CAPACITY; i++) {
            auto value = Helper::generateRandomNumber(0, CAPACITY);
            fillContainer(container, i, value);
        }

    }

    void Server::package_log(pPackage package) {
        auto[uid, type, command, data] = package->getData();

        auto timestamp = QTime::currentTime().toString().toStdString();
        HeaderType heading{timestamp, uid.toStdString(), type.toStdString(),
                                         command.toStdString(), data.toStdString()};
        log.addDatainRow(heading.begin(), heading.end());
        heading.clear();
    }
}

