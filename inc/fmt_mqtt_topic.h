#ifndef FMT_MQTT_TOPIC_H
#define FMT_MQTT_TOPIC_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief set MQTT topic for specified messages to be published to.
 * @param[in] messageTag The field numeber of the sub-message this topic will
 * be used for.  If @p messageTag = 0, the topic will be a default for publish.
 * @param[in] topic A pointer to the topic string.  The string should reside in
 * static or read-only memory so it can be accessed any time.
 */
bool fmt_setPublishTopic(uint32_t messageTag, const char *topic);

/**
 * @brief Adds a topic to the list of those subscribed to.
 * @param[in] topic A pointer to the topic string.  The string should reside in
 * static or read-only memory so it can be accessed any time.
 */
bool fmt_mqttSubscribe(const char *topic);

#endif // FMT_MQTT_TOPIC_H