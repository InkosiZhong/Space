#ifndef NETWORKUTIL_H
#define NETWORKUTIL_H

#include <QObject>
#include <utils/curlutil.h>
#include <QString>
#include <QThread>
#include <QDebug>

namespace Util {

class Worker: public QObject
{
    Q_OBJECT
public:
    Worker(QString host = "https://www.baidu.com", long timeout = 1, long cycle_t = 3);

public slots:
    void onCheckHostOnline();

signals:
    void signalConnStateChanged(bool);

private:
    QString m_host;
    long m_timeout = 1;
    long m_cycle_t = 3;
};

class NetworkUtil : public QObject
{
    Q_OBJECT
    QThread worker_thread;
public:
    explicit NetworkUtil(QObject *parent = nullptr);

    ~NetworkUtil();

    bool isConnecting();

signals:
    void signalCheckHostOnline();
    void signalConnStateChanged(bool);

private:

private slots:
    void onConnStateChanged(bool);

private:
    bool m_conn;
    Worker* m_worker;

signals:

};

}

#endif // NETWORKUTIL_H
