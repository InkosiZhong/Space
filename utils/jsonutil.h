#ifndef JSONUTIL_H
#define JSONUTIL_H

#include <string>
#include "json/json.h"

namespace Util {

class JsonUtil {
public:
    JsonUtil(std::string str = "");

public:
    bool parseJson(std::string str);
    const Json::Value& getJson();

private:
    Json::Value m_json;
};

}

#endif
