#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>

extern "C"
{
#include <fmt_transport.h>
#include <mock_esp_mqtt.h>
#include "test_shared.h"
#include <signal.h> // for debugging tests: raise(SIGINT);
}

TEST_GROUP(fmt_transport)
{

  void setup()
  {
    fmt_initTransport();
  };
  void teardown()
  {
    mock().checkExpectations();
    mock().clear();
  };
};

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
/*
TEST(fmt_transport, txChain_continues_untill_pullTx_false)
{
  mock().expectNCalls(3, "esp_mqtt_client_publish").andReturnValue(0);
  mock().expectNCalls(3, "pullTxPacket").andReturnValue(true);
  fmt_startTxChain();
  mock().expectOneCall("pullTxPacket").andReturnValue(false);
}


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
