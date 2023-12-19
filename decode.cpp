//
// Created by God on 2023/11/3.
//

#include <openssl/evp.h>
#include <openssl/sha.h>
#include <iostream>
#include <sstream>
#include <iomanip>

#include <openssl/hmac.h>
#include <openssl/md5.h>

//std::string hmac_md5(const std::string& key, const std::string& message) {
//    unsigned char digest[MD5_DIGEST_LENGTH];
//    unsigned int digest_length;
//
//    HMAC_CTX* ctx = HMAC_CTX_new();
//    HMAC_Init_ex(ctx, key.c_str(), key.length(), EVP_md5(), nullptr);
//    HMAC_Update(ctx, reinterpret_cast<const unsigned char*>(message.c_str()), message.length());
//    HMAC_Final(ctx, digest, &digest_length);
//    HMAC_CTX_free(ctx);
//
//    std::ostringstream result;
//    for (unsigned int i = 0; i < digest_length; ++i) {
//        result << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(digest[i]);
//    }
//
//    return result.str();
//}

std::string hmac_md5(const std::string &key, const std::string &data) {
    const EVP_MD *md = EVP_md5();  // Use MD5 hash function
    unsigned char result[EVP_MAX_MD_SIZE];
    unsigned int resultLen;

    HMAC(md, key.c_str(), key.length(), reinterpret_cast<const unsigned char *>(data.c_str()), data.length(), result, &resultLen);

    // Convert the result to a hex string
    std::string hmac;
    char hexBuffer[3];
    for (unsigned int i = 0; i < resultLen; ++i) {
        snprintf(hexBuffer, sizeof(hexBuffer), "%02x", result[i]);
        hmac += hexBuffer;
    }

    return hmac;
}

std::string get_sha1(const std::string& value) {
    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA1(reinterpret_cast<const unsigned char*>(value.c_str()), value.size(), hash);

    std::stringstream ss;
    for (unsigned char i : hash) {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(i);
    }

    return ss.str();
}

const char PADCHAR = '=';
const std::string ALPHA = "LVoJPiCN2R8G90yg+hmFHuacZ1OWMnrsSTXkYpUq/3dlbfKwv6xztjI7DeBE45QA";

// 获取字符串 s 中索引 i 处的字符的整数值
int getbyte(const std::string& s, int i) {
    unsigned char x = s[i];
    return x;
}

std::string get_base64(const std::string& s) {
    long int i, b10;
    int imax = static_cast<int>(s.length() - s.length() % 3);
    std::string result;
    result.reserve(s.length()); // 预先分配足够的内存以减少动态分配

    // 按 3 字节块处理字符串
    for (i = 0; i < imax; i += 3) {
        b10 = (getbyte(s, i) << 16) | (getbyte(s, i + 1) << 8) | getbyte(s, i + 2);
        result.push_back(ALPHA[b10 >> 18]);
        result.push_back(ALPHA[(b10 >> 12) & 63]);
        result.push_back(ALPHA[(b10 >> 6) & 63]);
        result.push_back(ALPHA[b10 & 63]);
    }

    // 处理最后 1 或 2 个字节的不完整块
    int remaining = static_cast<int>(s.length() - i);
    if (remaining == 1) {
        b10 = getbyte(s, i) << 16;
        result.push_back(ALPHA[b10 >> 18]);
        result.push_back(ALPHA[(b10 >> 12) & 63]);
        result.push_back(PADCHAR);
        result.push_back(PADCHAR);
    } else if (remaining == 2) {
        b10 = (getbyte(s, i) << 16) | (getbyte(s, i + 1) << 8);
        result.push_back(ALPHA[b10 >> 18]);
        result.push_back(ALPHA[(b10 >> 12) & 63]);
        result.push_back(ALPHA[(b10 >> 6) & 63]);
        result.push_back(PADCHAR);
    }

    return result;
}

