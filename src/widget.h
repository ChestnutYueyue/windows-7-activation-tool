#ifndef WIDGET_H
#define WIDGET_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QMap>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QProcess>
#include <QWidget>
#include <QThreadPool>
#include "mythread.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget {
  Q_OBJECT

public:
  Widget(QWidget *parent = nullptr);
  void setButton();
  ~Widget();
signals:
  void SendCiD(QString );
  void SendActivation(QString);
private:
  Ui::Widget *ui;
  QNetworkAccessManager *manager,*iidManager;
  QJsonObject jsonObject{{"keys", ""},
                         {"justgetdescription", "0"},
                         {"username", "trogiup24h"},
                         {"password", "PHO"},
                         {"_", "1652533184040"}};
  QJsonObject iidObject{{"iids", ""},
                        {"username", "trogiup24h"},
                        {"password", "PHO"},
                        {"_", "1652542671535"}};
  QMap<QString, QString> Retail_code{
      {"0xC004C008", "可以电话激活或者网页激活"},
      {"0xC004C060", "密钥已被封杀，不再可用"},
      {"0xC004C020", "密钥剩余次数暂时归0，可尝试电话激活"},
      {"0xC004C003", "密钥已被封杀，不再可用"},
      {"0xC004C004", "密钥被锁定，仅供特殊人群使用"}};
  QString Keys;
  QString Iid;
  QString Cid;
  QThreadPool *pool;
};
#endif // WIDGET_H
