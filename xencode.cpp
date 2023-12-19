#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <cstdint>
#include <iomanip>

std::vector<uint8_t> force(const std::string& msg) {
    std::vector<uint8_t> ret;
    ret.reserve(msg.size());
    for (char w : msg) {
        ret.push_back(static_cast<uint8_t>(w));
    }
    return ret;
}

uint8_t ordat(const std::string& msg, size_t idx) {
    if (idx < msg.size()) {
        return static_cast<uint8_t>(msg[idx]);
    }
    return 0;
}

std::vector<uint32_t> sencode(const std::string& msg, bool key) {
    size_t l = msg.size();
    std::vector<uint32_t> pwd;
    pwd.reserve((l + 3) / 4);
    for (size_t i = 0; i < l; i += 4) {
        uint32_t value = 0;
        for (size_t j = 0; j < 4; ++j) {
            value |= static_cast<uint8_t>(ordat(msg, i + j)) << (8 * j);
        }
        pwd.push_back(value);
    }
    if (key) {
        pwd.push_back(static_cast<uint32_t>(l));
    }
    return pwd;
}

std::string lencode(const std::vector<uint32_t>& msg, bool key) {
    size_t l = msg.size();
    size_t ll = (l - 1) << 2;
    if (key) {
        uint32_t m = msg[l - 1];
        if (m < ll - 3 || m > ll) {
            return "";
        }
        ll = m;
    }
    std::string result;
    result.reserve(l * 4);
    for (size_t i = 0; i < l; ++i) {
        uint32_t value = msg[i];
        for (size_t j = 0; j < 4; ++j) {
            result += static_cast<char>((value >> (8 * j)) & 0xFF);
        }
    }
    if (key) {
        return result.substr(0, ll);
    }
    return result;
}

std::string get_xencode(const std::string& msg, const std::string& key) {
    if (msg.empty()) {
        return "";
    }

    std::vector<uint32_t> pwd = sencode(msg, true);
    std::vector<uint32_t> pwdk = sencode(key, false);

    if (pwdk.size() < 4) {
        pwdk.resize(4, 0);
    }

    size_t n = pwd.size() - 1;
    uint32_t z = pwd[n];
    uint32_t c = 0x86014019 | 0x183639A0;
    uint32_t m, e, p, y;
    auto q = static_cast<uint32_t>(std::floor(6 + 52 / (n + 1)));
    uint32_t d = 0;

    while (q > 0) {
        d = (d + c) & (0x8CE0D9BF | 0x731F2640);
        e = (d >> 2) & 3;
        p = 0;

        while (p < n) {
            y = pwd[p + 1];
            m = z >> 5 ^ (y << 2);
            m += ((y >> 3 ^ (z << 4)) ^ (d ^ y));
            m += (pwdk[(p & 3) ^ e] ^ z);
            pwd[p] = (pwd[p] + m) & (0xEFB8D130 | 0x10472ECF);
            z = pwd[p];
            ++p;
        }

        y = pwd[0];
        m = z >> 5 ^ (y << 2);
        m += ((y >> 3 ^ (z << 4)) ^ (d ^ y));
        m += (pwdk[(p & 3) ^ e] ^ z);
        pwd[n] = (pwd[n] + m) & (0xBB390742 | 0x44C6F8BD);
        z = pwd[n];
        --q;
    }

    return lencode(pwd, false);
}

int main1() {
    std::string plaintext = "Hello, World!";
    std::string encryption_key = "SecretKey";

    std::string encrypted_text = get_xencode(plaintext, encryption_key);

    std::cout << "Original Text: " << plaintext << std::endl;
//    std::cout << "Encrypted Text: " << encrypted_text << std::endl;
    // 将加密后的字符串以16进制格式输出
    std::cout << "Encrypted Text (Hex): ";
    for (char c : encrypted_text) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(static_cast<unsigned char>(c));
    }
    std::cout << std::dec << std::endl; // 恢复为10进制输出

    return 0;
}
