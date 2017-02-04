#include "traydialog.h"
#include "ui_traydialog.h"
#include <QScreen>
#include <QDesktopWidget>
#include "amixst.h"

namespace AMixST {

TrayDialog::TrayDialog(QWidget *parent) :
    QDialog(parent, Qt::Popup | Qt::SplashScreen | Qt::FramelessWindowHint),
    ui(new Ui::TrayDialog)
{
    ui->setupUi(this);
    ui->toggle->setEnabled(false);
}

TrayDialog::~TrayDialog()
{
    delete ui;
}

void TrayDialog::show()
{
    Relocate();
    QDialog::show();
    setFocus();
    activateWindow();
}

void TrayDialog::Relocate()
{
    const QRect g = geometry();
    const QRect ig = app->sysTray.geometry();

    int px = ig.x() + ((ig.width() - g.width()) / 2);
    int py;
    if(ig.y() < g.height())
        py = ig.y() + g.height();
    else
        py = ig.y() - g.height();

    int sn = QApplication::desktop()->screenNumber(this);
    const QRect gs = QApplication::desktop()->screenGeometry(sn);
    if(px < gs.x())
        px = gs.x() + g.width();
    else if((px + g.width()) >= gs.right())
        px = gs.right() - g.width();

    if(py < gs.y())
        py = gs.y() + g.height();
    else if((py + g.height()) >= gs.bottom())
        py = gs.bottom() - g.height();

    move(px, py);
}

void TrayDialog::focusOutEvent(QFocusEvent *event)
{
    event->ignore();

    if(!AnyChildActive(this))
        hide();
    else
    {
        setFocus();
        activateWindow();
    }
}

} // namespace AMixST
