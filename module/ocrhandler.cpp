#include "ocrhandler.h"

OCRWords::OCRWords(QString wd, int t, int l, int w, int h){
    words = wd;
    top = t;
    left = l;
    width = w;
    height = h;
}

OCRInfoPack::OCRInfoPack(){}

OCRInfoPack::OCRInfoPack(const Json::Value& root){
    int num = root["words_result_num"].asInt();
    for (int i = 0; i < num; i++){
        Json::Value val = root["words_result"][i];
        qDebug() << QString::fromStdString(val["words"].asString());
        OCRWords ocr_words = {
            QString::fromStdString(val["words"].asString()),
            val["location"]["top"].asInt(),
            val["location"]["left"].asInt(),
            val["location"]["width"].asInt(),
            val["location"]["height"].asInt(),
        };
        ocr_result.push_back(ocr_words);
    }
}

OCRHandler::OCRHandler(const QString& app_token){
    setAppToken(app_token);
}

OCRHandler::OCRHandler(const QString& app_id, const QString& app_key){
    setAppTokenbyKey(app_id, app_key);
}

OCRInfoPack OCRHandler::handle(QPixmap& pixmap){
    std::string url = "https://aip.baidubce.com/rest/2.0/ocr/v1/accurate";
    std::string ret = Util::CurlUtil::post(url, m_app_token.toStdString(), pixmap);
    Util::JsonUtil json_util(ret);
    const Json::Value& root = json_util.getJson();
    return OCRInfoPack(root);
}

void OCRHandler::setAppToken(const QString& app_token){
    m_app_token = app_token;
}

void OCRHandler::setAppTokenbyKey(const QString& api_key, const QString& sec_key){
    std::string url = "https://aip.baidubce.com/oauth/2.0/token";
    std::string ret = Util::CurlUtil::post(url, api_key.toStdString(), sec_key.toStdString(), NULL);
    Util::JsonUtil json_util(ret);
    const Json::Value& root = json_util.getJson();
    m_app_token = QString::fromStdString(root["access_token"].asString());
}

bool OCRHandler::available(){
    return !m_app_token.isEmpty();
}
