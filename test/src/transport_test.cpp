#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>

extern "C" {
#include "test_shared.h"
#include <fmt_sizes.h>
#include <fmt_transport.h>
#include <mock_esp_mqtt.h>
#include <signal.h> // for debugging tests: raise(SIGINT);
}

static bool pullTxPacket(uint8_t *const txBuffer);
static void pushRxPacket(const uint8_t *const rxData);

TEST_GROUP(fmt_transport) {
  void setup() {
    mock().disable();
    fmt_initTransport();
    fmt_linkTransport(pullTxPacket, pushRxPacket);
    mock().enable();
  };
  void teardown() {
    mock().checkExpectations();
    mock().clear();
  };
};

static bool pullTxPacket(uint8_t *const txBuffer) {
  return mock()
      .actualCall("pullTxPacket")
      .withOutputParameter("txBuffer", txBuffer)
      .returnBoolValue();
}

static void pushRxPacket(const uint8_t *const rxData) {
  mock().actualCall("pushRxPacket");
}

TEST_GROUP(fmt_transport_init) {
  void teardown() {
    mock().checkExpectations();
    mock().clear();
  };
};
TEST(fmt_transport_init, setup_succeeds) {
  mock().expectOneCall("esp_mqtt_client_init").andReturnValue((void *)1);
  mock()
      .expectOneCall("esp_mqtt5_client_set_connect_property")
      .andReturnValue(ESP_OK);
  mock().expectOneCall("esp_mqtt_client_register_event").andReturnValue(ESP_OK);
  mock().expectOneCall("esp_mqtt_client_start").andReturnValue(ESP_OK);
  CHECK_TRUE(fmt_initTransport());
}

TEST(fmt_transport, txChain_noPublish_if_pullTx_false) {
  mock()
      .expectOneCall("pullTxPacket")
      .withUnmodifiedOutputParameter("txBuffer")
      .andReturnValue(false);
  mock().expectNoCall("esp_mqtt_client_publish");
  fmt_startTxChain();
}

TEST(fmt_transport, txChain_continues_until_pullTx_false) {
  const int chainLen = 3;
  uint8_t packet[MAX_PACKET_SIZE_BYTES] = {};
  mock()
      .expectNCalls(1, "esp_mqtt_client_publish")
      .withIntParameter("len", (chainLen * MAX_PACKET_SIZE_BYTES))
      .andReturnValue(0);
  mock()
      .expectNCalls(chainLen, "pullTxPacket")
      .withOutputParameterReturning("txBuffer", packet, sizeof(packet))
      .andReturnValue(true);
  mock()
      .expectOneCall("pullTxPacket")
      .withUnmodifiedOutputParameter("txBuffer")
      .andReturnValue(false);
  fmt_startTxChain();
}

TEST(fmt_transport, dataEvent_callsPushRx_once) {
  const unsigned messageLen = 1;
  char packet[MAX_PACKET_SIZE_BYTES] = {messageLen};
  esp_mqtt_event_t event = {
      .event_id = MQTT_EVENT_DATA,
      .data = packet,
      .data_len = messageLen + 1,
  };
  mock().expectOneCall("pushRxPacket");
  test_sendFakeEvent(&event);
}

TEST(fmt_transport, subscribes_after_version_packet) {
  const char topicEdgeBound[] = "edgeBound";
  const char projectName[] = "fmtExMq";
  uint8_t versionPacket[MAX_PACKET_SIZE_BYTES] = {};

  mock()
      .expectOneCall("esp_mqtt_client_publish")
      .withIntParameter("len", (MAX_PACKET_SIZE_BYTES));
  mock()
      .expectOneCall("esp_mqtt_client_subscribe_single")
      .withStringParameter("topic", topicEdgeBound);
  mock()
      .expectOneCall("pullTxPacket")
      .withOutputParameterReturning("txBuffer", versionPacket,
                                    sizeof(versionPacket))
      .andReturnValue(true);
  mock()
      .expectOneCall("pullTxPacket")
      .withUnmodifiedOutputParameter("txBuffer")
      .andReturnValue(false);
  fmt_startTxChain();
}

/*
TEST(fmt_transport, tx_packets_aggregated)
{
}

TEST(fmt_transport, )
{
}
*/
