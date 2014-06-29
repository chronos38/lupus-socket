#pragma once

#include <Lupus/Network/Definitions.h>

namespace Lupus {
    //! Sockettyp
    enum class SocketType {
        Unspecified = 0,
        SeqPacket = SOCK_SEQPACKET,
        Stream = SOCK_STREAM,
        Datagram = SOCK_DGRAM,
        Rdm = SOCK_RDM,
        Raw = SOCK_RAW
    };

    //! Netzwerkprotokoll
    enum class ProtocolType {
        Unspecified = 0,
        IP = IPPROTO_IP,
        IPv4 = IPPROTO_IPV4,
        IPv6 = IPPROTO_IPV6,
        TCP = IPPROTO_TCP,
        UDP = IPPROTO_UDP,
        ICMP = IPPROTO_ICMP,
        IGMP = IPPROTO_IGMP,
        EGP = IPPROTO_EGP,
        PUP = IPPROTO_PUP,
        IDP = IPPROTO_IDP,
        Routing = IPPROTO_ROUTING,
        Fragment = IPPROTO_FRAGMENT,
        ESP = IPPROTO_ESP,
        AH = IPPROTO_AH,
        ICMPv6 = IPPROTO_ICMPV6,
        None = IPPROTO_NONE,
        PIM = IPPROTO_PIM,
        SCTP = IPPROTO_SCTP,
        Raw = IPPROTO_RAW,
        IPv6HopByHopOptions = IPPROTO_HOPOPTS,
        IPv6DestinationOptions = IPPROTO_DSTOPTS
    };

    //! Adressart
    enum class AddressFamily {
        Unspecified = AF_UNSPEC,
        InterNetwork = AF_INET,
        InterNetworkV6 = AF_INET6,
        AppleTalk = AF_APPLETALK,
        UNIX = AF_UNIX,
        IPX = AF_IPX
    };

    enum class SelectMode {
        Read = LU_POLLIN,
        Write = LU_POLLOUT,
        Error = POLLERR,
        OutOfBand = LU_POLLPRI,
        HungUp = POLLHUP,
        Invalid = POLLNVAL
    };

    enum class PollResult {
        Timeout = -1,
        False = 0,
        True = 1
    };

    enum class SocketShutdown {
        Receive = LU_SHUTDOWN_READ,
        Send = LU_SHUTDOWN_WRITE,
        Both = LU_SHUTDOWN_BOTH
    };

    enum class SocketFlags {
        None = 0,
        OutOfBand = MSG_OOB,
        Peek = MSG_PEEK,
        DontRoute = MSG_DONTROUTE,
        Truncated = MSG_CTRUNC
    };

    inline SocketFlags operator|(SocketFlags lhs, SocketFlags rhs)
    {
        return static_cast<SocketFlags>(static_cast<int>(lhs) | static_cast<int>(rhs));
    }

    inline SocketFlags operator&(SocketFlags lhs, SocketFlags rhs)
    {
        return static_cast<SocketFlags>(static_cast<int>(lhs) & static_cast<int>(rhs));
    }

    inline SocketFlags& operator|=(SocketFlags& lhs, SocketFlags rhs)
    {
        lhs = static_cast<SocketFlags>(static_cast<int>(lhs) | static_cast<int>(rhs));
        return lhs;
    }

    inline SocketFlags& operator&=(SocketFlags& lhs, SocketFlags rhs)
    {
        lhs = static_cast<SocketFlags>(static_cast<int>(lhs) & static_cast<int>(rhs));
        return lhs;
    }

    enum SocketError {
        // TODO: Werte eintragen
    };
}
