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
