#include "firmwarehandler.h"
#include "qeventloop.h"

FirmwareHandler::FirmwareHandler(QObject *parent) : QObject{parent} {}

/**
 * @brief QVector<quint8>转Base64格式字符串
 * @param dataVecotr 输入参数
 *
 * @return 返回base64格式字符串
 */
QString FirmwareHandler::vecToBase64String(const QVector<quint8> &dataVector) {
  QByteArray byteArray = QByteArray::fromRawData(
      reinterpret_cast<const char *>(dataVector.data()), dataVector.size());
  QString base64String = byteArray.toBase64();
  return base64String;
}

QVector<FirmwareHandler::FirmwareData>
FirmwareHandler::getAllFirmwareInfos(const QList<QNetworkCookie> cookies,
                                     const QString &server) {
  QVector<FirmwareData> allFwDatas;

  QNetworkAccessManager manager;
  QNetworkRequest request((QUrl(server)));

  // 设置请求类型
  request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

  // 设置 cookies
  foreach (const QNetworkCookie &cookie, cookies) {
    request.setHeader(QNetworkRequest::CookieHeader,
                      QVariant::fromValue(cookie));
  }

  QNetworkReply *reply = manager.get(request);

  QEventLoop loop;
  QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
  loop.exec();

  if (reply->error() == QNetworkReply::NoError) {
    QByteArray responseData = reply->readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
    QJsonArray jsonArray = jsonDoc.array();

    for (const QJsonValue &value : jsonArray) {
      QJsonObject obj = value.toObject();
      FirmwareData data;
      data.id = obj["id"].toInt();
      data.fwcode = obj["fwcode"].toInt();
      data.version_m = obj["version_m"].toInt();
      data.version_n = obj["version_n"].toInt();
      data.version_l = obj["version_l"].toInt();
      data.fwsize = obj["fwsize"].toInt();
      data.created_at =
          QDateTime::fromString(obj["created_at"].toString(), Qt::ISODate);
      data.updated_at =
          QDateTime::fromString(obj["updated_at"].toString(), Qt::ISODate);
      allFwDatas.append(data);
    }
  } else {
    qDebug() << "Failed to get firmware: " << reply->errorString();
  }

  reply->deleteLater();
  return allFwDatas;
}

/**
 * @brief           更新服务器上的固件
 * @param server    服务器地址
 * @param id        固件id号
 * @param updatedFw 待更新的固件
 *
 * @return void
 */
void FirmwareHandler::updateFirmware(const QList<QNetworkCookie> cookies,
                                     const QString &server, int id,
                                     const UpdateFirmwareData &updatedFw) {
  QNetworkAccessManager manager;
  QNetworkRequest request(QUrl(server + "/" + QString::number(id)));

  // 设置请求类型
  request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

  // 设置 cookies
  foreach (const QNetworkCookie &cookie, cookies) {
    request.setHeader(QNetworkRequest::CookieHeader,
                      QVariant::fromValue(cookie));
  }

  QJsonDocument doc;
  QJsonObject obj;
  obj["fwcode"] = updatedFw.fwcode;
  obj["version_m"] = updatedFw.version_m;
  obj["version_n"] = updatedFw.version_n;
  obj["version_l"] = updatedFw.version_l;
  obj["fwsize"] = updatedFw.fwsize;
  obj["fwdata"] = QString::fromUtf8(updatedFw.fwdata);
  obj["updated_at"] = updatedFw.updated_at.toString(Qt::ISODate);

  doc.setObject(obj);
  QByteArray data = doc.toJson();

  QNetworkReply *reply = manager.put(request, data);
  QEventLoop loop;
  QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
  loop.exec();

  if (reply->error() == QNetworkReply::NoError) {
    qDebug() << "Firmware updated successfully";
  } else {
    qDebug() << "Failed to update firmware: " << reply->errorString();
  }

  reply->deleteLater();
}

/**
 * @brief           上传固件到服务器
 * @param server    服务器地址
 * @param newData   待上传的固件
 *
 * @return void
 */
