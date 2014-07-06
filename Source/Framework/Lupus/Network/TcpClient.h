#pragma once

#include <Lupus/Network/Enum.h>

namespace Lupus {
    class NetworkStream;
    class IPAddress;
    class IPEndPoint;
    class Socket;

    class TcpClient : public ReferenceType
    {
    public:

        TcpClient() NOEXCEPT;
        TcpClient(AddressFamily family) throw(std::invalid_argument);
        TcpClient(Pointer<IPEndPoint> endPoint) throw(null_pointer);
        TcpClient(const String& hostname, U16 port) throw(socket_error);

        virtual bool Active() const NOEXCEPT;
        virtual void Active(bool) NOEXCEPT;
        virtual U32 Available() const throw(socket_error);
        virtual Pointer<Socket> Client() const NOEXCEPT;
        virtual void Client(Pointer<Socket>) throw(null_pointer);
        virtual bool IsConnected() const NOEXCEPT;
        virtual bool ExclusiveAddressUse() const throw(socket_error);
        virtual void ExclusiveAddressUse() throw(socket_error);
        virtual bool NoDelay() const NOEXCEPT;
        virtual void NoDelay(bool) NOEXCEPT;
        virtual S32 SendBuffer() const throw(socket_error);
        virtual void SendBuffer(S32) throw(socket_error);
        virtual S32 ReceiveBuffer() const throw(socket_error);
        virtual void ReceiveBuffer(S32) throw(socket_error);
        virtual S32 SendTimeout() const NOEXCEPT;
        virtual void SendTimeout(S32) throw(socket_error);
        virtual S32 ReceiveTimeout() const NOEXCEPT;
        virtual void ReceiveTimeout(S32) throw(socket_error);

        virtual void Close() throw(socket_error);
        virtual void Connect(Pointer<IPEndPoint>) throw(null_pointer, socket_error);
        virtual void Connect(Pointer<IPAddress>, U16 port) throw(null_pointer, socket_error);
        virtual void Connect(const Vector<Pointer<IPEndPoint>>& endPoints) throw(null_pointer);
        virtual void Connect(const String& host, U16 port) throw(socket_error, std::invalid_argument);

        virtual Pointer<NetworkStream> GetStream() const NOEXCEPT;
    };
}
