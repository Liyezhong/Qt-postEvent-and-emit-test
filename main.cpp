#include <QtCore>
#include "myobject.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qDebug()<<"From main thread: "<<QThread::currentThreadId();
    MyThread t;
    t.startWorker();
    QThread::msleep(4500);
    t.start();
    QCoreApplication::postEvent(&(t.worker),new QEvent((QEvent::Type)2333));
    while(1);
    return a.exec();
}
