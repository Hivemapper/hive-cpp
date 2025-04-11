cmake_minimum_required(VERSION 3.10)
project(hivecpp)

# <---------- set c++ standard ------------->
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the output directory for the library
set(CMAKE_INSTALL_PREFIX /opt/dashcam/bin/hivecpp)

# add libcurl dependency
set(CURL_STATICLIB ON)
find_package(CURL REQUIRED)

# add hivecpp library
add_library(hivecpp STATIC
    src/PositionApi.cpp
    src/LandmarksApi.cpp
    src/FileApi.cpp
    src/Utils.cpp
    src/Network.cpp
)

# add include directory
target_include_directories(hivecpp PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/lib
)

target_link_libraries(hivecpp
    PRIVATE CURL::libcurl
)

# Specify the install location for the library
install(TARGETS hivecpp DESTINATION lib)
install(DIRECTORY include/ DESTINATION include)