#ifndef MYOBJECT_H
#define MYOBJECT_H

#include <QObject>
#include <iostream>
#include <QtCore>
using namespace std;
class Worker : public QObject
{
    Q_OBJECT
public:
    Worker(){
        num=0;
    }

    void operate(){
        QEvent *ev=new QEvent((QEvent::Type)2333);
        QCoreApplication::postEvent(this,ev);
    }

    void work(){
        for(int i=0;i<10;i++){
            emit Message();
            QThread::msleep(1500);
        }
    }
signals:
    void Message();
private:
    bool event(QEvent *event){
        num++;
        qDebug()<<"Event #"<<num<<":"<<event->type();
        if(event->type()==(QEvent::Type)2333){
            onTimeout();
            return true;
        }
        return QObject::event(event);
    }
    int num;

private slots:
    void onTimeout()
    {
        qDebug()<<"Worker::onTimeout get called from?: "<<QThread::currentThreadId();
    }
};
class MyThread:public QThread
{
    Q_OBJECT
public:
    MyThread(){
        connect(&worker,&Worker::Message,this,&MyThread::post);
        connect(&timer,&QTimer::timeout,this,&MyThread::post);
        connect(this,&MyThread::begin,&worker,&Worker::work);
        timer.start(500);
        moveToThread(this);
        worker.moveToThread(this);
        timer.moveToThread(this);
    }
    Worker worker;
    void post(){
        qDebug()<<"Message in"<<currentThreadId();
    }

    void run(){
        this->exec();
    }
    void startWorker(){
        emit begin();
    }
    QTimer timer;
signals:
    void begin();
};

#endif // MYOBJECT_H
