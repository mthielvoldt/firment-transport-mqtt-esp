cmake_minimum_required(VERSION 3.31)

include(FetchContent)
FetchContent_Declare(
  fmt-transport-iface
  GIT_REPOSITORY https://github.com/mthielvoldt/firment-transport-iface.git
  GIT_TAG v0.13.0
)
FetchContent_MakeAvailable(fmt-transport-iface)

add_library(fmt_transport
  src/fmt_transport_mqtt_esp.c
)

## Possibly not needed: interface is a separate target.
# target_include_directories(fmt_transport
#   PUBLIC
#     inc
# )

# The parent project must provide esp-idf.
target_link_libraries(fmt_transport 
  fmt_transport_iface
  esp-mqtt
)