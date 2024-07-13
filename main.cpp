#include "mainwindow.h"

#include <QApplication>
#include <QDebug>
#include <QKeyEvent>
#include <QLabel>
#include <QMainWindow>
#include <QSplashScreen>
#include <QTextEdit>
#include <QTimer>
#include <QtConcurrent/QtConcurrent>

void LogMessage(QtMsgType type, const QMessageLogContext &context,
                const QString &msg) {
  static QMutex mutex;
  Q_UNUSED(context);
  mutex.lock();
  QString current_date_time =
      QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz");
  QString message = QString("%1  %2 \r\n").arg(current_date_time).arg(msg);
  LogElement log_element = {type, message};
  log_txt.enqueue(log_element);
  mutex.unlock();
}

int main(int argc, char *argv[]) {

  // 添加高分辨率屏幕自适应
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

  QApplication a(argc, argv);

  // 重定向日志
  qInstallMessageHandler(LogMessage);

  QPixmap pix(":/resource/logo_512x512_cryan_r.png");
  QSplashScreen splash(pix);
  splash.show();
  a.processEvents();

  MainWindow w;
  w.show();

  splash.finish(&w); // 程序启动画面结束

  return a.exec();
}
