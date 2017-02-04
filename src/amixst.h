#ifndef AMIXST_H
#define AMIXST_H

#include <QDebug>
#include "app.h"

#define app (static_cast<App*>(QCoreApplication::instance()))

bool AnyChildActive(QObject* obj);

#endif // AMIXST_H
