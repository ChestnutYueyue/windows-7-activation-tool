#include "mythread.h"

MyThread::MyThread(QString Key, QObject *parent)
    : QRunnable(), process(new QProcess(parent)) {
  this->Keys = Key;
  setAutoDelete(true);
}
void MyThread::run() {
  QMutexLocker locker(&mutex);
  process->start("cmd",
                 QStringList()
                     << "/c"
                     << "cscript //nologo c:\\windows\\system32\\slmgr.vbs"
                     << "/ipk" << Keys);
  process->waitForStarted();
  process->waitForFinished();
  emit SendKeys(QString::fromLocal8Bit(process->readAllStandardOutput()));
  process->start("cmd",
                 QStringList()
                     << "/c"
                     << "cscript //nologo c:\\windows\\system32\\slmgr.vbs"
                     << "/dti");
  process->waitForStarted();
  process->waitForFinished();
  emit finished(QString::fromLocal8Bit(process->readAllStandardOutput())
                    .replace(QString("安装 ID: "), QString("")));
}
