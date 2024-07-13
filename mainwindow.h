#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QTimer>
#include <QtConcurrent/QtConcurrent>
#include <dialogabout.h>

#include <firmwarehandler.h>

typedef struct {
  QtMsgType type;
  QString txt;
} LogElement;
extern QQueue<LogElement> log_txt;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

  void initMsgPrinter(); // 日志打印重定向

private slots:
  void msgTimerTimeout();

  void on_actionAbout_triggered();

  void on_pushButton_OpenFile_clicked();

  void on_pushButton_Upload_clicked();

  void on_pushButton_RefreshFw_clicked();

private:
  Ui::MainWindow *ui;

  DialogAbout *dlgAbout;

  FirmwareHandler *fwManager;

  QTimer *m_msgTimer; // 日志打印
};
#endif // MAINWINDOW_H
