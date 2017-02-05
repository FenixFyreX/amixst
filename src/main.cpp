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

#include "amixst.h"
#include "runguard.h"

bool AnyChildActive(QObject* parent)
{
    QWidget* elt;
    QObject* obj;

    if((parent == nullptr) || parent->children().empty())
        return false;

    foreach(obj, parent->children())
    {
        if(obj ==nullptr) continue;
        elt = dynamic_cast<QWidget*>(obj);
        if((elt != nullptr) && elt->hasFocus())
            return true;
        if(AnyChildActive(obj))
            return true;
    }

    return false;
}

int main(int argc, char *argv[])
{
    RunGuard lock("amixst");
    if(!lock.TryLock())
    {
        qDebug() << "AMixST is already running.";
        exit(1);
    }

    AMixST::App a(argc, argv);
    return a.exec();
}
