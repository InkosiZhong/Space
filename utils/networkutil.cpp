#include "networkutil.h"

Util::NetworkUtil::NetworkUtil(QObject *parent) : QObject(parent)
{
    m_worker = new Worker("https://www.baidu.com", 1, 1);
    m_worker->moveToThread(&worker_thread);
    connect(this, SIGNAL(signalCheckHostOnline()), m_worker, SLOT(onCheckHostOnline()));
    connect(m_worker, SIGNAL(signalConnStateChanged(bool)), this, SLOT(onConnStateChanged(bool)));
    connect(&worker_thread, &QThread::finished, m_worker, &QObject::deleteLater);
    worker_thread.start();
    emit signalCheckHostOnline();
}

Util::NetworkUtil::~NetworkUtil(){
    worker_thread.quit();
    worker_thread.wait();
    if (m_worker){
        delete m_worker;
    }
}

bool Util::NetworkUtil::isConnecting(){
    return m_conn;
}

void Util::NetworkUtil::onConnStateChanged(bool connecting){
    m_conn = connecting;
    emit signalConnStateChanged(m_conn);
}

Util::Worker::Worker(QString host, long timeout, long cycle_t):
    m_host(host), m_timeout(timeout), m_cycle_t(cycle_t) {}

void Util::Worker::onCheckHostOnline(){
    std::string res;

    CURL *curl = NULL;
    while(true) {
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
                emit signalConnStateChanged(false);
            } else {
                emit signalConnStateChanged(!res.empty());
            }
            curl_easy_cleanup(curl);
        }
        QThread::sleep(m_cycle_t);
    }
}
