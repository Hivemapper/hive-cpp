# hive-cpp

This library provides **Position**, **Landmarks**, and **File** APIs. These APIs communicate with a dashcam server using HTTP requests to retrieve GNSS data, landmark observations, and manage cached files.

---

## 🔧 Configuration

All API requests are constructed using the following environment variables:

- `DASHCAM_HOST` (default: `192.168.0.10`)
- `DASHCAM_PORT` (default: `5000`)

---

## 📍 Position API

### `get_latest_position()`
**Returns:** JSON object containing the latest GNSS position.

**Output Example:**
```json
{
  "id": 858894,
  "latitude": 37.7880956,
  "longitude": -122.3992437,
  "altitude":-16.401,
  "estimated_speed":7.18377685546875,
  "utc_time": "2025-04-08 23:07:52.750329",
  "unix_timestamp": 1744153672750,
  "gdop":1.39,
  "hdop":0.67,
  "eph":3.776
}
```

### Output Field Descriptions:
- `latitude`, `longitude`: Position in decimal degrees (WGS84)
- `altitude`: Height in meters above ellipsoid
- `estimated_speed`: Estimated Speed in meters per second (m/s)
- `utc_time`: UTC timestamp of fix
- `unix_timestamp`: Milliseconds since unix epoch
- `gdop`: Geometric dilution of precision
- `hdop`: Horizontal dilution of precision
- `eph`: Standard deviation of horizontal position error
- `id`: Internal row identifier

### `get_positions_near_unix_timestamp(unix_timestamp, interval_secs)`

**Parameters:**

- `unix_timestamp` (long): The central point in time to search around, in milliseconds since unix epoch.

- `interval_secs` (double): Time range in seconds to search on both sides of unix_timestamp.
A value of 5 searches ±5 seconds, i.e., a total window of 10 seconds. Maximum allowed value is 10 seconds for a full window of 20 seconds.

**Returns:**
A JSON array of GNSS positions where each position includes an added unix_timestamp field (converted from utc_time).

**Output Example:**
```json
[
  {
    "id": 858891,
    "latitude": 37.7880901,
    "longitude": -122.3992480,
    "altitude":-16.401,
    "estimated_speed":7.18377685546875,
    "utc_time": "2025-04-08 23:07:47.750329",
    "unix_timestamp": 1744153672750,
    "gdop":1.39,
    "hdop":0.67,
    "eph":3.776
  },
  {
    "id": 858892,
    "latitude": 37.7880923,
    "longitude": -122.3992461,
    "altitude":-16.401,
    "estimated_speed":7.18377685546875,
    "utc_time": "2025-04-08 23:07:50.750329",
    "unix_timestamp": 1744153672750,
    "gdop":1.39,
    "hdop":0.67,
    "eph":3.625
  },
  ...
]
```

---

## 🗺️ Landmarks API

### `get_latest_landmark()`
**Returns:** Latest landmark observation.

**Output Example:**
```json
{
  "class_label": "speed-sign",
  "id": 150,
  "lat": 37.88845244211102,
  "lon": -122.11894385278458,
  "alt": 57.79848423999026,
  "azimuth": 86.00081339834321,
  "ts": 1744223540969,
  "pos_confidence": 0.7931751037445554,
  "attributes": {
    "speed_label": 25,
    "speed_label_conf": 0.9999944065961269
  }
}
```

### `get_last_n_landmarks(int n)`
**Returns:** Array of the last `n` landmarks.

**Output Example:**
```json
[
  {
    "class_label": "regulatory-speed-sign",
    "id": 150,
    "lat": 37.88845244211102,
    "lon": -122.11894385278458,
    "alt": 57.79848423999026,
    "azimuth": 86.00081339834321,
    "ts": 1744223540969,
    "pos_confidence": 0.7931751037445554,
    "attributes": {
      "speed_label": 25,
      "speed_label_conf": 0.9999944065961269
    }
  },
  {
    "class_label": "speed-sign",
    "id": 144,
    "lat": 37.88837779145528,
    "lon": -122.11823105294746,
    "alt": 57.006156626506026,
    "azimuth": 2.939116503885373,
    "ts": 1744223531970,
    "pos_confidence": 0.7199414419651322,
    "attributes": {
      "speed_label": 55,
      "speed_label_conf": 0.9999944065961269
    }
  }
]
```

