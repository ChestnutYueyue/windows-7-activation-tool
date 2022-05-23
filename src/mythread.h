#ifndef MYTHREAD_H
#define MYTHREAD_H
#include <QMutex>
#include <QMutexLocker>
#include <QObject>
#include <QRunnable>
#include <QString>
#include <QThread>
#include <QProcess>
#include <QWaitCondition>

class MyThread : public QObject, public QRunnable {
  Q_OBJECT
public:
  MyThread(QString Key,QObject *parent = nullptr);
  void run();
signals:
  void finished(QString _message);
  void SendKeys(QString _message);
private:
  QProcess *process;
  QMutex mutex;
  QString Keys;
};

#endif