#ifndef USER_H
#define USER_H

#include <QDateTime>
#include <QDebug>
#include <QEventLoop>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkCookie>
#include <QNetworkCookieJar>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <QUrl>
#include <QVector>

class User : public QObject {
  Q_OBJECT
public:
  explicit User(QObject *parent = nullptr,
                QString baseUrl = "http://localhost:20000");

  bool regUser(const QString email, const QString username,
               const QString password, const QString passwordConfirm);
  bool login(const QString email, const QString password);
  void logout();

  QList<QNetworkCookie> getCookies();

private:
  QString m_baseUrl;
  QList<QNetworkCookie> cookies;
};

#endif // USER_H
