#ifndef TRAYMENU_H
#define TRAYMENU_H

#include <QMenu>

namespace Ui {
class TrayMenu;
}

class TrayMenu : public QMenu
{
    Q_OBJECT

public:
    explicit TrayMenu(QWidget *parent = 0);
    ~TrayMenu();

private:
    Ui::TrayMenu *ui;
};

#endif // TRAYMENU_H
