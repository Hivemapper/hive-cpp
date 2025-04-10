
#include <string>

#include "hivecpp/LandmarksApi.h"


/* @brief Get the latest landmark
*
*   Retrieves the latest (last inserted) observations and landmarks by their rowid.
*
*   @return A JSON object containing the latest landmark data.
*/
nlohmann::json get_latest_landmark(){
    std::string landmarks_url = "http://" + dashcam::DASHCAM_HOST + ":" + dashcam::DASHCAM_PORT + "/api/1/landmarks";
    std::string request = landmarks_url + "/latest";
    return get_json_from_url(request);
}

/* @brief Get the last n landmarks
*
*   Retrieves the last n landmarks (in descending order based on their internal id)
*   along with the most recent associated observation timestamp.
*
*   @param n The number of landmarks to retrieve.
*   @return A JSON object containing the last n landmarks.
*/
nlohmann::json get_last_n_landmarks(int n){
    std::string landmarks_url = "http://" + dashcam::DASHCAM_HOST + ":" + dashcam::DASHCAM_PORT + "/api/1/landmarks";
    std::string request = landmarks_url + "/last/" + std::to_string(n);
    return get_json_from_url(request);
}

/* @brief Get landmarks with timestamps in a range
*
*   Retrieves landmarks with timestamps in the specified range.
*   If no landmarks are found, the endpoint falls back to returning the last 5000 landmarks.
*
*   @param since_timestamp The lower bound (inclusive) for the unix timestamp of the landmarks.
*   @param until_timestamp The upper bound (inclusive) for the unix timestamp of the landmarks.
*   @return A JSON object containing the landmark data.
*/
nlohmann::json get_landmarks_by_timestamp(long since_timestamp, long until_timestamp){
    std::string landmarks_url = "http://" + dashcam::DASHCAM_HOST + ":" + dashcam::DASHCAM_PORT + "/api/1/landmarks";
    std::string request = landmarks_url;
    if (since_timestamp != -1){
        request += "?since=" + std::to_string(since_timestamp);
    }
    if (since_timestamp != -1 && until_timestamp != -1){
        request += "&until=" + std::to_string(until_timestamp);
    }
    else if (until_timestamp != -1){
        request += "?until=" + std::to_string(until_timestamp);
    }
    return get_json_from_url(request);
}

/* @brief Get landmark starting with id (inclusive)
*
*   Retrieves the landmark with the specified id and its associated observation timestamp.
*   If no landmark is found with the given id, the endpoint falls back to returning the last
*   5000 landmarks.
*
*   @param id The id of the landmark to retrieve.
*   @return A JSON object containing the landmark data.
*/
nlohmann::json get_landmarks_after_id(long id){
    std::string landmarks_url = "http://" + dashcam::DASHCAM_HOST + ":" + dashcam::DASHCAM_PORT + "/api/1/landmarks";
    std::string request = landmarks_url + "/" + std::to_string(id);
    return get_json_from_url(request);
}
