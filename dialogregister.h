#ifndef DIALOGREGISTER_H
#define DIALOGREGISTER_H

#include <QDialog>

namespace Ui {
class DialogRegister;
}

class DialogRegister : public QDialog {
  Q_OBJECT

public:
  explicit DialogRegister(QWidget *parent = nullptr);
  ~DialogRegister();

private slots:
  void on_pushButton_Register_clicked();

private:
  Ui::DialogRegister *ui;
};

#endif // DIALOGREGISTER_H
