cmake_minimum_required(VERSION 3.25)
set(CMAKE_POLICY_VERSION_MINIMUM 3.25)

add_compile_options("-g")

project(fmt_transport_mqtt_esp_test LANGUAGES CXX C)

include(FetchContent)
FetchContent_Declare(
    CppUTest
    GIT_REPOSITORY https://github.com/cpputest/cpputest.git
    GIT_TAG v4.0
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(CppUTest)


# add_subdirectory(fakes/queue) # -> provides 
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
