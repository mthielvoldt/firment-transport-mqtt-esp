#include <CppUTestExt/MockSupport.h>

extern "C" {
#include "esp_err.h"
#include "mock_esp_mqtt.h"
#include <stdint.h>
}

typedef int32_t esp_err_t;
typedef void (*event_handler_t)(void *handler_args, esp_mqtt_event_id_t base,
                                int32_t event_id, void *event_data);

struct esp_mqtt_client {
} client;
esp_event_handler_t handler = NULL;

esp_mqtt_client_handle_t
esp_mqtt_client_init(const esp_mqtt_client_config_t *config) {
  return (esp_mqtt_client_handle_t)mock()
      .actualCall("esp_mqtt_client_init")
      .returnPointerValueOrDefault(&client);
}

esp_err_t esp_mqtt_client_start(esp_mqtt_client_handle_t client) {
  return mock().actualCall("esp_mqtt_client_start").returnIntValue();
}

esp_err_t esp_mqtt5_client_set_connect_property(
    esp_mqtt5_client_handle_t client,
    const esp_mqtt5_connection_property_config_t *connect_property) {
  return mock()
      .actualCall("esp_mqtt5_client_set_connect_property")
      .returnIntValue();
}

int esp_mqtt_client_subscribe_single(esp_mqtt_client_handle_t client,
                                     const char *topic, int qos) {
  return mock()
      .actualCall("esp_mqtt_client_subscribe_single")
      .withStringParameter("topic", topic)
      .returnIntValue();
}

int esp_mqtt_client_publish(esp_mqtt_client_handle_t client, const char *topic,
                            const char *data, int len, int qos, int retain) {
  return mock()
      .actualCall("esp_mqtt_client_publish")
      .withIntParameter("len", len)
      .withStringParameter("topic", topic)
      .returnIntValue();
}

esp_err_t esp_mqtt_client_register_event(esp_mqtt_client_handle_t client,
                                         esp_mqtt_event_id_t event,
                                         esp_event_handler_t event_handler,
                                         void *event_handler_arg) {
  handler = event_handler;
  return mock().actualCall("esp_mqtt_client_register_event").returnIntValue();
}

/* Functions only needed by tests below. */

void test_sendFakeEvent(esp_mqtt_event_t *event) {
  if (handler) {
    ((event_handler_t)handler)(NULL, MQTT_EVENT_ANY, event->event_id, event);
  }
}