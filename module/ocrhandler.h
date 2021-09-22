#ifndef OCRHANDLER_H
#define OCRHANDLER_H

#include <list>
#include <QString>
#include "constants.h"
#include "../utils/curlutil.h"
#include "../utils/jsonutil.h"
#include <QDebug>
#include <QPixmap>

struct OCRWords{
    QString words = "";
    int top = 0, left = 0;
    int width = 0, height = 0;
    OCRWords(){}
    OCRWords(QString, int, int, int, int);
};

struct OCRInfoPack {
    std::list<OCRWords> ocr_result;
    QPixmap* src = NULL;
    OCRInfoPack(const Json::Value& root, QPixmap* pixmap);
    OCRInfoPack();
};

class OCRHandler
{
public:
    OCRHandler(const QString& app_token = "");

    OCRHandler(const QString& app_id, const QString& app_key);

    OCRInfoPack handle(QPixmap& pixmap);

    void setAppToken(const QString& app_token = "");

    void setAppTokenbyKey(const QString& api_key, const QString& sec_key);

    bool available();

private:
    QString m_app_token;
};

#endif // OCRHANDLER_H
