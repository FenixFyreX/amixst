#ifndef RUNGUARD_H
#define RUNGUARD_H

#include <QString>
#include <QSharedMemory>
#include <QSystemSemaphore>

class RunGuard
{
public:
    RunGuard(const QString& key);
    ~RunGuard();
    bool Locked();
    bool TryLock();
    void Unlock();

private:
    QString GenHash(const QString& str, const QString& salt);

    const QString    key;
    const QString    memLockKey;
    const QString    sharedMemKey;
    QSharedMemory    sharedMem;
    QSystemSemaphore memLock;

    Q_DISABLE_COPY ( RunGuard )
};

#endif // RUNGUARD_H
