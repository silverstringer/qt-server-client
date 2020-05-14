#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QTextEdit>
#include <QLineEdit>
#include <QSpinBox>
#include <QLabel>
#include <QDebug>
#include <QTimer>
#include <QUuid>
#include <QIntValidator>
#include <random>
#include <memory>
#include "../common/common.h"



class ISocketAdapter;
class Package;

using typePackage = Package::TypePackage;
using Logger = Log::CSVLogger;
using pPackage = std::shared_ptr<Package>;

namespace RBT {

class Q_WIDGETS_EXPORT ClientWidget : public QWidget {
Q_OBJECT
public:
    explicit ClientWidget(QWidget *parent = nullptr, quint16 port =5151, QString host = "192.168.1.40" );
    ~ClientWidget();

    DECLARE_NO_COPY_CLASS(ClientWidget);
    void createWindow();
    void init();

    pPackage packageRead;
    pPackage packageW;
    Logger log;

    QString m_uidClient;
    QTimer m_timerToRead;
    QTimer m_timerToWrite;

    struct stafftime {
        int packageReadValue;
        int packageWriteValue;
    } m_time;

    //UI
    QTextEdit *recieve_messages;
    QPushButton *btnSend;
    QSpinBox *spinboxMessage;
    QVBoxLayout *mainLayout;
    QLabel *lbParams;
    QLabel *lbStatusConnection;

protected:
    quint16 m_port;
    QString m_host;
    ISocketAdapter *m_pSock;
public slots:
    void on_message(const QString text) const;
    void on_send();

private slots:
    void reconnect();
    void on_connected();
    void sendPackage(typePackage type);
};
}

//TODO:future pImpl usage to any clients or factory pattern