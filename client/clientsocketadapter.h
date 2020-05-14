#ifndef CLIENTSOCKETADAPTER_H
#define CLIENTSOCKETADAPTER_H

#include "../common/socketadapter.h"
#include <QTimer>

class ClientSocketAdapter : public SocketAdapter {
  Q_OBJECT
public:
  explicit ClientSocketAdapter(QObject *parent,const QString host,const quint16 port);
  bool isConnected();
private slots:
  void slot_reconnect();
protected:
    QString hostName;
    quint16 port;

signals:
   void reconnect();
   void connected();
};

#endif // CLIENTSOCKETADAPTER_H
