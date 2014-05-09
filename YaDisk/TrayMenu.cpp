#include "TrayMenu.h"
#include "ui_TrayMenu.h"

TrayMenu::TrayMenu(QWidget *parent) :
    QMenu(parent),
    ui(new Ui::TrayMenu)
{
    ui->setupUi(this);
}

TrayMenu::~TrayMenu()
{
    delete ui;
}
