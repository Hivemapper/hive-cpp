/* Position API */

#include "hivecpp/PositionApi.h"

/* @brief Get the latest position
*
*   Will return empty JSON object if no position is available yet
*   meaning that the GNSS has not yet obtained a fix after boot
*
*   @return A JSON object containing the latest position data.
*/
nlohmann::json get_latest_position(){
    std::string postion_url = "http://" + dashcam::DASHCAM_HOST + ":" + dashcam::DASHCAM_PORT + "/api/1/gnssConcise";
    std::string request = postion_url + "/latestValid";
    nlohmann::json position = get_json_from_url(request);
    // check if "utc_time" key exists
    if (position.find("utc_time") != position.end()) {
        position["unix_timestamp"] = utc_to_unix_timestamp(position["utc_time"]);
    }
    return position;
}

long utc_to_unix_timestamp(const std::string& timestamp_str) {
    // Split timestamp into main datetime and fractional part
    size_t dot_pos = timestamp_str.find('.');
    if (dot_pos == std::string::npos) {
        throw std::runtime_error("Invalid timestamp format. No fractional part found.");
    }

    std::string datetime_part = timestamp_str.substr(0, dot_pos);
    std::string fractional_part = timestamp_str.substr(dot_pos + 1);

    // Parse datetime part
    std::tm tm = {};
    std::istringstream ss(datetime_part);
    ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
    if (ss.fail()) {
        throw std::runtime_error("Failed to parse timestamp.");
    }

    // Convert to time_t (Unix timestamp in seconds)
    time_t time_sec = std::mktime(&tm);
    if (time_sec == -1) {
        throw std::runtime_error("Failed to convert to time_t.");
    }

    // Parse fractional microseconds (pad if necessary to get 6 digits)
    while (fractional_part.length() < 6) {
        fractional_part += "0";
    }
    long micros = std::stol(fractional_part.substr(0, 6));

    // Combine seconds and microseconds
    long full_timestamp = static_cast<long long>(time_sec) * 1000 + micros * 1E-3;
    return full_timestamp;
}