#include "curlutil.h"

size_t Util::CurlUtil::curlSaveresToStdString(void *contents, size_t size, size_t nmemb, std::string *s) {
    size_t newLength = size * nmemb;
    size_t oldLength = s->size();
    s->resize(oldLength + newLength);
    std::copy((char*)contents, (char*)contents+newLength, s->begin()+oldLength);

    return size * nmemb;
}

std::string Util::CurlUtil::post(const char* url, std::list<const char*> headers, const char* file, const char* data, bool is_json_body, CURLcode *code) {
    std::string res;

    CURL *curl = curl_easy_init();

    if (curl) {
        // Headers
        struct curl_slist *curl_headers = NULL;

        std::list<const char *>::const_iterator iter;
        for (iter = headers.cbegin(); iter != headers.cend(); ++iter) {
            curl_headers = curl_slist_append(curl_headers, *iter);
        }

        if (data != NULL && is_json_body) {
            curl_headers = curl_slist_append(curl_headers, "Accept: application/json; charset=utf-8");
            curl_headers = curl_slist_append(curl_headers, "Content-Type: application/json");
        }

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, curl_headers);
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlSaveresToStdString);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &res);
        curl_easy_setopt(curl, CURLOPT_POST, 1); // POST 请求
        if (data != NULL)curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data); // POST 参数
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt( curl, CURLOPT_TIMEOUT, 10L);
        //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        if (file != NULL) {
            struct curl_httppost* post = NULL;
            struct curl_httppost* last = NULL;
            curl_formadd(&post, &last, CURLFORM_COPYNAME, "file", CURLFORM_FILE, file, CURLFORM_END);
            curl_easy_setopt(curl, CURLOPT_HTTPPOST, post);
        }

        CURLcode curl_code = curl_easy_perform(curl);
        if (code != NULL) {
            *code = curl_code;
        }
        curl_slist_free_all(curl_headers); /* free the header list */
    } else {
        if (code != NULL) {
            *code = CURLE_FAILED_INIT;
        }
    }

    // 释放 curl 资源
    curl_easy_cleanup(curl);

    return res;
}

std::string Util::CurlUtil::post(std::string url, std::string token, QPixmap& image, CURLcode *code) {
    std::string res;

    CURL *curl = NULL;
    url = url +  "?access_token=" + token;
    QString base64;
    if (!Util::CodecUtil::pixmap2Base64(image, base64)){
        qDebug() << "failed to convert.";
        return res;
    }
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.data());
        curl_easy_setopt(curl, CURLOPT_POST, 1);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        base64 = QUrl::toPercentEncoding(base64.simplified());
        std::string urlbase64 = "image=" + base64.toStdString();
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, urlbase64.data());
        struct curl_slist *curl_headers = NULL;
        curl_headers = curl_slist_append(curl_headers, "Content-Type:application/x-www-form-urlencoded");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, curl_headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlSaveresToStdString);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &res);
        curl_easy_setopt( curl, CURLOPT_TIMEOUT, 10L);
        //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        CURLcode curl_code = curl_easy_perform(curl);
        curl_slist_free_all(curl_headers); /* free the header list */
        if (code)*code = curl_code;
        if (curl_code != CURLE_OK) {
            qDebug() << "curl_easy_perform() failed: " << curl_easy_strerror(curl_code);
            return res;
        }
        curl_easy_cleanup(curl);
    }

    return res;
}

std::string Util::CurlUtil::post(std::string url, std::string api_key, std::string secret_key, CURLcode *code){
    std::string res;

    CURL *curl = NULL;
    url = url +  "?grant_type=client_credentials&client_id=" + api_key + "&client_secret=" + secret_key;
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.data());
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlSaveresToStdString);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &res);
        curl_easy_setopt( curl, CURLOPT_TIMEOUT, 10L);
        //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        CURLcode curl_code = curl_easy_perform(curl);
        if (code)*code = curl_code;
        if (curl_code != CURLE_OK) {
            qDebug() << "curl_easy_perform() failed: " << curl_easy_strerror(curl_code);
            return res;
        }
        curl_easy_cleanup(curl);
    }

    return res;
}
