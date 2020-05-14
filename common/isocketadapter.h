#pragma once

#include <QObject>

class ISocketAdapter : public QObject {
  Q_OBJECT
public:
  explicit ISocketAdapter(QObject *parent);
  virtual ~ISocketAdapter();
  virtual void sendString(const QString& str) = 0;
signals:
  void message(QString text);
  void disconnected();
};

