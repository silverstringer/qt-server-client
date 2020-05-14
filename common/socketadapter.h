#pragma once

#include "isocketadapter.h"

class QTcpSocket;
class SocketAdapter : public ISocketAdapter {
  Q_OBJECT
public:
  SocketAdapter(QObject *parent, QTcpSocket *pSock = nullptr);
  virtual ~SocketAdapter();
  virtual void sendString(const QString& str);
protected slots:
  void on_readyRead();
  void on_disconnected();
protected:
  QTcpSocket *m_ptcpSocket;
  qint16 m_msgSize;
};


