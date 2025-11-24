#include <mqtt5_client.h>
#include <stdint.h>

typedef int32_t esp_err_t;

esp_err_t esp_mqtt5_client_set_connect_property(esp_mqtt5_client_handle_t client, const esp_mqtt5_connection_property_config_t *connect_property)
{
  return 1;
}

int esp_mqtt_client_subscribe_single(esp_mqtt_client_handle_t client,
                                     const char *topic, int qos)
                                     {}

int esp_mqtt_client_publish(esp_mqtt_client_handle_t client, const char *topic,
                            const char *data, int len, int qos, int retain)
{
}

esp_err_t esp_mqtt_client_register_event(esp_mqtt_client_handle_t client,
                                         esp_mqtt_event_id_t event,
                                         esp_event_handler_t event_handler,
                                         void *event_handler_arg)
                                         {}

// Likely not used because fmt_base already has a queue.
int esp_mqtt_client_enqueue(esp_mqtt_client_handle_t client, const char *topic,
                            const char *data, int len, int qos, int retain,
                            bool store)
{
}