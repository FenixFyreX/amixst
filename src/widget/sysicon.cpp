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
