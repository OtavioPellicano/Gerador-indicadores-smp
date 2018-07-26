#include "regrasdescarte.h"
#include "ui_regrasdescarte.h"

RegrasDescarte::RegrasDescarte(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegrasDescarte)
{
    ui->setupUi(this);
}

RegrasDescarte::~RegrasDescarte()
{
    delete ui;
}
