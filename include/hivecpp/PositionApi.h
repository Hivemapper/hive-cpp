/* Position API */

#ifndef POSITION_API_H
#define POSITION_API_H


#include <iostream>
#include <string>

#include <nlohmann/json.hpp>

#include "hivecpp/Utils.h"

/* @brief Get the latest position
*
*   Will return empty JSON object if no position is available yet
*   meaning that the GNSS has not yet obtained a fix after boot
*
*   @return A JSON object containing the latest position data.
*/
nlohmann::json get_latest_position();

/* @brief Convert UTC timestamp to Unix timestamp
*
*   Converts a UTC timestamp string to a Unix timestamp in microseconds.
*
*   @param timestamp_str The UTC timestamp string to convert.
*   @return The Unix timestamp in microseconds.
*/
long utc_to_unix_timestamp(const std::string& timestamp_str);

#endif // POSITION_API_H