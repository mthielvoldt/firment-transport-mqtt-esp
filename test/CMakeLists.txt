cmake_minimum_required(VERSION 3.31)
set(CMAKE_POLICY_VERSION_MINIMUM 3.31)

add_compile_options("-g")

project(fmt_transport_mqtt_esp_test LANGUAGES CXX C)

include(FetchContent)
FetchContent_Declare(
  CppUTest
  GIT_REPOSITORY https://github.com/cpputest/cpputest.git
  GIT_TAG v4.0
  GIT_SHALLOW TRUE
)
FetchContent_Populate(
  esp-mqtt
  GIT_REPOSITORY https://github.com/espressif/esp-mqtt.git
  GIT_TAG 6af4446a48ea7fa54948edc4e62277ed70abb6e1 # used by esp-idf v5.5.1
  # GIT_SUBMODULES ""
  # SOURCE_SUBDIR "${PROJECT_SOURCE_DIR}/mocks/esp-mqtt"  # prevent building
)
FetchContent_MakeAvailable(CppUTest)

add_subdirectory(mocks/esp-mqtt) # -> provides 
add_subdirectory(.. fmt_transport)

add_executable(fmt_transport_test
  src/fmt_transport_mqtt_esp_test.cpp
  src/test_main.cpp
  src/test_shared.c
)

target_link_libraries(fmt_transport_test
  CppUTest
  CppUTestExt
  fmt_transport
)
