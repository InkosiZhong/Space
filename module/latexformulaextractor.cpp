#include "latexformulaextractor.h"

LatexInfoPack::LatexInfoPack(const Json::Value& root){
    m_type = root["is_printed"].asBool() ? LATEX_PRINTED : root["is_handwritten"].asBool() ? LATEX_HANDWRITED : LATEX_UNKOWN;
    m_rotate_degrees = root["auto_rotate_degrees"].asDouble();
    m_rotate_confidence = root["auto_rotate_confidence"].asDouble();
    if (root["error"].asString() != ""){
        m_confidence = -1;
        m_msg = QString::fromStdString(root["error"].asString());
    }
    else{
        m_confidence = root["confidence"].asDouble();
        m_msg = QString::fromStdString(root["latex_styled"].asString());
    }
}

LatexInfoPack::LatexInfoPack(){}

LatexFormulaExtractor::LatexFormulaExtractor(const QString& app_id, const QString& app_key)
    { setAppIdAndKey(app_id, app_key); }

void LatexFormulaExtractor::setAppIdAndKey(const QString& app_id, const QString& app_key)
    { m_app_id = app_id; m_app_key = app_key; }

LatexInfoPack LatexFormulaExtractor::handle(const QString& path){
    std::string app_id = "app_id:" + m_app_id.toStdString(), app_key = "app_key:" + m_app_key.toStdString();
    std::list<const char*> header = { app_id.c_str(), app_key.c_str() };
    std::string ret = Util::CurlUtil::post("https://api.mathpix.com/v3/text", header, path.toStdString().c_str());
    qDebug() << QString::fromStdString(ret);
    Util::JsonUtil json_util(ret);
    const Json::Value& root = json_util.getJson();
    return LatexInfoPack(root);
}

bool LatexFormulaExtractor::available(){
    return !m_app_id.isEmpty() && !m_app_key.isEmpty();
}
