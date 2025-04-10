/* File API */

#ifndef FILE_API_H
#define FILE_API_H

#include <iostream>
#include <string>
#include <fstream>

#include <curl/curl.h>
#include <nlohmann/json.hpp>

#include "hivecpp/Utils.h"

/** @brief Post a JSON file to the plugin_cache
 *
 *   @param filename The name of the file to post.
 *   @param jsonContent The JSON content to post.
 *   @return true if the file was posted successfully, false otherwise.
 */
bool post_json_file(const std::string& filename, const std::string& jsonContent);

/** @brief List all files in the plugin_cache
 *
 *   @return A JSON object containing the list of files.
 */
nlohmann::json list_cached_files();

/**
 * @brief Check if a file exists in the plugin cache by querying the /file/exists API
 *
 * @param fileName The name of the file to check.
 * @return true if the file exists, false otherwise.
 */
bool check_file_exists(const std::string& fileName);

/** @brief Delete a file from the plugin_cache
 *
 *   @param fileName The name of the file to delete.
 *   @return true if the file was deleted successfully, false otherwise.
*/
bool delete_file(const std::string& fileName);

/** @brief Purge the plugin_cache
 *
 *   @return true if the cache was purged successfully, false otherwise.
*/
bool purge_cache();

#endif // FILE_API_H

