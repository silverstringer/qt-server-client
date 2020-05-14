#include "clientwidget.h"
#include "clientsocketadapter.h"

namespace RBT {

ClientWidget::ClientWidget(QWidget *parent, quint16 port, QString host) : QWidget(parent), m_port(port),m_host(host), m_pSock(new ClientSocketAdapter(this, m_host, m_port)) {

    createWindow();
    init();

    connect(m_pSock, SIGNAL(message(QString)), SLOT(on_message(QString)));
    connect(btnSend, SIGNAL(clicked()), SLOT(on_send()));
    connect(m_pSock, SIGNAL(disconnected()), this, SLOT(reconnect()));
    connect(m_pSock, SIGNAL(connected()), this, SLOT(on_connected()));
    connect(&m_timerToRead, &QTimer::timeout, this, [&]() { this->sendPackage(typePackage::COMMAND_READ); });
    connect(&m_timerToWrite, &QTimer::timeout, this, [&]() { this->sendPackage(typePackage::COMMAND_WRITE); });

}

ClientWidget::~ClientWidget() {

    m_timerToRead.stop();
    m_timerToWrite.stop();

    delete m_pSock;
}

void ClientWidget::on_message(const QString text) const {
    recieve_messages->setHtml(recieve_messages->toHtml() + text + "<br>");
}

void ClientWidget::on_send() {

    QString pack ;
    auto message = spinboxMessage->text();
    packageRead->setData(message);
    packageRead->serialize(pack);

    try {
        m_pSock->sendString(pack);
        spinboxMessage->clear();
    }
    catch (std::exception &e) {
        qDebug() << "Exception" << e.what();

    }

}

void ClientWidget::reconnect() {
//    delete m_pSock;
    m_pSock = nullptr;//rights delete m_pSock, freeze
    m_pSock = new ClientSocketAdapter(this, m_host, m_port);
    lbStatusConnection->setText("Status:Reconnect");
}

void ClientWidget::on_connected() {
    lbStatusConnection->setText("Status:Сonnected");
}


void ClientWidget::sendPackage(typePackage type) {

    auto pack_data = QString::number(Helper::generateRandomNumber(0, CAPACITY/2));

    QString message;

    if (type == typePackage::COMMAND_READ) {
        packageRead->setData(pack_data);
        packageRead->serialize(message);
    }

    if (type == typePackage::COMMAND_WRITE) {
        packageW->setData(pack_data);
        packageW->serialize(message);
    }

    m_pSock->sendString(message);
    spinboxMessage->clear();


}


void ClientWidget::createWindow() {

    mainLayout = new QVBoxLayout(this);
    lbParams = new QLabel("On hostname: " + m_host + ":" + QString::number(m_port));
    lbStatusConnection = new QLabel("Status:");
    btnSend = new QPushButton("Send");
    spinboxMessage = new QSpinBox;
    spinboxMessage->setMinimum(1);
    spinboxMessage->setMaximum(CAPACITY/2);
    recieve_messages = new QTextEdit;
    recieve_messages->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    mainLayout->addWidget(lbParams);
    mainLayout->addWidget(lbStatusConnection);
    mainLayout->addWidget(recieve_messages);
    mainLayout->addWidget(spinboxMessage);
    mainLayout->addWidget(btnSend);
    setLayout(mainLayout);
}

void ClientWidget::init() {

    m_time.packageReadValue = 6000;
    m_time.packageWriteValue = m_time.packageReadValue * 9;
    m_uidClient = QUuid::createUuid().toString();
//    m_uidClient = QUuid::createUuid().toString(QUuid::WithoutBraces);

    m_timerToRead.start(m_time.packageReadValue);
    m_timerToWrite.start(m_time.packageWriteValue);

    packageRead = std::make_shared<Package>(m_uidClient, QString::number(typePackage::COMMAND_READ), QString("READ"),
                                            QString("10"));
    packageW = std::make_shared<Package>(m_uidClient, QString::number(typePackage::COMMAND_WRITE), QString("WRITE"),
                                         QString("10"));


    std::string current_filename = m_uidClient.toStdString() + "_log.csv";
    std::string dump_dir = std::string(getenv("HOME")) + "/.incoma";
    log.settings(true, dump_dir, current_filename);
    std::vector<std::string> heading{"timestamp", "package"};
    log.addDatainRow(heading.begin(), heading.end());

}
}