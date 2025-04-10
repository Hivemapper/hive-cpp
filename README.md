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
  "altitude": 15.891,
  "cno": 39,
  "heading": 297.11066000000005,
  "id": 858894,
  "latitude": 37.7880956,
  "longitude": -122.3992437,
  "speed": 0.029583653435111046,
  "utc_time": "2025-04-08 23:07:52.750329",
  "unix_timestamp": 1744153672750
}
```

### Output Field Descriptions:
- `latitude`, `longitude`: **Position** in decimal degrees (WGS84)
- `altitude`: **Height** in meters above ellipsoid
- `speed`: **Speed** in meters per second (m/s)
- `heading`: **Movement direction** in degrees (0–360 clockwise, 0 = North)
- `cno`: **Carrier-to-noise density ratio** in dB-Hz (GNSS signal quality)
- `utc_time`: UTC timestamp of fix
- `unix_timestamp`: Milliseconds since unix epoch
- `id`: Internal row identifier

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
    "class_label": "traffic-light",
    "id": 144,
    "lat": 37.88837779145528,
    "lon": -122.11823105294746,
    "alt": 57.006156626506026,
    "azimuth": 2.939116503885373,
    "ts": 1744223531970,
    "pos_confidence": 0.7199414419651322
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
- `class_label`: Object type (e.g., `traffic-light`, `regulatory-speed-sign`)
- `attributes`: Additional structured data (e.g., `speed_label` in km/h and its `speed_label_conf`)
- `id`: Internal identifier for the landmark

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

## 🛠️ Requirements

- `libcurl`
- C++17 or newer

---
