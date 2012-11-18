#include "pointadddialog.h"
#include "ui_pointadddialog.h"

PointAddDialog::PointAddDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PointAddDialog)
{
    ui->setupUi(this);
}

PointAddDialog::~PointAddDialog()
{
    delete ui;
}

int PointAddDialog::getClassNumber()
{
    return ui->classSpinBox->value();
}

ClassType PointAddDialog::getClassType()
{
    if(ui->comboBox->currentIndex() == 0 )
        return UNDEFINED;
    else if( ui->comboBox->currentIndex() == 1 )
           return DEFINED;
}
