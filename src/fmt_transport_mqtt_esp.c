#include <fmt_transport.h>
#include <stddef.h>

#define MAX_PACKET_SIZE 1

static transportErrCount_t errs = {};
static txCallback_t _pullTxPacket = NULL;
static rxCallback_t _pushRxPacket = NULL;

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
  uint8_t txPacket[MAX_PACKET_SIZE];
  if (_pullTxPacket(txPacket))
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
}