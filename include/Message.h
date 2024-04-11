#ifndef DW_MESSAGE
#define DW_MESSAGE

#include <cstdint>
#include <vector>

namespace dw {

template<typename MessageType>
struct MessageHeader {
    MessageType type{};
    std::int32_t size;
};

template<typename MessageType>
struct Message {
    MessageHeader<MessageType> header;
    std::vector<std::uint8_t> body;
};

}

#endif