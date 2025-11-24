#ifndef MOCK_ESP_MQTT_H
#define MOCK_ESP_MQTT_H

#include <mqtt5_client.h>

void test_sendFakeEvent(esp_mqtt_event_id_t eventType);

#endif // MOCK_ESP_MQTT_H