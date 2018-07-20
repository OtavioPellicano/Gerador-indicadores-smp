#include "detalheindicador.h"
#include "ui_detalheindicador.h"

DetalheIndicador::DetalheIndicador(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DetalheIndicador)
{
    ui->setupUi(this);
}

DetalheIndicador::~DetalheIndicador()
{
    delete ui;
}
