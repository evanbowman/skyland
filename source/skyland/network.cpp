#include "network.hpp"



namespace skyland {



namespace network {



void poll_messages(Platform& pfrm, App& app, Listener& listener)
{
    while (auto message = pfrm.network_peer().poll_message()) {
        if (message->length_ < sizeof(Header)) {
            return;
        }
        Header header;
        memcpy(&header, message->data_, sizeof header);

        switch (header.message_type_) {
        case Header::null:
            pfrm.network_peer().poll_consume(sizeof(Header));
            continue;

#define HANDLE_MESSAGE(MESSAGE_TYPE)                                           \
    case MESSAGE_TYPE::mt: {                                                   \
        NET_EVENT_SIZE_CHECK(MESSAGE_TYPE)                                     \
        if (message->length_ < sizeof(MESSAGE_TYPE)) {                         \
            return;                                                            \
        }                                                                      \
        MESSAGE_TYPE m;                                                        \
        memcpy(&m, message->data_, sizeof m);                                  \
        pfrm.network_peer().poll_consume(sizeof(MESSAGE_TYPE));                \
        listener.receive(pfrm, app, m);                                        \
        continue;                                                              \
    }

            HANDLE_MESSAGE(RoomConstructed)
            HANDLE_MESSAGE(RoomSalvaged)
        }

        error(pfrm, "garbled message!?");
        pfrm.network_peer().disconnect();
        return;
    }
}



}



}
