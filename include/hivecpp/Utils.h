/* Utility Functions */

#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>
#include <cstdlib>

#include <curl/curl.h>
#include <nlohmann/json.hpp>

// get environment variable or default value
std::string get_env_or_default(const char* env_var, const std::string& default_value);

namespace dashcam {
    extern const std::string DASHCAM_HOST;
    extern const std::string DASHCAM_PORT;
}

// Helper for writing curl response into a string
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);

// Get JSON from URL
nlohmann::json get_json_from_url(const std::string& url);

#endif // UTILS_H