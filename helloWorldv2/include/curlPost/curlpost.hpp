#include <curl/curl.h>
#include <iostream>
#include <string>

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output);
bool performPostRequest(const std::string& url, const std::string& postData, std::string& response);