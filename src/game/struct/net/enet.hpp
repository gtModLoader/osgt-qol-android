#include <cstdint>

struct ENetPacket
{
    size_t refCount;
    uint32_t flags;
    uint8_t* data;
    size_t dataLength;
    void* freeCallback;
    void* userData;
};