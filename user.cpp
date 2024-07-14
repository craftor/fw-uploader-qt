#include "user.h"

User::User(QObject *parent, QString baseUrl) : QObject{parent} {
  m_baseUrl = baseUrl;
}

bool User::regUser(const QString email, const QString username,
                   const QString password, const QString passwordConfirm) {

  QNetworkAccessManager manager;
  QNetworkRequest request(QUrl(m_baseUrl + "/user/auth/register"));
  request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

  QJsonDocument doc;
  QJsonObject obj;
  obj["email"] = email;
  obj["username"] = username;
  obj["password"] = password;
  obj["passwordConfirm"] = passwordConfirm;

  doc.setObject(obj);
  QByteArray data = doc.toJson();

  QNetworkReply *reply = manager.post(request, data);
  QEventLoop loop;
  QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
  loop.exec();

  bool result;

  if (reply->error() == QNetworkReply::NoError) {
    qDebug() << "User Registration successfully";
    result = true;
  } else {
    qDebug() << "Failed to register user : " << reply->errorString();
    result = false;
  }

  reply->deleteLater();

  return result;
}

bool User::login(const QString email, const QString password) {

  QNetworkAccessManager manager;
  QNetworkCookieJar cookieJar;
  manager.setCookieJar(&cookieJar);
  QUrl loginUrl = QUrl(m_baseUrl + "/user/auth/login");
  QNetworkRequest request(loginUrl);
  request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

  QJsonDocument doc;
  QJsonObject obj;
  obj["email"] = email;
  obj["password"] = password;

  doc.setObject(obj);
  QByteArray data = doc.toJson();

  QNetworkReply *reply = manager.post(request, data);
  QEventLoop loop;
  QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
  loop.exec();

  bool result;

  if (reply->error() == QNetworkReply::NoError) {
    cookies = cookieJar.cookiesForUrl(loginUrl);
    qDebug() << "User Login successfully";
    result = true;
  } else {
    qDebug() << "Login failed : " << reply->errorString();
    result = false;
  }

  reply->deleteLater();

  return result;
}

void User::logout() {
  QNetworkAccessManager manager;
  QNetworkRequest request(QUrl(m_baseUrl + "/user/auth/logout"));
  request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

  QNetworkReply *reply = manager.get(request);
  QEventLoop loop;
  QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
  loop.exec();

  if (reply->error() == QNetworkReply::NoError) {
    qDebug() << "User Logout successfully";
  } else {
    qDebug() << "Logout failed : " << reply->errorString();
  }

  reply->deleteLater();
}

QList<QNetworkCookie> User::getCookies() { return cookies; }
