/******************************************************************************
 *  @file     firmwarehandler.h                                               *
 *  @brief    固件的一些基本操作（上传、查询、删除、更新） *
 *                                                                            *
 *  @author   Craftor                                                         *
 *  @email    craftor@126.com                                                 *
 *  @version  0.0.1                                                           *
 *  @date     2024.7.13                                                       *
 *  @license  GNU General Public License (GPL)                                *
 *                                                                            *
 *----------------------------------------------------------------------------*
 *  Remark         : Description                                              *
 *----------------------------------------------------------------------------*
 *  Change History :                                                          *
 *  <Date>     | <Version> | <Author>       | <Description>                   *
 *----------------------------------------------------------------------------*
 *  2024/07/13 | 0.0.1     | Craftor        | Create file                     *
 *----------------------------------------------------------------------------*
 *                                                                            *
 *****************************************************************************/

#ifndef FIRMWAREHANDLER_H
#define FIRMWAREHANDLER_H

#include <QDateTime>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkCookie>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <QVector>

class FirmwareHandler : public QObject {
  Q_OBJECT
public:
  explicit FirmwareHandler(QObject *parent = nullptr);

  struct FirmwareVersion {
    int m;
    int n;
    int l;
  };

  struct FirmwareData {
    int id;
    int fwcode;
    int version_m;
    int version_n;
    int version_l;
    int fwsize;
    QByteArray fwdata;
    QDateTime created_at;
    QDateTime updated_at;
  };

  struct NewFirmwareData {
    int fwcode;
    int version_m;
    int version_n;
    int version_l;
    int fwsize;
    QByteArray fwdata;
  };

  struct UpdateFirmwareData {
    int fwcode;
    int version_m;
    int version_n;
    int version_l;
    int fwsize;
    QByteArray fwdata;
    QDateTime updated_at;
  };

  QString vecToBase64String(const QVector<quint8> &data);

  QVector<FirmwareData> getAllFirmwareInfos(const QList<QNetworkCookie> cookies,
                                            const QString &server);

  void updateFirmware(const QList<QNetworkCookie> cookies,
                      const QString &server, int id,
                      const UpdateFirmwareData &updatedFw);
  void pushNewFirmware(const QList<QNetworkCookie> cookies,
                       const QString &server, const NewFirmwareData &newFw);
  void deleteFirmwareById(const QList<QNetworkCookie> cookies,
                          const QString &server, int id);

  QString cmdUploadFw(const QList<QNetworkCookie> cookies,
                      const QString &server, const QString &code,
                      const QString &version, const QString &fdata, int fsize);

  QVector<FirmwareData> findFirmware(const QVector<FirmwareData> &allFwFiles,
                                     int code, const FirmwareVersion &version);
};

#endif // FIRMWAREHANDLER_H
