//
// Created by God on 2023/12/12.
//

#include <cstring>
#include <sstream>
#include <iostream>
#include "generate.h"
#include "main.h"
#include "decode.h"
#include "xencode.h"

const char* get_info() {
    // 计算所需缓冲区大小
    size_t buffer_size = strlen(username) + strlen(password) + strlen(init_url) + strlen(ac_id) + strlen(enc) + 200; // 额外的字符用于 JSON 结构和字符串终止符

    // 创建缓冲区
    char* buffer = new char[buffer_size];

    // 构建 JSON 字符串
    snprintf(buffer, buffer_size, R"({"username":"%s","password":"%s","ip":"%s","acid":"%s","enc_ver":"%s"})",
             username, password, ip.c_str(), ac_id, enc);

    return buffer;
}

std::string get_temp_i() {
    std::string temp_i = "{SRBX1}" + get_base64(get_xencode(get_info(), token));
    return temp_i;
}

std::string get_chksum() {
    std::ostringstream chksum;
    chksum << token << username;
    chksum << token << hmac_md5(token, password);
    chksum << token << ac_id;
    chksum << token << ip;
    chksum << token << n;
    chksum << token << type;
    chksum << token << get_temp_i();
    return get_sha1(chksum.str());
}

