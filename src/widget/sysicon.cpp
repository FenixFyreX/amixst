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

#include "widget/sysicon.h"

namespace AMixST {

SysIcon::SysIcon(QObject* parent) : QSystemTrayIcon(parent)
{
    wheelData = 0;
}

void SysIcon::wheelEvent(QWheelEvent *event)
{
    int dir;
    bool accept = false;
    QPoint numDegrees = event->angleDelta() / 8;
    if(!numDegrees.isNull())
    {
        QPoint numSteps = numDegrees / 15;
        wheelData += numSteps.y();

        dir = wheelData < 0 ? -1 : 1;
        while(abs(wheelData) >= 120)
        {
            accept = true;
            emit onWheel(dir);
            wheelData += 120 * dir;
        }
    }

    if(accept) event->accept();
    else event->ignore();
}

void SysIcon::moveEvent(QMoveEvent *event)
{
    emit onMove();
    event->accept();
}

} // namespace AMixST
