#include "networkutil.h"

Util::NetworkUtil::NetworkUtil(QObject *parent) : QObject(parent)
{
    m_worker = new Worker("https://www.baidu.com", 1, 1);
    m_worker->moveToThread(&worker_thread);
    connect(this, SIGNAL(signalCheckHostOnline()), m_worker, SLOT(onCheckHostOnline()));
    connect(m_worker, SIGNAL(signalConnState(bool)), this, SLOT(onConnStateChanged(bool)));
    connect(&worker_thread, &QThread::finished, &worker_thread, &QObject::deleteLater);
    worker_thread.start();
    emit signalCheckHostOnline();
}

Util::NetworkUtil::~NetworkUtil(){
    //emit signalStopCheckHostOnline();
    m_worker->stopCheckHostOnline();
    worker_thread.quit();
    worker_thread.wait();
    if (m_worker){
        m_worker->deleteLater();
    }
}

bool Util::NetworkUtil::isConnecting(){
    return m_conn;
}

void Util::NetworkUtil::onConnStateChanged(bool connecting){
    if (m_conn == !connecting){
        emit signalConnStateChanged(connecting);
    }
    m_conn = connecting;
}

Util::Worker::Worker(QString host, long timeout, long cycle_t):
    m_host(host), m_timeout(timeout), m_cycle_t(cycle_t) {}

void Util::Worker::onCheckHostOnline(){
    std::string res;
    m_checking = true;
    CURL *curl = NULL;
    while(m_checking) {
        curl = curl_easy_init();
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, m_host.toStdString().data());
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Util::CurlUtil::curlSaveresToStdString);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &res);
            curl_easy_setopt( curl, CURLOPT_TIMEOUT, m_timeout);
            //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
            CURLcode curl_code = curl_easy_perform(curl);
            if (curl_code != CURLE_OK) {
                qDebug() << "curl_easy_perform() failed: " << curl_easy_strerror(curl_code);
                emit signalConnState(false);
            } else {
                emit signalConnState(!res.empty());
            }
            curl_easy_cleanup(curl);
        }
        QThread::sleep(m_cycle_t);
    }
}

void Util::Worker::stopCheckHostOnline(){
    m_checking = false;
}
