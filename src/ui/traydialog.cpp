/* amixst - AlsaMixer Sound Tool
 * Copyright (c) 2017 Chaz Domerese
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
