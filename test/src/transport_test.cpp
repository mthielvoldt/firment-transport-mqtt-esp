#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>

extern "C"
{
#include <fmt_transport.h>
#include <mock_esp_mqtt.h>
#include "test_shared.h"
#include <signal.h> // for debugging tests: raise(SIGINT);
}

static bool pullTxPacket(uint8_t *const txBuffer);
static void pushRxPacket(const uint8_t *const rxData);


TEST_GROUP(fmt_transport)
{
  void setup()
  {
    mock().disable();
    fmt_initTransport();
    fmt_linkTransport(pullTxPacket, pushRxPacket);
    mock().enable();
  };
  void teardown()
  {
    mock().checkExpectations();
    mock().clear();
  };
};

static bool pullTxPacket(uint8_t *const txBuffer)
{
  return mock().actualCall("pullTxPacket").returnBoolValue();
}

static void pushRxPacket(const uint8_t *const rxData)
{
  mock().actualCall("pushRxPacket");
}

TEST_GROUP(fmt_transport_init)
{
  void teardown()
  {
    mock().checkExpectations();
    mock().clear();
  };
};
TEST(fmt_transport_init, setup_succeeds)
{
  mock().expectOneCall("esp_mqtt_client_init").andReturnValue((void *)1);
  mock().expectOneCall("esp_mqtt5_client_set_connect_property").andReturnValue(ESP_OK);
  mock().expectOneCall("esp_mqtt_client_register_event").andReturnValue(ESP_OK);
  mock().expectOneCall("esp_mqtt_client_start").andReturnValue(ESP_OK);
  CHECK_TRUE(fmt_initTransport());
}

TEST(fmt_transport, txChain_continues_untill_pullTx_false)
{
  mock().expectNCalls(3, "esp_mqtt_client_publish").andReturnValue(0);
  mock().expectNCalls(3, "pullTxPacket").andReturnValue(true);
  mock().expectOneCall("pullTxPacket").andReturnValue(false);
  fmt_startTxChain();
}

/*
TEST(fmt_transport, dataEvent_callsPushRx_once)
{
mock().expectOneCall("pushRxPacket");
test_sendFakeEvent(MQTT_EVENT_DATA);
}

TEST(fmt_transport, tx_packets_aggregated)
{
}

TEST(fmt_transport, )
{
}
*/
