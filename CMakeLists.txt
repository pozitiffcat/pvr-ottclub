cmake_minimum_required(VERSION 2.8)

project(ottpvr)

set(CMAKE_BUILD_TYPE debug)

add_subdirectory("3rd_party/jsoncpp/include")

include_directories(
    "3rd_party/jsoncpp"
    "${XBMC_ROOT}/xbmc/addons/kodi-addon-dev-kit/include/kodi"
    "${XBMC_ROOT}/xbmc")

add_library(${PROJECT_NAME} SHARED "addon.cpp" "OTTClient.cpp")
target_link_libraries(${PROJECT_NAME} curl jsoncpp)

add_executable(testott "otttest.cpp" "OTTClient.cpp")
target_link_libraries(testott curl jsoncpp)
