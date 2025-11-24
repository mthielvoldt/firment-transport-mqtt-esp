#include <fmt_transport.h>
#include <fmt_sizes.h>
#include <fmt_transport_config.h>
#include <mqtt5_client.h>
#include <esp_log.h>
#include <esp_err.h>
#include <stddef.h>

#define MQTT_BUFFER_SIZE (MAX_PACKET_SIZE_BYTES * 100)

static transportErrCount_t errs = {};
static txCallback_t _pullTxPacket = NULL;
static rxCallback_t _pushRxPacket = NULL;

static esp_mqtt_client_handle_t client = NULL;
static uint8_t mqttBuffer[MQTT_BUFFER_SIZE];
static uint32_t mqttWritePos = 0;
static uint32_t lastResetTimeUs = 0;
static char topicHqBound[36] = "";
static char topicEdgeBound[36] = "";
static const char *TAG = "mqtt";

static void unpackAndPushToApp(uint8_t *packed, int len);
static void mqtt5_event_handler(
    void *handler_args, esp_mqtt_event_id_t base, int32_t event_id,
    void *event_data);

static bool fmt_linkTransport_prod(txCallback_t pullTxPacket, rxCallback_t pushRxPacket)
{
  if (pullTxPacket && pushRxPacket)
  {
    _pullTxPacket = pullTxPacket;
    _pushRxPacket = pushRxPacket;
    return true;
  }
  return false;
}
fmt_linkTransport_t fmt_linkTransport = fmt_linkTransport_prod;

void fmt_startTxChain_prod(void)
{
  uint8_t txPacket[MAX_PACKET_SIZE_BYTES];
  if (!_pullTxPacket || !client)
    return;
  
  while (_pullTxPacket(txPacket))
  {
    esp_mqtt_client_publish(
        client, topicHqBound, (char *)mqttBuffer, mqttWritePos, 1, 1);
  }
}
fmt_startTxChain_t fmt_startTxChain = fmt_startTxChain_prod;

const transportErrCount_t *fmt_getTransportErrCount_prod(void)
{
  return &errs;
}
fmt_getTransportErrCount_t fmt_getTransportErrCount =
    fmt_getTransportErrCount_prod;

bool fmt_initTransport(void)
{
  esp_mqtt5_connection_property_config_t connect_property = {
      .session_expiry_interval = 10,
      .maximum_packet_size = 1024,
      .receive_maximum = 65535,
      .topic_alias_maximum = 2,
      .request_resp_info = true,
      .request_problem_info = true,
      .will_delay_interval = 10,
      .payload_format_indicator = true,
      .message_expiry_interval = 10,
      .response_topic = "/test/response",
      .correlation_data = "123456",
      .correlation_data_len = 6,
  };

  esp_mqtt_client_config_t mqtt5_cfg = {
      .broker.address.uri = CONFIG_BROKER_URL,
      .session.protocol_ver = MQTT_PROTOCOL_V_5,
      .network.disable_auto_reconnect = true,
      .credentials.username = "123",
      .credentials.authentication.password = "456",
      .session.last_will.topic = "/topic/will",
      .session.last_will.msg = "i will leave",
      .session.last_will.msg_len = 12,
      .session.last_will.qos = 1,
      .session.last_will.retain = true,
  };

  // Creates event loop.
  client = esp_mqtt_client_init(&mqtt5_cfg);

  ESP_ERROR_CHECK(
      esp_mqtt5_client_set_connect_property(client, &connect_property));

  /* The last argument may be used to pass data to the event handler */
  ESP_ERROR_CHECK(esp_mqtt_client_register_event(
      client, MQTT_EVENT_ANY, mqtt5_event_handler, NULL));

  ESP_ERROR_CHECK(esp_mqtt_client_start(client));
  return true;
}

static void mqtt5_event_handler(void *handler_args, esp_mqtt_event_id_t base, int32_t event_id, void *event_data)
{
  esp_mqtt_event_handle_t event = event_data;
  esp_mqtt_client_handle_t client = event->client;

  switch ((esp_mqtt_event_id_t)event_id)
  {
  case MQTT_EVENT_CONNECTED:
    ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
    break;
  case MQTT_EVENT_DISCONNECTED:
    ESP_LOGI(TAG, "DISCONNECTED");
    break;
  case MQTT_EVENT_SUBSCRIBED:
    ESP_LOGI(TAG, "SUBSCRIBED, msg_id=%d", event->msg_id);
    break;
  case MQTT_EVENT_UNSUBSCRIBED:
    ESP_LOGI(TAG, "UNSUBSCRIBED, msg_id=%d", event->msg_id);
    break;
  case MQTT_EVENT_PUBLISHED:
    break;
  case MQTT_EVENT_DATA:
    unpackAndPushToApp((uint8_t *)(event->data), event->data_len);
    break;
  case MQTT_EVENT_ERROR:
    ESP_LOGI(TAG, "MQTT_EVENT_ERROR, return code %d", event->error_handle->connect_return_code);
    if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT)
    {
      if (event->error_handle->esp_tls_last_esp_err)
        ESP_LOGE(TAG, "reported from esp-tls");
      if (event->error_handle->esp_tls_stack_err)
        ESP_LOGE(TAG, "reported from tls stack");
      if (event->error_handle->esp_transport_sock_errno)
        ESP_LOGE(TAG, "captured as transport's socket errno");
      ESP_LOGI(TAG, "Last errno string (%s)", strerror(event->error_handle->esp_transport_sock_errno));
    }
    break;
  default:
    ESP_LOGI(TAG, "Other event id:%d", event->event_id);
    break;
  }
}

static void unpackAndPushToApp(uint8_t *packed, int len)
{
  size_t packetSize;
  uint8_t packet[MAX_PACKET_SIZE_BYTES];

  // needs revision if max message length set > 255.
  int msgPos = 0;
  uint8_t msgSize = packed[msgPos + LENGTH_POSITION] + LENGTH_SIZE_BYTES;

  while (
      msgPos + msgSize <= len && // prevent reading past buffer.
      msgSize > 1)               // quit if we encounter 0-length msg.
  {
    memcpy(packet, &packed[msgPos], msgSize);
    // packetSize = appendCRC(packet);  // placeholder
    _pushRxPacket(packet);
    msgPos += msgSize;
    msgSize = packed[msgPos + LENGTH_POSITION] + LENGTH_SIZE_BYTES;
  }
}