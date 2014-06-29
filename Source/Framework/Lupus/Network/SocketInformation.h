#pragma once

#include <Lupus/Network/Enum.h>

namespace Lupus {
    enum class SocketInformationOption {
        None,
        Connected,
        Bound
    };

    extern "C" struct SocketInformation {
        SocketInformationOption Options;
        Vector<Byte> ProtocolInformation;
    };
}
