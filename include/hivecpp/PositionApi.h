/* Position API */

#ifndef POSITION_API_H
#define POSITION_API_H


#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

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

/** @brief Get the positions near a landmark.
 *
 *   This function retrieves the positions near a specified landmark
 *   by specifying the utc_time and the interval in seconds.
 *
 *  @param unix_timestamp The unix_timestamp to search for.
 *  @param interval_secs The time interval in seconds on either side of the UTC time provided.
 *      A value of 5 would return 10 seconds of data (5 seconds before and 5 seconds after).
 *  @return A JSON object containing the positions near the specified UTC time.

**/
nlohmann::json get_positions_near_unix_timestamp(const long unix_timestamp, const double interval_secs);

/** @brief Convert UTC timestamp to Unix timestamp
*
*   Converts a UTC timestamp string to a Unix timestamp in microseconds.
*
*   @param timestamp_str The UTC timestamp string to convert.
*   @return The Unix timestamp in microseconds.
**/
long utc_to_unix_timestamp(const std::string& timestamp_str);


/** @brief Convert Unix timestamp to UTC timestamp
*
*   Converts a Unix timestamp string to a UTC timestamp in microseconds.
*
*   @param unix_timestamp The Unix timestamp in microseconds.
*   @return The UTC timestamp string to convert.
**/
std::string unix_to_utc_timestamp(const long unix_timestamp);

/** @brief URL-encode a string
 *
 *  Encodes a string for use in a URL by replacing special characters with their percent-encoded equivalents.
 *
 *  @param value The string to encode.
 *  @return The URL-encoded string.
 */
std::string url_encode(const std::string &value);

#endif // POSITION_API_H