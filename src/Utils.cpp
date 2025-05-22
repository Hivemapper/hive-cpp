/* Utility Functions */

#include "hivecpp/Utils.h"

std::string get_env_or_default(const char* env_var, const std::string& default_value) {
    const char* value = std::getenv(env_var);
    return (value == nullptr) ? default_value : std::string(value);
}

namespace dashcam {
    const std::string DASHCAM_HOST = get_env_or_default("DASHCAM_HOST", "192.168.0.10");
    const std::string DASHCAM_PORT = get_env_or_default("DASHCAM_PORT", "5000");
}

// Helper for writing curl response into a string
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

nlohmann::json get_json_from_url(const std::string& url) {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;
    long http_code = 0;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        // Set the write function and buffer
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        // Optional: follow redirects
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        res = curl_easy_perform(curl);

        // Get the HTTP response code
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
        curl_easy_cleanup(curl);

        if (res != CURLE_OK) {
            throw std::runtime_error("curl_easy_perform() failed: " + std::string(curl_easy_strerror(res)));
        }

        // Attempt to parse the response, even for errors
        nlohmann::json response_json;
        try {
            response_json = nlohmann::json::parse(readBuffer);
        } catch (...) {
            response_json = nlohmann::json{
                {"error", "Invalid JSON in response body"},
                {"raw_response", readBuffer}
            };
        }

        if (http_code >= 400) {
            // Attach HTTP code to error response
            response_json["status_code"] = http_code;
            throw std::runtime_error(response_json.dump());
        }

        return response_json;
    } else {
        throw std::runtime_error("Failed to initialize CURL");
    }
}
