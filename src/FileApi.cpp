/* File API */

#include "hivecpp/FileApi.h"

/** @brief Post a JSON file to the plugin_cache
 *
 *   @param filename The name of the file to post.
 *   @param jsonContent The JSON content to post.
 *   @return true if the file was posted successfully, false otherwise.
 */
bool post_json_file(const std::string& filename, const std::string& jsonContent) {
    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Failed to initialize libcurl.\n";
        return false;
    }

    std::string apiBaseUrl = "http://" + dashcam::DASHCAM_HOST + ":" + dashcam::DASHCAM_PORT + "/api/1/file/";
    std::string url = apiBaseUrl + "?name=" + curl_easy_escape(curl, filename.c_str(), filename.length());

    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonContent.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, jsonContent.size());

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "CURL error: " << curl_easy_strerror(res) << "\n";
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
        return false;
    }

    long responseCode;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &responseCode);

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    std::cout << std::endl;

    return responseCode == 200;
}

/** @brief List all files in the plugin_cache
 *
 *   @return A JSON object containing the list of files.
 */
nlohmann::json list_cached_files() {
    std::string url = "http://" + dashcam::DASHCAM_HOST + ":" + dashcam::DASHCAM_PORT + "/api/1/file/list";
    return get_json_from_url(url);
}

/**
 * @brief Check if a file exists in the plugin cache by querying the /file/exists API
 *
 * @param fileName The name of the file to check.
 * @return true if the file exists, false otherwise.
 */
bool check_file_exists(const std::string& fileName) {
    CURL* curl = curl_easy_init();
    if (!curl) return false;

    char* escapedName = curl_easy_escape(curl, fileName.c_str(), 0);
    if (!escapedName) {
        curl_easy_cleanup(curl);
        return false;
    }

    std::string url = "http://" + dashcam::DASHCAM_HOST + ":" + dashcam::DASHCAM_PORT + "/api/1/file/exists/?name=" + std::string(escapedName);
    nlohmann::json jsonResponse = get_json_from_url(url);

    std::cout << jsonResponse << std::endl;

    // parse the JSON body to check if the file exists
    try {
        // Check the "exists" field in the JSON response
        if (jsonResponse.contains("exists")) {
            return jsonResponse["exists"].get<bool>();
        }
    } catch (const nlohmann::json::parse_error& e) {
        std::cerr << "Error parsing JSON response: " << e.what() << std::endl;
    }

    return false;
}

/** @brief Delete a file from the plugin_cache
 *
 *   @param fileName The name of the file to delete.
 *   @return true if the file was deleted successfully, false otherwise.
*/
bool delete_file(const std::string& fileName) {
    CURL* curl = curl_easy_init();
    if (!curl) return false;

    char* escapedName = curl_easy_escape(curl, fileName.c_str(), 0);
    if (!escapedName) {
        curl_easy_cleanup(curl);
        return false;
    }

    std::string url = "http://" + dashcam::DASHCAM_HOST + ":" + dashcam::DASHCAM_PORT + "/api/1/file/?name=" + std::string(escapedName);
    curl_free(escapedName);

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");

    // Capture the response code
    long responseCode = 0;

    CURLcode res = curl_easy_perform(curl);
    if (res == CURLE_OK) {
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &responseCode);
    }

    curl_easy_cleanup(curl);

    std::cout << std::endl;

    return (res == CURLE_OK && responseCode == 200);
}

/**
 * @brief Purge the plugin_cache
 *
 * @return true if the cache was purged successfully (HTTP 200), false otherwise.
 */
bool purge_cache() {
    CURL* curl = curl_easy_init();
    if (!curl) return false;

    std::string url = "http://" + dashcam::DASHCAM_HOST + ":" + dashcam::DASHCAM_PORT + "/api/1/file/purge";

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);

    // Capture the response code
    long responseCode = 0;

    CURLcode res = curl_easy_perform(curl);
    if (res == CURLE_OK) {
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &responseCode);
    }

    curl_easy_cleanup(curl);

    std::cout << std::endl;

    return (res == CURLE_OK && responseCode == 200);
}