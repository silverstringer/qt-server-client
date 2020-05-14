#include <QCoreApplication>
#include <QDir>
#include <QSharedMemory>
#include "server.h"

int main(int argc, char *argv[]) {


  QCoreApplication app(argc, argv);

  RBT::Server server(5151);

  QDir home(QDir::homePath() + "/.incoma");
  if(!home.exists())
      home.mkdir(home.path());

  QFile file(QDir::homePath() + "/.incoma/semaphore");
  file.open(QIODevice::ReadWrite | QIODevice::Text);
  file.write(QString(" ").toLatin1());
  file.close();
  {
      QSharedMemory sm("incoma");
      sm.attach();
  }
  QSharedMemory sm("incoma");

  if(sm.attach()) {
      return 1;
  }
  if(!sm.create(1))
      return 1;

  app.exec();
}
