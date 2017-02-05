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
