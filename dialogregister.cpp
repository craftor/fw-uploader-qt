#include "dialogregister.h"
#include "ui_dialogregister.h"

DialogRegister::DialogRegister(QWidget *parent)
    : QDialog(parent), ui(new Ui::DialogRegister) {
  ui->setupUi(this);
}

DialogRegister::~DialogRegister() { delete ui; }

void DialogRegister::on_pushButton_Register_clicked() {}
