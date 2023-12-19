//
// Created by God on 2023/11/3.
//
#include <iostream>
#include <string>
#include <regex>
#include <curl/curl.h>
#include "main.h"

// Define global variable 'ip'
std::string ip;
std::string token;

size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *output) {
    size_t total_size = size * nmemb;
    output->append((char *)contents, total_size);
    return total_size;
}

void init_getip() {
    // Initialize cURL
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if (curl) {
        // Set the URL to 'init_url'
        curl_easy_setopt(curl, CURLOPT_URL, init_url);

        // Set the 'User-Agent' header
        struct curl_slist *headers = nullptr;
        headers = curl_slist_append(headers, requests_headers.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        // 允许自动跳转
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);

        // Store the response in 'response'
        std::string response;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        // Perform the HTTP request
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        // Search for the IP in the HTTP response
        std::regex ip_regex("id=\"user_ip\" value=\"(.*?)\"");
        std::smatch ip_match;
        if (std::regex_search(response, ip_match, ip_regex)) {
            ip = ip_match[1];
            std::cout << "ip: " << ip << std::endl;
        } else{
            std::cout << response << std::endl;
        }

        // Cleanup cURL
        curl_easy_cleanup(curl);
    }
}

void get_token() {
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, get_challenge_api.c_str());

        struct curl_slist *headers = nullptr;
        headers = curl_slist_append(headers, requests_headers.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        std::string callback_param = "jQuery1124007344203445056241_" + std::to_string(int(time(nullptr) * 1000));
        std::string timestamp_param = std::to_string(int(time(nullptr) * 1000));

        std::string get_challenge_url = get_challenge_api;
        get_challenge_url += "?callback=" + callback_param;
        get_challenge_url += "&username=" + std::string(username);
        get_challenge_url += "&ip=" + ip;
        get_challenge_url += "&_=" + timestamp_param;

        curl_easy_setopt(curl, CURLOPT_URL, get_challenge_url.c_str());

        std::string response;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
        std::regex challenge_regex("\"challenge\":\"(.*?)\"");
        std::smatch challenge_match;
        if (std::regex_search(response, challenge_match, challenge_regex))
            token = challenge_match[1];
        std::cout << "response: " << token << std::endl;

        curl_easy_cleanup(curl);
    }
}

