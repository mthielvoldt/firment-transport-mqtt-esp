#include <CppUTest/TestHarness.h>
#include "CppUTestExt/MockSupport.h"

extern "C"
{
#include <fmt_transport.h>
#include <mqtt_client.h>
#include "test_shared.h"
#include <signal.h> // for debugging tests: raise(SIGINT);
}

TEST_GROUP(fmt_transport)
{
  void setup()
  {
  };
  void teardown()
  {
    mock().checkExpectations();
    mock().clear();
  };
};

TEST(fmt_transport, setup_succeeds)
{
  // CHecks just setup.
  mock().expectOneCall("esp_mqtt_client_init");
  mock().expectOneCall("esp_mqtt_client_start");
  fmt_initTransport();
}

TEST(fmt_transport, txChain_continues_untill_pullTx_false)
{
  mock().expectNCalls(3, "esp_mqtt_client_publish").andReturnValue(0);
  mock().expectNCalls(3, "pullTxPacket").andReturnValue(true);
  fmt_startTxChain();
  mock().expectOneCall("pullTxPacket").andReturnValue(false);
}

TEST(fmt_transport, dataEvent_callsPushRx_once)
{
  esp_mqtt_event_t event = {
    .event_id = MQTT_EVENT_DATA
  };
  mock().expectOneCall("pushRxPacket");
  esp_mqtt_dispatch_custom_event(client, &event);
}

TEST(fmt_transport, )
{
}

TEST(fmt_transport, )
{
}
