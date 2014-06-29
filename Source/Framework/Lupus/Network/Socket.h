#pragma once

#include <Lupus/Network/Enum.h>

namespace Lupus {
    struct SocketInformation;
    class IPEndPoint;
    class IPAddress;

    namespace Internal {
        class SocketState;
    }

    class LUPUS_API Socket : public ReferenceType
    {
    public:
        ///////////////////////////////////////////////////////////////////////
        /// Constructors
        ///////////////////////////////////////////////////////////////////////
        Socket() = delete;
        Socket(const SocketInformation& socketInformation) throw(std::invalid_argument, socket_error);
        Socket(AddressFamily family, SocketType type, ProtocolType protocol) throw(socket_error);
        virtual ~Socket();

        ///////////////////////////////////////////////////////////////////////
        /// Methods
        ///////////////////////////////////////////////////////////////////////
        virtual Pointer<Socket> Accept() throw (socket_error);
        virtual void Bind(Pointer<IPEndPoint> localEndPoint) throw(socket_error);
        virtual void Close() throw(socket_error);
        virtual void Close(U32 timeout) throw(socket_error);
        virtual void Connect(Pointer<IPEndPoint> remoteEndPoint) throw(socket_error, null_pointer);
        virtual void Connect(Pointer<IPAddress> address, U16 port) throw(socket_error, null_pointer);
        virtual void Connect(const Vector<Pointer<IPEndPoint>>& endPoints) throw(null_pointer);
        virtual void Connect(const String& host, U16 port) throw(socket_error, std::invalid_argument);
        virtual void Disconnect(bool reuseSocket) throw(socket_error);
        virtual SocketInformation DuplicateAndClose() throw(socket_error);
        virtual void Listen(U32 backlog) throw(socket_error);
        virtual PollResult Poll(U32 milliSeconds, SelectMode mode) throw(socket_error);
        virtual S32 Receive(Vector<Byte>& buffer) throw(socket_error);
        virtual S32 Receive(Vector<Byte>& buffer, U32 offset) throw(socket_error, std::out_of_range);
        virtual S32 Receive(Vector<Byte>& buffer, U32 offset, U32 size) throw(socket_error, std::out_of_range);
        virtual S32 Receive(Vector<Byte>& buffer, U32 offset, U32 size, SocketFlags socketFlags) throw(socket_error, std::out_of_range);
        virtual S32 Receive(Vector<Byte>& buffer, U32 offset, U32 size, SocketFlags socketFlags, SocketError& errorCode) throw(socket_error, std::out_of_range);
        virtual S32 ReceiveFrom(Vector<Byte>& buffer, Pointer<IPEndPoint>& remoteEndPoint) throw(socket_error);
        virtual S32 ReceiveFrom(Vector<Byte>& buffer, U32 offset, Pointer<IPEndPoint>& remoteEndPoint) throw(socket_error, std::out_of_range);
        virtual S32 ReceiveFrom(Vector<Byte>& buffer, U32 offset, U32 size, Pointer<IPEndPoint>& remoteEndPoint) throw(socket_error, std::out_of_range);
        virtual S32 ReceiveFrom(Vector<Byte>& buffer, U32 offset, U32 size, SocketFlags socketFlags, Pointer<IPEndPoint>& remoteEndPoint) throw(socket_error, std::out_of_range);
        virtual S32 Send(const Vector<Byte>& buffer) throw(socket_error);
        virtual S32 Send(const Vector<Byte>& buffer, U32 offset) throw(socket_error, std::out_of_range);
        virtual S32 Send(const Vector<Byte>& buffer, U32 offset, U32 size) throw(socket_error, std::out_of_range);
        virtual S32 Send(const Vector<Byte>& buffer, U32 offset, U32 size, SocketFlags socketFlags) throw(socket_error, std::out_of_range);
        virtual S32 Send(const Vector<Byte>& buffer, U32 offset, U32 size, SocketFlags socketFlags, SocketError& errorCode) throw(socket_error, std::out_of_range);
        virtual S32 SendTo(const Vector<Byte>& buffer, Pointer<IPEndPoint> remoteEndPoint) throw(socket_error);
        virtual S32 SendTo(const Vector<Byte>& buffer, U32 offset, Pointer<IPEndPoint> remoteEndPoint) throw(socket_error, std::out_of_range);
        virtual S32 SendTo(const Vector<Byte>& buffer, U32 offset, U32 size, Pointer<IPEndPoint> remoteEndPoint) throw(socket_error, std::out_of_range);
        virtual S32 SendTo(const Vector<Byte>& buffer, U32 offset, U32 size, SocketFlags socketFlags, Pointer<IPEndPoint> remoteEndPoint) throw(socket_error, std::out_of_range);
        virtual void Shutdown(SocketShutdown how) throw(socket_error);

        ///////////////////////////////////////////////////////////////////////
        /// Modifiers
        ///////////////////////////////////////////////////////////////////////
        virtual SocketHandle Handle() const NOEXCEPT;
        virtual bool IsConnected() const NOEXCEPT;
        virtual bool IsBound() const NOEXCEPT;
        virtual bool IsListening() const throw(socket_error);
        virtual AddressFamily Family() const throw(socket_error);
        virtual ProtocolType Protocol() const throw(socket_error);
        virtual SocketType Type() const throw(socket_error);
        virtual U32 Available() const throw(socket_error);
        virtual bool Blocking() const NOEXCEPT;
        virtual void Blocking(bool) throw(socket_error);
        virtual Pointer<IPEndPoint> LocalEndPoint() const throw(socket_error);
        virtual Pointer<IPEndPoint> RemoteEndPoint() const throw(socket_error);
        virtual S32 SendBuffer() const throw(socket_error);
        virtual void SendBuffer(S32) throw(socket_error);
        virtual S32 ReceiveBuffer() const throw(socket_error);
        virtual void ReceiveBuffer(S32) throw(socket_error);
        virtual S32 SendTimeout() const NOEXCEPT;
        virtual void SendTimeout(S32) throw(socket_error);
        virtual S32 ReceiveTimeout() const NOEXCEPT;
        virtual void ReceiveTimeout(S32) throw(socket_error);

        ///////////////////////////////////////////////////////////////////////
        /// Static Methods
        ///////////////////////////////////////////////////////////////////////
        static void Select(const Vector<Pointer<Socket>>& checkRead, const Vector<Pointer<Socket>>& checkWrite, const Vector<Pointer<Socket>>& checkError, U32 microSeconds) throw(socket_error);

    private:

        Socket(SocketHandle, AddrStorage);

        SocketHandle mHandle = INVALID_SOCKET;
        S32 mSendTime = 0; // Windows support
        S32 mRecvTime = 0; // Windows support
        bool mBlocking = true;
        Pointer<IPEndPoint> mLocal;
        Pointer<IPEndPoint> mRemote;
        bool mBound = false;
        bool mConnected = false;

        Pointer<Internal::SocketState> mState;

        friend Internal::SocketState;
    };

    typedef Pointer<Socket> SocketPtr;
}
