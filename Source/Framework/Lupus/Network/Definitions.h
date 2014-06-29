#pragma once

#include <Lupus/Definitions.h>

DefineError(socket_error)

#ifdef _MSC_VER

#pragma warning(disable: 4251)
#pragma warning(disable: 4996)
#pragma comment(lib, "Ws2_32.lib")

#include <WinSock2.h> // Header muss vor <Windows.h> inkludiert werden
#include <WS2tcpip.h> // Header muss vor <Windows.h> inkludiert werden

#define inet_pton InetPton
#define inet_ntop InetNtop
#define poll WSAPoll

#define LU_POLLIN POLLRDNORM
#define LU_POLLOUT POLLWRNORM
#define LU_POLLPRI POLLRDBAND

#define LU_SHUTDOWN_READ SD_RECEIVE
#define LU_SHUTDOWN_WRITE SD_SEND
#define LU_SHUTDOWN_BOTH SD_BOTH

#define GetLastSocketErrorString (std::strerror(WSAGetLastError()))
#define GetLastAddressInfoErrorString (std::strerror(WSAGetLastError()))

namespace Lupus {
    typedef SOCKET SocketHandle;

    namespace Internal {
        template <typename T>
        S32 GetSocketDomain(T h) throw(socket_error)
        {
            WSAPROTOCOL_INFO info;
            int size = sizeof(info);

            memset(&info, 0, sizeof(WSAPROTOCOL_INFO));

            if (getsockopt(h, SOL_SOCKET, SO_PROTOCOL_INFO, (char*)&info, &size) != 0) {
                throw socket_error(GetLastSocketErrorString);
            }

            return info.iAddressFamily;
        }

        template <typename T>
        S32 GetSocketProtocol(T h) throw(socket_error)
        {
            WSAPROTOCOL_INFO info;
            int size = sizeof(info);

            memset(&info, 0, sizeof(WSAPROTOCOL_INFO));

            if (getsockopt(h, SOL_SOCKET, SO_PROTOCOL_INFO, (char*)&info, (int*)&size) != 0) {
                throw socket_error(GetLastSocketErrorString);
            }

            return info.iProtocol;
        }
    }
}

#else

#include <cerrno>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>

#define closesocket close
#define ioctlsocket ioctl
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1

#define LU_POLLIN POLLIN
#define LU_POLLOUT POLLOUT
#define LU_POLLPRI POLLPRI

#define LU_SHUTDOWN_READ SHUT_RD
#define LU_SHUTDOWN_WRITE SHUT_WR
#define LU_SHUTDOWN_BOTH SHUT_RDWR

#define GetLastSocketErrorString (std::strerror(errno))
#define GetLastAddressInfoErrorString (gai_strerror(errno))

namespace Lupus {
    typedef int SocketHandle;
    typedef unsigned long u_long;

    namespace Internal {
        template <typename T>
        S32 GetSocketDomain(T h) throw(socket_error)
        {
            int domain = 0;
            socklen_t size = sizeof(int);

            memset(&info, 0, sizeof(WSAPROTOCOL_INFO));

            if (getsockopt(h, SOL_SOCKET, SO_DOMAIN, &domain, &size) != 0) {
                throw socket_error(GetLastSocketErrorString);
            }

            return domain;
        }

        template <typename T>
        S32 GetSocketProtocol(T h) throw(socket_error)
        {
            int protocol = 0;
            socklen_t size = sizeof(int);

            memset(&info, 0, sizeof(WSAPROTOCOL_INFO));

            if (getsockopt(h, SOL_SOCKET, SO_PROTOCOL, &protocol, &size) != 0) {
                throw socket_error(GetLastSocketErrorString);
            }

            return protocol;
        }
    }
}

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
    typedef socklen_t AddrLength;
    typedef addrinfo AddrInfo;
    typedef sockaddr Addr;
    typedef sockaddr_in AddrIn;
    typedef sockaddr_in6 AddrIn6;
    typedef sockaddr_storage AddrStorage;
}
