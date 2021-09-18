#ifndef CURLUTIL_H
#define CURLUTIL_H

#include <iostream>
#include <string>
#include <list>
#include <curl/curl.h>
#include <codecutil.h>
#include <QDebug>
#include <QUrl>

namespace Util {

namespace CurlUtil {

size_t curlSaveresToStdString(void *contents, size_t size, size_t nmemb, std::string *s);

/** post
 * @brief 执行 HTTP POST 请求
 * @param url  请求的 URL
 * @param file 需要上传的文件
 * @param data 请求的参数
 * @param is_json_body 如果为 true，则请求的参数是 JSON 格式，否则为 Form 表单的格式 key1=value1&key2=value2&...
 * @param code 请求返回的状态码的指针
 * @param headers 请求头
 * @return 请求执行成功时返回响应的字符串，失败则返回空字符串，请求是否执行成功可以通过 code 进行判断
 */
std::string post(const char* url,
                 std::list<const char*> headers = std::list<const char*>(),
                 const char* file = NULL,
                 const char* data = NULL,
                 bool is_json_body = true,
                 CURLcode *code = NULL);

/** post
 * @brief 执行 HTTP POST 请求
 * @param url  请求的 URL
 * @param token 验证
 * @param image 图片
 * @param code 请求返回的状态码的指针
 * @return 请求执行成功时返回响应的字符串，失败则返回空字符串，请求是否执行成功可以通过 code 进行判断
 */
std::string post(std::string url,
                 std::string token,
                 QPixmap& image,
                 CURLcode *code = NULL);

/** post
 * @brief 执行 HTTP POST 请求
 * @param url  请求的 URL
 * @param api_key 百度OCRApi
 * @param secret_key 图片
 * @param code 请求返回的状态码的指针
 * @return 请求执行成功时返回响应的字符串，失败则返回空字符串，请求是否执行成功可以通过 code 进行判断
 */
std::string post(std::string url, std::string api_key, std::string secret_key, CURLcode *code);

}

}

#endif // CURLUTIL_H