void FirmwareHandler::pushNewFirmware(const QList<QNetworkCookie> cookies,
                                      const QString &server,
                                      const NewFirmwareData &newData) {
  QNetworkAccessManager manager;
  QNetworkRequest request((QUrl(server)));

  // 设置请求类型
  request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

  // 设置 cookies
  foreach (const QNetworkCookie &cookie, cookies) {
    request.setHeader(QNetworkRequest::CookieHeader,
                      QVariant::fromValue(cookie));
  }

  QJsonDocument doc;
  QJsonObject obj;
  obj["fwcode"] = newData.fwcode;
  obj["version_m"] = newData.version_m;
  obj["version_n"] = newData.version_n;
  obj["version_l"] = newData.version_l;
  obj["fwsize"] = newData.fwsize;
  obj["fwdata"] = QString::fromUtf8(newData.fwdata);

  doc.setObject(obj);
  QByteArray data = doc.toJson();

  QNetworkReply *reply = manager.post(request, data);
  QEventLoop loop;
  QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
  loop.exec();

  if (reply->error() == QNetworkReply::NoError) {
    qDebug() << "Firmware uploaded successfully";
  } else {
    qDebug() << "Failed to upload firmware: " << reply->errorString();
  }

  reply->deleteLater();
}

/**
 * @brief           根据id删除服务器上的固件
 * @param server    服务器地址
 * @param id        固件id
 *
 * @return void
 */
void FirmwareHandler::deleteFirmwareById(const QList<QNetworkCookie> cookies,
                                         const QString &server, int id) {
  QNetworkAccessManager manager;
  QNetworkRequest request(QUrl(server + "/" + QString::number(id)));

  // 设置请求类型
  request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

  // 设置 cookies
  foreach (const QNetworkCookie &cookie, cookies) {
    request.setHeader(QNetworkRequest::CookieHeader,
                      QVariant::fromValue(cookie));
  }

  QNetworkReply *reply = manager.deleteResource(request);
  QEventLoop loop;
  QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
  loop.exec();

  if (reply->error() == QNetworkReply::NoError) {
    qDebug() << "Firmware deleted successfully";
  } else {
    qDebug() << "Failed to delete firmware: " << reply->errorString();
  }

  reply->deleteLater();
}

/**
 * @brief           一条上传固件的命令
 * @param server    服务器地址
 * @param code      固件code
 * @param version   固件version
 * @param fdata     固件base64格式的内容
 * @param fsize     固件大小
 *
 * @return QString  返回上传成功或失败的消息
 */
QString FirmwareHandler::cmdUploadFw(const QList<QNetworkCookie> cookies,
                                     const QString &server, const QString &code,
                                     const QString &version,
                                     const QString &fdata, int fsize) {
  bool ok;
  int fwcode = code.toInt(&ok, 16);
  if (!ok) {
    return "Wrong Code!";
  }

  QStringList parts = version.split('.');
  int version_m = parts[0].toInt(&ok);
  int version_n = parts[1].toInt(&ok);
  int version_l = parts[2].toInt(&ok);

  QStringList fdataSlice = fdata.split(",");
  QByteArray fwData = fdataSlice[1].toUtf8().toBase64();

  FirmwareVersion versionData = {version_m, version_n, version_l};
  NewFirmwareData newData = {fwcode,    version_m, version_n,
                             version_l, fsize,     fwData};
  UpdateFirmwareData updatedFw = {fwcode,
                                  version_m,
                                  version_n,
                                  version_l,
                                  fsize,
                                  fwData,
                                  QDateTime::currentDateTime()};

  // Assume these functions exist with similar functionality in your Qt
  // application
  QVector<FirmwareData> allFwFiles = getAllFirmwareInfos(cookies, server);
  QVector<FirmwareData> oldFw = findFirmware(allFwFiles, fwcode, versionData);

  if (oldFw.length() > 0) {
    // If firmware exists, update firmware
    QString msg = QString("Firmware existed, updating... ") +
                  QString::number(updatedFw.fwcode);
    updateFirmware(cookies, server, oldFw[0].id, updatedFw);
    qDebug() << msg;
    return msg;
  } else {
    // If firmware doesn't exist, upload new firmware
    QString msg =
        QString("Upload a new firmware -> ") + QString::number(newData.fwcode);
    pushNewFirmware(cookies, server, newData);
    qDebug() << msg;
    return msg;
  }
}

/**
 * @brief            从固件清单中根据code和version查询相应的固件
 * @param allFwFiles 固件清单
 * @param code       固件code
 * @param version    固件version
 *
 * @return QVector<FirmwareData>  返回查询到的固件列表，可能长度为0。
 */
QVector<FirmwareHandler::FirmwareData>
FirmwareHandler::findFirmware(const QVector<FirmwareData> &allFwFiles, int code,
                              const FirmwareVersion &version) {

  QVector<FirmwareHandler::FirmwareData> result;

  for (const FirmwareData &firmware : allFwFiles) {
    if (firmware.fwcode == code && firmware.version_m == version.m &&
        firmware.version_n == version.n && firmware.version_l == version.l) {
      // return firmware;
      result.append(firmware);
    }
  }

  return result;
}
