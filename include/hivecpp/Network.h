// Network.h

#ifndef NETWORK_H
#define NETWORK_H

#include <string>
#include <nlohmann/json.hpp>

/**
 * @brief Routes a network request via a proxy gateway.
 *
 * This function directs any network request through the proxy located at
 * "http://gateway.beemaps.com/". The function is universal in that:
 *   - The request body can be any string (JSON, plain text, XML, etc.).
 *   - The HTTP method is fully flexible (e.g., GET, POST, PUT, DELETE, etc.).
 *   - Headers are provided as a JSON object containing arbitrary key/value pairs.
 *
 * Internally, the function packages the parameters into a JSON payload and sends
 * the payload to the proxy using libcurl. It returns the raw string response as
 * received from the proxy.
 *
 * @param target_url The actual URL for the network call.
 * @param method The HTTP method to use (defaults to "GET").
 * @param body The request payload as a string (defaults to an empty string).
 * @param headers A JSON object containing any additional HTTP header key/value pairs.
 * @return std::string The raw response from the proxy.
 * @throws std::runtime_error if there is an error with the request process.
 */
std::string fetch(const std::string& target_url,
                                  const std::string& method = "GET",
                                  const std::string& body = "",
                                  const nlohmann::json& headers = nlohmann::json::object());

#endif // NETWORK_H
