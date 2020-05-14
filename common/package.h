#pragma once

#include <QString>
#include <QDataStream>
#include <QDebug>

class Package
{

private:
    struct  package
    {
        QString id;
        QString type;
        QString command;
        QString data;

    } m_package;

public:
    explicit Package(const QString &id, const QString &type, const QString &command, const QString data);
    Package();
    ~Package(){}
    enum TypePackage
    {
        NAME_SERVER =1,
        TOTAL_CLIENTS =2,
        CLIENTS_ONLINE =3,
        SEND_MESSAGE =4,
        COMMAND_READ = 5,
        COMMAND_WRITE = 6
    };

    QString toString();

    void serialize(QString &message);
    void deserialize(QString &message);
    void setData(QString &data) { m_package.data = data;}
    auto getData() {return m_package;}
    friend QDataStream& operator<<(QDataStream& os,const Package &p);
    friend QDataStream& operator>>(QDataStream& in, Package &p);

};
