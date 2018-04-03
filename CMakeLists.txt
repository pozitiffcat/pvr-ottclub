cmake_minimum_required(VERSION 2.8)

project(ottpvr)

set(CMAKE_CXX_FLAGS -std=c++03)
set(CMAKE_BUILD_TYPE debug)

include_directories(
    "${XBMC_ROOT}/xbmc/addons/kodi-addon-dev-kit/include/kodi"
    "${XBMC_ROOT}/xbmc")

add_library(${PROJECT_NAME} SHARED "addon.cpp" "OTTClient.cpp")
target_link_libraries(${PROJECT_NAME} curl jsoncpp)