### `get_landmarks_by_timestamp(long since, long until)`
**Returns:** Landmarks between `since` and `until` timestamps. Falls back to the last 5000 if none.

**Output Type:** Array of JSON objects

**Examples:**
- Only `since`: `?since=1744223530000` → Returns objects at and after the since timestamp (inclusive)
- `since` + `until`: `?since=1744223530000&until=1744223540000` → Filtered
- Only `until`: `?until=1744223531000` → Returns objects at and before the until timestamp (inclusive)

### `get_landmarks_after_id(long id)`
**Returns:** Landmarks after a specific ID (inclusive).

**Output Example:** Several landmark objects if ID is valid.

### Output Field Descriptions:
- `lat`, `lon`: **Latitude** and **Longitude** in decimal degrees (WGS84)
- `alt`: **Altitude** in meters above sea level
- `azimuth`: **Orientation** of object in degrees (0 = North)
- `ts`: **Unix timestamp** in milliseconds
- `pos_confidence`: **Position accuracy confidence** (0–1)
- `class_label`: Object type (e.g., `speed-sign`, `regulatory-speed-sign`)
- `attributes`: Additional structured data (e.g., `speed_label` in km/h and its `speed_label_conf`)
- `id`: Internal identifier for the landmark


### `get_landmark_context(id)`

**Parameters:**

- `id` (long) Landmark ID.


**Returns:**
A JSON array of GNSS positions nearby the specified landmark.

**Output Example:**
```json
[
  {
    "unix_timestamp" : 1745427610345,
    "latitude": 37.7880901,
    "longitude": -122.3992480,
    "altitude":-16.401,
    "estimated_speed":7.18377685546875,
    "eph":3.776
  },
  {
    "unix_timestamp" : 1745427611547,
    "latitude": 37.7880923,
    "longitude": -122.3992461,
    "altitude":-16.401,
    "estimated_speed":7.18377685546875,
    "eph":3.625
  },
  ...
]
```

---

## 📁 File API

### `post_json_file(filename, jsonContent)`
**Returns:** Boolean indicating success

**Output Example:**
```json
{"message":"File created successfully."}
```

### `list_cached_files()`
**Returns:** JSON with a list of file names.

**Output Example:**
```json
{"files": ["map1/map1.txt", "test.txt"]}
```

### `check_file_exists(filename)`
**Returns:** Boolean indicating file existence

**Output Example:**
```json
{"exists": true}
```

### `delete_file(filename)`
**Returns:** Boolean indicating if deletion succeeded

**Output Example:**
```json
{"message":"File created successfully."}
```

### `purge_cache()`
**Returns:** Boolean indicating if cache was cleared

**Output Example:**
```json
{"message":"Plugin cache directory purged successfully."}
```

---

## 🌐 Network Interface

### `fetch(target_url, method, body, headers)`
**Purpose:** Performs a universal HTTP request via the proxy gateway.

**Parameters:**
- `target_url` – Full URL to the final destination (e.g. `"http://httpbin.org/post"`)
- `method` – HTTP method as string (`"GET"`, `"POST"`, `"PUT"`, etc.)
- `body` – Request body as raw string (can be JSON, plain text, etc.)
- `headers` – JSON object containing key-value HTTP headers to pass along

**Returns:** An HttpResponse structure containing:
- `statusCode` - The HTTP status code of the response (e.g. 200, 400, etc.)
- `body` - The raw response data as a std::string

**Throws:** `std::runtime_error` if the request fails

**Example Usage:**
```cpp
nlohmann::json headers = {
  {"Content-Type", "application/json"},
  {"Authorization", "Bearer token123"}
};

std::string body = R"({"message":"hello"})";

HttpResponse response = fetch("http://httpbin.org/post", "POST", body, headers);
std::cout << "Status code: " << response.statusCode << std::endl;
std::cout << "Server responded with:
" << response.body << std::endl;
```

This interface is useful for sending network requests from the dashcam through a trusted proxy, without relying on external networking logic inside your own code.

---

## 🛠️ Requirements

- `libcurl`
- C++17 or newer

---
