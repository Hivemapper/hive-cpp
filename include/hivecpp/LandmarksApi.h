/* Landmarks Api */

#ifndef LANDMARKS_API_H
#define LANDMARKS_API_H

#include <iostream>
#include <string>

#include <nlohmann/json.hpp>

#include "hivecpp/Utils.h"

/* @brief Get the latest landmark
*
*   Retrieves the latest (last inserted) observations and landmarks by their rowid.
*
*   @return A JSON object containing the latest landmark data.
*/
nlohmann::json get_latest_landmark();

/* @brief Get the last n landmarks
*
*   Retrieves the last n landmarks (in descending order based on their internal id)
*   along with the most recent associated observation timestamp.
*
*   @param n The number of landmarks to retrieve.
*   @return A JSON object containing the last n landmarks.
*/
nlohmann::json get_last_n_landmarks(int n);

// Landmark API function calls
/* @brief Get landmarks with timestamps in a range
*
*   Retrieves landmarks with timestamps in the specified range.
*   If no landmarks are found, the endpoint falls back to returning the last 5000 landmarks.
*
*   @param since_timestamp The lower bound (inclusive) for the unix timestamp of the landmarks.
*   @param until_timestamp The upper bound (inclusive) for the unix timestamp of the landmarks.
*   @return A JSON object containing the landmark data.
*/
nlohmann::json get_landmarks_by_timestamp(long since_timestamp = -1, long until_timestamp = -1);

/* @brief Get landmark starting with id (inclusive)
*
*   Retrieves the landmark with the specified id and its associated observation timestamp.
*   If no landmark is found with the given id, the endpoint falls back to returning the last
*   5000 landmarks.
*
*   @param id The id of the landmark to retrieve.
*   @return A JSON object containing the landmark data.
*/
nlohmann::json get_landmarks_after_id(long id);

#endif // LANDMARKS_API_H