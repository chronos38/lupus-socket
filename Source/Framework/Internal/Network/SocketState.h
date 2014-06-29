#pragma once

#include <Lupus/Network/Enum.h>

namespace Lupus {
    struct SocketInformation;
    class IPEndPoint;
    class IPAddress;
    class Socket;

    namespace Internal {
        class SocketState : public ReferenceType
        {
        public:
            virtual ~SocketState() = default;

            virtual Pointer<Socket> Accept(Socket* socket) throw(socket_error);
            virtual void Bind(Socket* socket, Pointer<IPEndPoint> localEndPoint) throw(socket_error);
            virtual void Close(Socket* socket) throw(socket_error);
            virtual void Close(Socket* socket, U32 timeout) throw(socket_error);
            virtual void Connect(Socket* socket, Pointer<IPEndPoint> remoteEndPoint) throw(socket_error, null_pointer);
            virtual void Disconnect(Socket* socket, bool reuseSocket) throw(socket_error);
            virtual void Listen(Socket* socket, U32 backlog) throw(socket_error);
            virtual S32 Receive(Socket* socket, Vector<Byte>& buffer, U32 offset, U32 size, SocketFlags socketFlags, SocketError& errorCode) throw(socket_error, std::out_of_range);
            virtual S32 ReceiveFrom(Socket* socket, Vector<Byte>& buffer, U32 offset, U32 size, SocketFlags socketFlags, Pointer<IPEndPoint>& remoteEndPoint) throw(socket_error, std::out_of_range);
            virtual S32 Send(Socket* socket, const Vector<Byte>& buffer, U32 offset, U32 size, SocketFlags socketFlags, SocketError& errorCode) throw(socket_error, std::out_of_range);
            virtual S32 SendTo(Socket* socket, const Vector<Byte>& buffer, U32 offset, U32 size, SocketFlags socketFlags, Pointer<IPEndPoint> remoteEndPoint) throw(socket_error, std::out_of_range);
            virtual void Shutdown(Socket* socket, SocketShutdown how) throw(socket_error);

        protected:

            Pointer<Socket> CreateSocket(SocketHandle, AddrStorage) NOEXCEPT;

            void ChangeState(Socket* socket, Pointer<SocketState> state) NOEXCEPT;
            void SetRemoteEndPoint(Socket* socket, Pointer<IPEndPoint>) NOEXCEPT;
            void SetLocalEndPoint(Socket* socket, Pointer<IPEndPoint>) NOEXCEPT;
            Pointer<IPEndPoint> GetRemoteEndPoint(Socket* socket) const NOEXCEPT;
            Pointer<IPEndPoint> GetLocalEndPoint(Socket* socket) const NOEXCEPT;

            void SetConnected(Socket*, bool) NOEXCEPT;
            void SetBound(Socket*, bool) NOEXCEPT;
        };

        class SocketBound : public SocketState
        {
        public:
            SocketBound(Socket*);
            SocketBound(Socket*, Pointer<IPEndPoint>);
            virtual ~SocketBound() = default;

            virtual void Connect(Socket* socket, Pointer<IPEndPoint> remoteEndPoint) throw(socket_error, null_pointer) override;
            virtual void Listen(Socket* socket, U32 backlog) throw(socket_error) override;
        };

        class SocketListen : public SocketState
        {
        public:
            SocketListen(Socket*);
            virtual ~SocketListen() = default;

            virtual Pointer<Socket> Accept(Socket* socket) throw(socket_error);
        };

        class SocketConnected : public SocketState
        {
        public:
            SocketConnected(Socket*);
            SocketConnected(Socket*, Pointer<IPEndPoint>);
            virtual ~SocketConnected() = default;

            virtual void Disconnect(Socket* socket, bool reuseSocket) throw(socket_error) override;
            virtual S32 Receive(Socket* socket, Vector<Byte>& buffer, U32 offset, U32 size, SocketFlags socketFlags, SocketError& errorCode) throw(socket_error, std::out_of_range) override;
            virtual S32 ReceiveFrom(Socket* socket, Vector<Byte>& buffer, U32 offset, U32 size, SocketFlags socketFlags, Pointer<IPEndPoint>& remoteEndPoint) throw(socket_error, std::out_of_range) override;
            virtual S32 Send(Socket* socket, const Vector<Byte>& buffer, U32 offset, U32 size, SocketFlags socketFlags, SocketError& errorCode) throw(socket_error, std::out_of_range) override;
            virtual S32 SendTo(Socket* socket, const Vector<Byte>& buffer, U32 offset, U32 size, SocketFlags socketFlags, Pointer<IPEndPoint> remoteEndPoint) throw(socket_error, std::out_of_range) override;
            virtual void Shutdown(Socket* socket, SocketShutdown how) throw(socket_error) override;
        };

        class SocketDisconnected : public SocketState
        {
        public:
            SocketDisconnected(Socket*);
            virtual ~SocketDisconnected() = default;

            virtual void Connect(Socket* socket, Pointer<IPEndPoint> remoteEndPoint) throw(socket_error, null_pointer);
            virtual void Shutdown(Socket* socket, SocketShutdown how) throw(socket_error);

        };

        class SocketClosed : public SocketState
        {
        public:
            SocketClosed(Socket*);
            virtual ~SocketClosed() = default;

            virtual void Bind(Socket* socket, Pointer<IPEndPoint> localEndPoint) throw(socket_error) override;
            virtual void Connect(Socket* socket, Pointer<IPEndPoint> remoteEndPoint) throw(socket_error, null_pointer) override;
        };
    }
}
