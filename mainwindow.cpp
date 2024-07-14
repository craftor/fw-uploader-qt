#include "mainwindow.h"
#include "ui_mainwindow.h"

QQueue<LogElement> log_txt;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  dlgAbout = new DialogAbout();
  fwManager = new FirmwareHandler();

  initMsgPrinter();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_actionAbout_triggered() { dlgAbout->show(); }

void MainWindow::on_pushButton_OpenFile_clicked() {}

void MainWindow::on_pushButton_Upload_clicked() {}

void MainWindow::on_pushButton_RefreshFw_clicked() {

  QVector<FirmwareHandler::FirmwareData> fwData =
      fwManager->getAllFirmwareInfos("http://localhost:20000/firmware");

  qDebug() << fwData.length();
}

void MainWindow::initMsgPrinter() {

  // 启动定时器
  m_msgTimer = new QTimer(this);
  m_msgTimer->setInterval(100);
  connect(m_msgTimer, &QTimer::timeout, this, &MainWindow::msgTimerTimeout);
  m_msgTimer->start();
}

void MainWindow::msgTimerTimeout() {
  QTextCharFormat tcf;
  while (log_txt.count()) {
    LogElement log_element = log_txt.dequeue();
    switch (log_element.type) {
    case QtMsgType::QtInfoMsg:
      tcf.setForeground(Qt::blue);
      break;
    case QtMsgType::QtWarningMsg:
      tcf.setForeground(Qt::red);
      break;
    default:
      tcf.setForeground(Qt::black);
      break;
    }
    //        m_msgPrinter->moveCursor(QTextCursor::End);
    ui->textEdit_Log->textCursor().insertText(log_element.txt, tcf);
  }
}
