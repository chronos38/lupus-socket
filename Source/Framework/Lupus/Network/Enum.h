#pragma once

#include <Lupus/Network/Definitions.h>

namespace Lupus {
    //! Sockettyp
    enum class SocketType {
        Unspecified = 0, //!< Typ ist nicht spezifiziert und von daher variabl.
        SeqPacket = SOCK_SEQPACKET, //!< Squenzpaket-Stream Socket
        Stream = SOCK_STREAM, //!< Streaming Socket (TCP).
        Datagram = SOCK_DGRAM, //!< Datagram Socket (UDP).
        Rdm = SOCK_RDM, //!< reliably-delivered message.
        Raw = SOCK_RAW //!< Socket verwendet normale NAT Pakete.
    };

    //! Netzwerkprotokoll
    enum class ProtocolType {
        Unspecified = 0, //!< Protokoll ist nicht spezifiziert und von daher variabl.
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
        Unspecified = AF_UNSPEC, //!< Domäne ist nicht spezifiziert und von daher variabl.
        InterNetwork = AF_INET, //!< IPv4
        InterNetworkV6 = AF_INET6, //!< IPv6
        AppleTalk = AF_APPLETALK,
        UNIX = AF_UNIX,
        IPX = AF_IPX
    };

    enum class SocketPollFlags : short {
        Read = LU_POLLIN, //!< Überprüft ob Daten zum lesen vorhanden sind.
        Write = LU_POLLOUT, //!< Überprüft ob Daten zum schreiben vorhanden sind.
        OutOfBand = LU_POLLPRI, //!< Überprüft ob Out-Of-Band Daten vorhanden sind.
        Error = POLLERR,
        HungUp = POLLHUP,
        Invalid = POLLNVAL,
        Timeout
    };

    inline SocketPollFlags operator|(SocketPollFlags lhs, SocketPollFlags rhs)
    {
        return static_cast<SocketPollFlags>(static_cast<short>(lhs) | static_cast<short>(rhs));
    }

    inline SocketPollFlags operator&(SocketPollFlags lhs, SocketPollFlags rhs)
    {
        return static_cast<SocketPollFlags>(static_cast<short>(lhs)& static_cast<short>(rhs));
    }

    inline SocketPollFlags& operator|=(SocketPollFlags& lhs, SocketPollFlags rhs)
    {
        lhs = static_cast<SocketPollFlags>(static_cast<short>(lhs) | static_cast<short>(rhs));
        return lhs;
    }

    inline SocketPollFlags& operator&=(SocketPollFlags& lhs, SocketPollFlags rhs)
    {
        lhs = static_cast<SocketPollFlags>(static_cast<short>(lhs)& static_cast<short>(rhs));
        return lhs;
    }

    enum class SocketShutdown {
        Receive = LU_SHUTDOWN_READ, //!< Schließt die Leseverbindung.
        Send = LU_SHUTDOWN_WRITE, //!< Schließt die Schreibverbindung.
        Both = LU_SHUTDOWN_BOTH //!< Schließt sowohl Lese- als auch Schreibverbindung.
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
