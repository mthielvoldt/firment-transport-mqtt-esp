#include <fmt_transport.h>
#include <fmt_sizes.h>
#include <fmt_transport_config.h>
#include <mqtt_client.h>
#include <stddef.h>

#define MQTT_BUFFER_SIZE (MAX_PACKET_SIZE_BYTES * 100)

static transportErrCount_t errs = {};
static txCallback_t _pullTxPacket = NULL;
static rxCallback_t _pushRxPacket = NULL;

static esp_mqtt_client_handle_t client;
static uint8_t mqttBuffer[MQTT_BUFFER_SIZE];
static uint32_t mqttWritePos = 0;
static uint32_t lastResetTimeUs = 0;
static char topicHqBound[36] = "";
static char topicEdgeBound[36] = "";
static const char *TAG = "mqtt";




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
  if (_pullTxPacket && _pullTxPacket(txPacket))
  {

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
  // client = esp_mqtt_client_init(&mqtt5_cfg);
}