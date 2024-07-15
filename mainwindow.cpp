#include "mainwindow.h"
#include "ui_mainwindow.h"

QQueue<LogElement> log_txt;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  dlgAbout = new DialogAbout();
  fwManager = new FirmwareHandler();

  // 登录窗口
  dlgLogin = new DialogLogin();
  dlgLogin->show();

  connect(dlgLogin, SIGNAL(loginSuccess()), this, SLOT(show()));

  initMsgPrinter();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_actionAbout_triggered() { dlgAbout->show(); }

void MainWindow::on_pushButton_OpenFile_clicked() {
  QString file_full;
  QFileInfo file_info;

  file_full = QFileDialog::getOpenFileName(this, ("Open"), "",
                                           "(*.bin;*.hex;*.elf)", nullptr);
  file_info = QFileInfo(file_full);

  if (!file_full.isNull()) {
    ui->lineEdit_file->setText(file_full);
    ui->lineEdit_FileSize->setText(QString::number(file_info.size()));
  }
}

void MainWindow::on_pushButton_Upload_clicked() {
  QString code = ui->lineEdit_code->text();
  QString version = ui->lineEdit_version->text();
  QString filePath = ui->lineEdit_file->text();

  QFileInfo fileInfo(filePath);
  QString fdata = fwManager->readAndEncodeFileToBase64(filePath);

  QString result = fwManager->cmdUploadFw(
      dlgLogin->m_user->getCookies(), "http://localhost:20000/firmware", code,
      version, fdata, fileInfo.size());
  qDebug() << result;
}

void MainWindow::on_pushButton_RefreshFw_clicked() {

  QVector<FirmwareHandler::FirmwareData> fwData =
      fwManager->getAllFirmwareInfos(dlgLogin->m_user->getCookies(),
                                     "http://localhost:20000/firmware");

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

void MainWindow::on_pushButton_ClearLog_clicked() {
  ui->textEdit_Log->setText("");
}
