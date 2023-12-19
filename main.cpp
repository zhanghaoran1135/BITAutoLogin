#include <iostream>
#include <string>
#include <curl/curl.h>
#include <iomanip>
#include "main.h"
#include "requests.h"
#include "decode.h"
#include "generate.h"
#include "xencode.h"

// URL
const char *init_url = "http://10.0.0.55";
std::string get_challenge_api = "http://10.0.0.55/cgi-bin/get_challenge";
std::string srun_portal_api = "http://10.0.0.55/cgi-bin/srun_portal";
std::string requests_headers = "Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/63.0.3239.26 Safari/537.36";

// Fixed parameters
std::string n = "200";
std::string type = "1";
const char *ac_id = "8";
const char *enc = "srun_bx1";

const char *username = nullptr;
const char *password = nullptr;

std::string urlEncode(const std::string &value) {
    std::ostringstream escaped;
    escaped.fill('0');
    escaped << std::hex;

    for (char c : value) {
        // Keep alphanumeric and other accepted characters unchanged
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            escaped << c;
        } else {
            // Convert character to hex representation
            escaped << std::uppercase;
            escaped << '%' << std::setw(2) << int(static_cast<unsigned char>(c));
            escaped << std::nouppercase;
        }
    }

    return escaped.str();
}

std::string login() {
    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Failed to initialize curl" << std::endl;
        return "";
    }

    // 构建 URL
    std::string url = srun_portal_api + "?callback=jQuery1124007344203445056241_" + std::to_string(time(nullptr));
    std::string hmd5 = hmac_md5(token, password);
    std::string chksum = get_chksum();

    // 构建参数
    url += "&action=login";
    url += "&username=" + std::string(username);
    url += "&password=%7BMD5%7D" + hmd5;
    url += "&ac_id=" + std::string(ac_id);
    url += "&ip=" + ip;
    url += "&chksum=" + chksum;
    url += "&info=" + urlEncode(get_temp_i());
    url += "&n=" + n;
    url += "&type=" + type;
    url += "&os=windows%2B10";
    url += "&name=Windows";
    url += "&double_stack=0";
    url += "&ignore=2";
    url += "&_=" + std::to_string(time(nullptr));
    std::cout << url << std::endl;

    // 设置 curl 选项
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, curl_slist_append(nullptr, requests_headers.c_str()));

    // 执行请求
    std::string response;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
    }

    // 清理
    curl_easy_cleanup(curl);

    return response;
}

std::string auto_login() {

    init_getip();
    get_token();

    return login();
}

int main5() {

    init_getip();
    get_token();
    std::cout << login() << std::endl;

    return 0;
}
