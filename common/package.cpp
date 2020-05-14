#include "package.h"

Package::Package(const QString& id, const QString& type, const QString& command, const QString data)
{
    m_package.id = id;
    m_package.type = type;
    m_package.command = command;
    m_package.data = data;

}

Package::Package() {

}

QString Package::toString()
{
    return m_package.id +";" + m_package.type +";" + m_package.command + ";" + m_package.data;
}


void Package::serialize(QString &message)
{
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out << *this;
    message = QString::fromStdString(data.toStdString());
}

void Package::deserialize(QString &message)
{
    QByteArray data = message.toUtf8();
    QDataStream in(&data, QIODevice::ReadOnly);
    in>>*this;
}

QDataStream& operator>>(QDataStream& in,  Package& p)
{
    in>>p.m_package.id>>p.m_package.type>>p.m_package.command>>p.m_package.data;
    return in;

}

QDataStream& operator<<(QDataStream& os, const Package& p)
{
    os<<p.m_package.id<<p.m_package.type<<p.m_package.command<<p.m_package.data;
    return os;

}
