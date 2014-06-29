#pragma once

#include <Lupus/Definitions.h>

#ifdef _MSC_VER

#pragma warning(disable: 4251)
#pragma warning(disable: 4996)
#pragma comment(lib, "Ws2_32.lib")

#include <WinSock2.h> // Header muss vor <Windows.h> inkludiert werden
#include <WS2tcpip.h> // Header muss vor <Windows.h> inkludiert werden

namespace Lupus {
    typedef SOCKET SocketHandle;
    typedef int AddrLength;
}

#define inet_pton InetPton
#define inet_ntop InetNtop
#define poll WSAPoll

#define LU_POLLIN POLLRDNORM
#define LU_POLLOUT POLLWRNORM
#define LU_POLLPRI POLLRDBAND

#define GetLastSocketErrorString (std::strerror(WSAGetLastError()))
#define GetLastAddressInfoErrorString (std::strerror(WSAGetLastError()))

#else

#include <cerrno>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>

namespace Lupus {
    typedef int SocketHandle;
    typedef socklen_t AddrLength;
    typedef unsigned long u_long;
}

#define closesocket close
#define ioctlsocket ioctl
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1

#define LU_POLLIN POLLIN
#define LU_POLLOUT POLLOUT
#define LU_POLLPRI POLLPRI

#define GetLastSocketErrorString (std::strerror(errno))
#define GetLastAddressInfoErrorString (gai_strerror(errno))

#if defined(__linux__)

#include <endian.h>

#elif defined(__FreeBSD__) || defined(__NetBSD__)

#include <sys/endian.h>

#elif defined(__OpenBSD__)

#define be16toh(x) betoh16(x)
#define be32toh(x) betoh32(x)
#define be64toh(x) betoh64(x)

#endif

#define ntohll be64toh
#define htonll htobe64

#endif

namespace Lupus {
    typedef addrinfo AddrInfo;
    typedef sockaddr Addr;
    typedef sockaddr_in AddrIn;
    typedef sockaddr_in6 AddrIn6;
    typedef sockaddr_storage AddrStorage;
}

DefineError(socket_error)
