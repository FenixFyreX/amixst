#ifndef SYSICON_H
#define SYSICON_H

#include <QSystemTrayIcon>
#include <QWheelEvent>
#include <QMoveEvent>

namespace AMixST {

class SysIcon : public QSystemTrayIcon
{
    Q_OBJECT
public:
    explicit SysIcon(QObject* parent = 0);

signals:
    void onWheel(int dir);
    void onMove();

protected:
    void wheelEvent(QWheelEvent* event);
    void moveEvent(QMoveEvent* event);

private:
    int wheelData;
};

} // namespace AMixST

#endif // SYSICON_H
