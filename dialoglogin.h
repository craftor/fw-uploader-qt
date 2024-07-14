#ifndef DIALOGLOGIN_H
#define DIALOGLOGIN_H

#include <QDialog>
#include <QMessageBox>
#include <dialogregister.h>
#include <user.h>

namespace Ui {
class DialogLogin;
}

class DialogLogin : public QDialog {
  Q_OBJECT

public:
  explicit DialogLogin(QWidget *parent = nullptr);
  ~DialogLogin();

private slots:
  void on_pushButton_Login_clicked();

  void on_pushButton_Register_clicked();

public:
  User *m_user;

signals:
  void loginSuccess();

private:
  Ui::DialogLogin *ui;
};

#endif // DIALOGLOGIN_H
