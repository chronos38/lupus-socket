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
            virtual void Close(Socket* socket, U32 timeout) NOEXCEPT;
            virtual void Connect(Socket* socket, Pointer<IPEndPoint> remoteEndPoint) throw(socket_error, null_pointer);
            virtual void Disconnect(Socket* socket, bool reuseSocket) throw(socket_error);
            virtual void Listen(Socket* socket, U32 backlog) throw(socket_error);
            virtual bool Poll(Socket* socket, U32 microSeconds, SelectMode mode) throw(socket_error);
            virtual S32 Receive(Socket* socket, Vector<Byte>& buffer, U32 offset, U32 size, SocketFlags socketFlags, SocketError& errorCode) throw(socket_error);
            virtual S32 ReceiveFrom(Socket* socket, Vector<Byte>& buffer, U32 offset, U32 size, SocketFlags socketFlags, Pointer<IPEndPoint>& remoteEndPoint) throw(socket_error);
            virtual S32 Send(Socket* socket, const Vector<Byte>& buffer, U32 offset, U32 size, SocketFlags socketFlags, SocketError& errorCode) throw(socket_error);
            virtual S32 SendTo(Socket* socket, const Vector<Byte>& buffer, U32 offset, U32 size, SocketFlags socketFlags, Pointer<IPEndPoint> remoteEndPoint) throw(socket_error);
            virtual void Shutdown(Socket* socket, SocketShutdown how) throw(socket_error);

            virtual bool IsConnected() const NOEXCEPT;
            virtual bool IsBound() const NOEXCEPT;
            virtual bool IsListening() const NOEXCEPT;

        protected:

            void ChangeState(Socket* socket, Pointer<SocketState> state);
            void SetRemoteEndPoint(Socket* socket, Pointer<IPEndPoint>);
            void SetLocalEndPoint(Socket* socket, Pointer<IPEndPoint>);
            Pointer<IPEndPoint> GetRemoteEndPoint(Socket* socket) const;
            Pointer<IPEndPoint> GetLocalEndPoint(Socket* socket) const;
        };

        class SocketBound : public SocketState
        {
        public:
            virtual ~SocketBound() = default;

            virtual bool IsBound() const NOEXCEPT override;

            virtual void Connect(Socket* socket, Pointer<IPEndPoint> remoteEndPoint) throw(socket_error, null_pointer) override;
            virtual void Listen(Socket* socket, U32 backlog) throw(socket_error) override;
        };

        class SocketListen : public SocketState
        {
        public:
            virtual ~SocketListen() = default;

            virtual bool IsBound() const NOEXCEPT;
            virtual bool IsListening() const NOEXCEPT override;
        };

        class SocketConnected : public SocketState
        {
        public:
            virtual ~SocketConnected() = default;

            virtual bool IsConnected() const NOEXCEPT override;
        };

        class SocketDisconnected : public SocketState
        {
        public:
            virtual ~SocketDisconnected() = default;
        };

        class SocketClosed : public SocketState
        {
        public:
            virtual ~SocketClosed() = default;
        };
    }
}
