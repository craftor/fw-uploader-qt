#include "dialoglogin.h"
#include "ui_dialoglogin.h"

DialogLogin::DialogLogin(QWidget *parent)
    : QDialog(parent), ui(new Ui::DialogLogin) {
  ui->setupUi(this);
  m_user = new User();
}

DialogLogin::~DialogLogin() { delete ui; }

void DialogLogin::on_pushButton_Login_clicked() {

  QString email = ui->lineEdit_Email->text();
  QString password = ui->lineEdit_Password->text();

  bool result = m_user->login(email, password);
  if (result) {
    emit(loginSuccess());
    this->close();
  } else {
    QMessageBox::warning(this, tr("错误"), tr("登录失败!"), QMessageBox::Abort);
  }
}

void DialogLogin::on_pushButton_Register_clicked() {
  DialogRegister *m_regUser = new DialogRegister();
  m_regUser->exec();
}
