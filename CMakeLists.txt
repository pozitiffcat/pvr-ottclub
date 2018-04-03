cmake_minimum_required(VERSION 2.8)

project(ottpvr)

include_directories(
    "${XBMC_ROOT}/xbmc/addons/kodi-addon-dev-kit/include/kodi"
    "${XBMC_ROOT}/xbmc")

add_library(${PROJECT_NAME} SHARED "addon.cpp")
