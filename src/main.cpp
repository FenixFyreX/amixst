
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
