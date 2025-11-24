cmake_minimum_required(VERSION 3.31)

add_library(esp-mqtt
  mock_esp_mqtt.cpp
)

target_include_directories(esp-mqtt
  PUBLIC
    .
    ${PROJECT_BINARY_DIR}/esp-mqtt-src/include)

target_link_libraries(esp-mqtt
  CppUTestExt
)