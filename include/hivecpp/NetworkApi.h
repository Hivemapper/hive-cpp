// NetworkApi.h

#ifndef NETWORK_API_H
#define NETWORK_API_H

#include <iostream>
#include <stdexcept>
#include <string>

#include <curl/curl.h>
#include <nlohmann/json.hpp>

#include "hivecpp/Utils.h"

struct HttpResponse {
    int statusCode;
    std::string body;
};

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
                   const nlohmann::json& headers);

#endif // NETWORK_API_H
