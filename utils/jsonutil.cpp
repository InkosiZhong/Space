#include "jsonutil.h"

Util::JsonUtil::JsonUtil(std::string str) {
    parseJson(str);
}

bool Util::JsonUtil::parseJson(std::string str) {
    Json::CharReaderBuilder b;
    Json::CharReader* reader(b.newCharReader());
    JSONCPP_STRING errs;
    bool ret = reader->parse(str.c_str(), str.c_str() + std::strlen(str.c_str()), &m_json, &errs);
    delete reader;
    return ret;
}

const Json::Value& Util::JsonUtil::getJson() {
    return m_json;
}
