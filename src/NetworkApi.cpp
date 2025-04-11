#include <iostream>
#include <stdexcept>
#include <string>

#include <curl/curl.h>
#include <nlohmann/json.hpp>

#include "hivecpp/Utils.h"
#include "hivecpp/NetworkApi.h"

/**
 * @brief Universal fetch via proxy
 *
 * This function routes any network call through the proxy located at
 * "https://bee-internet-gateway.hmworkers.workers.dev/". It is fully universal in the sense that:
 *
 * - The request body does not have to be JSON (or any particular format)
 * - The HTTP method can be any valid method (GET, POST, PUT, DELETE, etc.)
 * - Headers can be passed via a JSON object containing arbitrary key/value pairs
 * - Both the HTTP response code and the raw response body are returned.
 *
 * @param target_url  The actual URL to request.
 * @param method      The HTTP method (e.g., "GET", "POST", "PUT", "DELETE", etc.).
 * @param body        The request payload as a string (does not have to be JSON).
 * @param headers     Additional HTTP headers as a JSON object.
 * @return HttpResponse The response code along with the raw response data.
 * @throws std::runtime_error if any curl errors occur.
 */
HttpResponse fetch(const std::string& target_url,
                   const std::string& method,
                   const std::string& body,
                   const nlohmann::json& headers) {

    nlohmann::json requestData;
    requestData["url"]     = target_url;
    requestData["method"]  = method;
    requestData["body"]    = body;
    requestData["headers"] = headers; // headers can be any valid JSON

    std::string jsonData = requestData.dump();
    std::string proxyUrl = "https://bee-internet-gateway.hmworkers.workers.dev/";

    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Failed to initialize libcurl." << std::endl;
        throw std::runtime_error("Failed to initialize libcurl.");
    }

    curl_easy_setopt(curl, CURLOPT_URL, proxyUrl.c_str());
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, static_cast<long>(jsonData.size()));

    struct curl_slist* headerList = nullptr;
    headerList = curl_slist_append(headerList, "Content-Type: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerList);

    std::string responseBuffer;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseBuffer);

    CURLcode res = curl_easy_perform(curl);
    int httpCode = 0;
    if (res == CURLE_OK) {
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
    } else {
        std::cerr << "Curl error: " << curl_easy_strerror(res) << std::endl;
        curl_slist_free_all(headerList);
        curl_easy_cleanup(curl);
        throw std::runtime_error("Curl error: " + std::string(curl_easy_strerror(res)));
    }

    curl_slist_free_all(headerList);
    curl_easy_cleanup(curl);

    return HttpResponse{httpCode, responseBuffer};
}
