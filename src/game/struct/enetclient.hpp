#pragma once
#include <cstdint>

class ENetClient
{
  public:
    uint8_t pad[136];
    void* m_pEnetPeer;// 136 peer
};