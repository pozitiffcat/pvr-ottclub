cmake_minimum_required(VERSION 2.8)

project(ottpvr)

set(CMAKE_BUILD_TYPE debug)

set(S_JSON_SRC_LIST
    "jparsers/jparsers.cpp"
    "jparsers/jvalue.cpp"
    "jparsers/jnull.cpp"
    "jparsers/jboolean.cpp"
    "jparsers/jnumber.cpp"
    "jparsers/jstring.cpp"
    "jparsers/jobject.cpp"
    "jparsers/jarray.cpp"
)

include_directories(
    "${XBMC_ROOT}/xbmc/addons/kodi-addon-dev-kit/include/kodi"
    "${XBMC_ROOT}/xbmc")

add_library(${PROJECT_NAME} SHARED ${S_JSON_SRC_LIST} "addon.cpp" "OTTClient.cpp")
target_link_libraries(${PROJECT_NAME} curl)

add_executable(otttest ${S_JSON_SRC_LIST} "otttest.cpp" "OTTClient.cpp")
target_link_libraries(otttest curl)
