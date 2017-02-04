#include "runguard.h"
#include <QByteArray>
#include <QCryptographicHash>

QString RunGuard::GenHash(const QString &str, const QString &salt)
{
    QByteArray data;

    data.append(str.toUtf8());
    data.append(salt.toUtf8());
    data = QCryptographicHash::hash(data, QCryptographicHash::Sha1).toHex();

    return data;
}

RunGuard::RunGuard(const QString& key)
    : key(key)
    , memLockKey(GenHash(key, "memLockKey"))
    , sharedMemKey(GenHash(key, "sharedMemKey"))
    , sharedMem(sharedMemKey)
    , memLock(memLockKey, 1)
{
    memLock.acquire();
    {
        QSharedMemory fix(sharedMemKey);
        fix.attach();
    }
    memLock.release();
}

RunGuard::~RunGuard()
{
    Unlock();
}

bool RunGuard::Locked()
{
    if(sharedMem.isAttached())
        return false;

    memLock.acquire();
    const bool isRunning = sharedMem.attach();
    if(isRunning)
        sharedMem.detach();
    memLock.release();

    return isRunning;
}

bool RunGuard::TryLock()
{
    if(Locked())
        return false;

    memLock.acquire();
    const bool result = sharedMem.create(sizeof(quint64));
    memLock.release();
    if(!result)
    {
        Unlock();
        return false;
    }

    return true;
}

void RunGuard::Unlock()
{
    memLock.acquire();
    if(sharedMem.isAttached())
        sharedMem.detach();
    memLock.release();
}
