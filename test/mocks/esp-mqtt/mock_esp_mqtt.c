#include "mock_esp_mqtt.h"
#include <stdint.h>

typedef int32_t esp_err_t;

// struct esp_mqtt_client client = {
// };

esp_mqtt_client_handle_t
esp_mqtt_client_init(const esp_mqtt_client_config_t *config)
{
  return (esp_mqtt_client_handle_t)1;
}

esp_err_t esp_mqtt5_client_set_connect_property(esp_mqtt5_client_handle_t client, const esp_mqtt5_connection_property_config_t *connect_property)
{
  return 1;
}

int esp_mqtt_client_subscribe_single(esp_mqtt_client_handle_t client,
                                     const char *topic, int qos)
{
}

int esp_mqtt_client_publish(esp_mqtt_client_handle_t client, const char *topic,
                            const char *data, int len, int qos, int retain)
{
}

esp_err_t esp_mqtt_client_register_event(esp_mqtt_client_handle_t client,
                                         esp_mqtt_event_id_t event,
                                         esp_event_handler_t event_handler,
                                         void *event_handler_arg)
{
}

// Likely not used because fmt_base already has a queue.
int esp_mqtt_client_enqueue(esp_mqtt_client_handle_t client, const char *topic,
                            const char *data, int len, int qos, int retain,
                            bool store)
{
}

/* Functions only needed by tests below. */

esp_err_t esp_mqtt_dispatch_custom_event(esp_mqtt_client_handle_t client, esp_mqtt_event_t *event)
{
  return 0;
}

void test_sendFakeEvent(esp_mqtt_event_id_t eventType)
{
  esp_mqtt_event_t event = {
      .event_id = MQTT_EVENT_DATA,
  };
}