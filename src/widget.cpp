#include "./ui_widget.h"
#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent), ui(new Ui::Widget),
      manager(new QNetworkAccessManager(this)),
      iidManager(new QNetworkAccessManager(this)), pool(new QThreadPool(this)) {
  ui->setupUi(this);
  this->setButton();
}
void Widget::setButton() {
  connect(ui->textEdit, &QTextEdit::textChanged, this, [=]() {
    jsonObject["keys"] = ui->textEdit->toPlainText();
    Keys = ui->textEdit->toPlainText();
    ui->textBrowser->clear();
    ui->textBrowser->append(ui->textEdit->toPlainText());
  });
  connect(iidManager, &QNetworkAccessManager::finished, this,
          [=](QNetworkReply *reply) {
            if (reply->error() == QNetworkReply::NoError) {
              ui->textBrowser->append("请求远程服务器成功！");
              QString bytes = reply->readAll();
              QJsonDocument jsonDocument = QJsonDocument::fromJson(
                  bytes.replace(QString("["), QString(""))
                      .replace(QString("]"), QString(""))
                      .toUtf8());
              QJsonObject jsonObject = jsonDocument.object();
              if (jsonObject["confirmation_id_no_dash"].toString() == "") {
                ui->textBrowser->clear();
                ui->textBrowser->append(jsonObject["short_result"].toString() +
                                        jsonObject["result"].toString());
              } else {
                Cid = jsonObject["confirmation_id_no_dash"].toString();
                ui->textBrowser->append("确认Id: " + Cid);
                emit SendCiD(Cid);
              }

            } else {
              ui->textBrowser->clear();
              ui->textBrowser->append(reply->errorString());
            }
            reply->deleteLater();
          });
  connect(
      manager, &QNetworkAccessManager::finished, this,
      [=](QNetworkReply *reply) {
        if (reply->error() == QNetworkReply::NoError) {
          ui->textBrowser->clear();
          ui->textBrowser->append("请求远程服务器成功！");
          QString bytes = reply->readAll();
          QJsonDocument jsonDocument =
              QJsonDocument::fromJson(bytes.replace(QString("["), QString(""))
                                          .replace(QString("]"), QString(""))
                                          .toUtf8());
          QJsonObject jsonObject = jsonDocument.object();
          if (jsonObject["errorcode"].toString() == "") {
            ui->textBrowser->clear();
            ui->textBrowser->append("密钥输入错误，请输入正确的密钥");
          } else {
            ui->textBrowser->append(
                "密钥状态：" + Retail_code[jsonObject["errorcode"].toString()]);
          }
        } else {
          ui->textBrowser->clear();
          ui->textBrowser->append(reply->errorString());
        }

        reply->deleteLater();
      });
  connect(ui->Key, &QPushButton::clicked, this, [=]() {
    if (!jsonObject["keys"].toString().isEmpty()) {
      auto Json = QJsonDocument(jsonObject).toJson(QJsonDocument::Compact);
      QNetworkRequest request(QUrl("https://khoatoantin.com/ajax/pidms_api"));
      request.setHeader(QNetworkRequest::ContentTypeHeader,
                        QVariant("application/json"));
      manager->post(request, Json);
    } else {
      ui->textBrowser->clear();
      ui->textBrowser->append("密钥输入为空！");
    }
  });
  connect(ui->Activation, &QPushButton::clicked, this, [=]() {
    ui->textBrowser->clear();
    MyThread *thread = new MyThread(Keys, this);
    pool->start(thread);
    connect(thread, &MyThread::SendKeys, this,
            [=](QString message) { ui->textBrowser->append(message); });
    connect(thread, &MyThread::finished, this, [=](QString message) {
      Iid = message;
      iidObject["iids"] = Iid;
      ui->textBrowser->append("安装ID: " + Iid);
      auto Json = QJsonDocument(iidObject).toJson(QJsonDocument::Compact);
      QNetworkRequest request(QUrl("https://khoatoantin.com/ajax/cidms_api"));
      request.setHeader(QNetworkRequest::ContentTypeHeader,
                        QVariant("application/json"));
      iidManager->post(request, Json);
    });
  });
  connect(this, &Widget::SendCiD, this, [=](QString cid) {
    QProcess *process = new QProcess(this);
    process->start("cmd",
                   QStringList()
                       << "/c"
                       << "cscript //nologo c:\\windows\\system32\\slmgr.vbs"
                       << "/atp" << cid);
    process->waitForStarted();
    process->waitForFinished();
    auto mag = QString::fromLocal8Bit(process->readAllStandardOutput());
    ui->textBrowser->append(mag);
    emit SendActivation(mag);
  });
  connect(this, &Widget::SendActivation, this, [=](QString Activation) {
    QProcess *process = new QProcess(this);
    process->start("cmd",
                   QStringList()
                       << "/c"
                       << "cscript //nologo c:\\windows\\system32\\slmgr.vbs"
                       << "/xpr");
    process->waitForStarted();
    process->waitForFinished();
    auto mag = QString::fromLocal8Bit(process->readAllStandardOutput());
    ui->textBrowser->append(mag);
  });
}

Widget::~Widget() { delete ui; }
