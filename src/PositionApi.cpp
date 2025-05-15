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

/** @brief Get the positions near a landmark.
 *
 *   This function retrieves the positions near a specified landmark
 *   by specifying the utc_time and the interval in seconds.
 *
 *  @param unix_timestamp The unix_timestamp to search for.
 *  @param interval_secs The time interval in seconds on either side of the UTC time provided.
 *      A value of 5 would return 10 seconds of data (5 seconds before and 5 seconds after).
 *      Maximum value of 10 seconds.
 *  @return A JSON object containing the positions near the specified UTC time.

**/
nlohmann::json get_positions_near_unix_timestamp(const long unix_timestamp, const double interval_secs){

    double query_interval = std::max(0.0, std::min(10.0, interval_secs));
    std::string start_time_str = unix_to_utc_timestamp(unix_timestamp - query_interval * 1000);
    std::string end_time_str = unix_to_utc_timestamp(unix_timestamp + query_interval * 1000);

    std::string base_url = "http://" + dashcam::DASHCAM_HOST + ":" + dashcam::DASHCAM_PORT;
    std::string request_url = base_url + "/api/1/gnssConcise/locationsBetweenUtcTime";
    request_url += "?startTime=" + url_encode(start_time_str) + "&endTime=" + url_encode(end_time_str);

    nlohmann::json position_data = get_json_from_url(request_url);
    // Convert "utc_time" to unix_timestamp for each position entry
    for (auto& position : position_data) {
        if (position.find("utc_time") != position.end()) {
            position["unix_timestamp"] = utc_to_unix_timestamp(position["utc_time"]);
        }
    }
    // Return the modified position data
    return position_data;
}

/* @brief Convert UTC timestamp to Unix timestamp
*
*   Converts a UTC timestamp string to a Unix timestamp in microseconds.
*
*   @param timestamp_str The UTC timestamp string to convert.
*   @return The Unix timestamp in microseconds.
*/
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
    time_t time_sec = timegm(&tm);
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

std::string unix_to_utc_timestamp(const long unix_ms) {
    // Split milliseconds into seconds and remaining milliseconds
    time_t seconds = unix_ms / 1000;
    int remaining_ms = unix_ms % 1000;

    // Convert to UTC time
    std::tm* tm_utc = std::gmtime(&seconds);
    if (!tm_utc) {
        throw std::runtime_error("Failed to convert time_t to UTC struct.");
    }

    // Format main timestamp
    std::ostringstream oss;
    oss << std::put_time(tm_utc, "%Y-%m-%d %H:%M:%S");

    // Convert remaining ms to microseconds and format as 6-digit fractional part
    int micros = remaining_ms * 1000;
    oss << '.' << std::setw(6) << std::setfill('0') << micros;

    return oss.str();
}

std::string url_encode(const std::string &value) {
    std::ostringstream escaped;
    escaped.fill('0');
    escaped << std::hex;

    for (char c : value) {
        if (isalnum(static_cast<unsigned char>(c)) || c == '-' || c == '_' || c == '.' || c == '~') {
            escaped << c;
        } else {
            escaped << '%' << std::setw(2) << int((unsigned char)c);
        }
    }

    return escaped.str();
}