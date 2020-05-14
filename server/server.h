#pragma once

#include <QObject>
#include <QList>
#include <QDebug>
#include <QTimer>
#include <QTime>
#include "../common/common.h"
#include "memory"


class QTcpServer;
class QTcpSocket;
class ISocketAdapter;



namespace RBT {

    using Logger = Log::CSVLogger;
    using pPackage = std::shared_ptr<Package>;
    using typePackage = Package::TypePackage;
    using DataType = container_map<int>;

    class Server : public QObject {
    Q_OBJECT
    public:
        explicit Server(int nPort, QObject *parent = nullptr);

    DECLARE_NO_COPY_CLASS(Server);

    public slots:
        void on_message(QString);

    private slots:
        void addClient();
        void removeClient();
    private:
        void init();
        void package_log(pPackage package);
        void broadcast(const QString &msg);
        void responseClient(const QString &msg);
        void handlerPackage();
    protected:
        QTcpServer *m_ptcpServer;
        QList<ISocketAdapter *> m_clients;
        Logger log;
        pPackage m_package;
        DataType container;
    };

}
