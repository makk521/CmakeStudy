#include "curlpost.hpp"

// Callback function to handle server response
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t total_size = size * nmemb;
    output->append((char*)contents, total_size);
    return total_size;
}

// Function to make a POST request
bool performPostRequest(const std::string& url, const std::string& postData, std::string& response) {
    // Initialize libcurl
    if (curl_global_init(CURL_GLOBAL_DEFAULT) != CURLE_OK) {
        std::cerr << "Failed to initialize libcurl." << std::endl;
        return false;
    }

    // Create a CURL object
    CURL* curl = curl_easy_init();
    if (curl) {
        // Set request URL
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        // Set POST data
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData.c_str());

        // Set write callback function to handle server response
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        // Perform the request
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
            curl_easy_cleanup(curl);
            curl_global_cleanup();
            return false;
        }

        // Cleanup CURL object
        curl_easy_cleanup(curl);
    } else {
        std::cerr << "Failed to initialize libcurl." << std::endl;
        curl_global_cleanup();
        return false;
    }

    // Cleanup libcurl
    curl_global_cleanup();
    return true;
}
