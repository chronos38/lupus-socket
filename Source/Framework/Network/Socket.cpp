#include <Lupus/Network/Socket.h>
#include <Lupus/Network/SocketInformation.h>
#include <Lupus/Network/Utility.h>
#include <Lupus/Network/IPAddress.h>
#include <Lupus/Network/IPEndPoint.h>
#include <Internal/Network/SocketState.h>

namespace Lupus {
	Socket::Socket(const SocketInformation& socketInformation)
	{
		if (socketInformation.ProtocolInformation.size() != sizeof(AddrStorage) + 12) {
			throw std::invalid_argument("socketInformatoin has not the right size");
		}

        Vector<Byte> information = socketInformation.ProtocolInformation;
        S32 family = *((S32*)(information.data()));
        S32 type = *((S32*)(information.data() + 4));
        S32 protocol = *((S32*)(information.data() + 8));
		AddrStorage storage;
		int addrsize = 0;

        memset(&storage, 0, sizeof(storage));
        memcpy(&storage, socketInformation.ProtocolInformation.data() + 12, sizeof(storage));

		if ((mHandle = socket((int)family, (int)type, (int)protocol)) == INVALID_SOCKET) {
			throw socket_error(GetLastSocketErrorString);
		}

		switch (family) {
            case AF_INET:
			    addrsize = sizeof(AddrIn);
			    break;

            case AF_INET6:
			    addrsize = sizeof(AddrIn6);
			    break;

		    default:
                throw std::invalid_argument("Address family is not supported");
        }

		switch (socketInformation.Options) {
            case SocketInformationOption::Connected:
                if (connect(mHandle, (Addr*)&storage, addrsize) != 0) {
                    throw socket_error(GetLastSocketErrorString);
                }

			    mState.reset(new Internal::SocketConnected(this));
			    break;

            case SocketInformationOption::Bound:
                if (bind(mHandle, (Addr*)&storage, addrsize) != 0) {
                    throw socket_error(GetLastSocketErrorString);
                }

			    mState.reset(new Internal::SocketBound(this));
			    break;

		    default:
                mState.reset(new Internal::SocketReady(this));
			    break;
		}
	}

	Socket::Socket(AddressFamily family, SocketType type, ProtocolType protocol)
	{
		if ((mHandle = socket((int)family, (int)type, (int)protocol)) == INVALID_SOCKET) {
			throw socket_error(GetLastSocketErrorString);
		}

        mState.reset(new Internal::SocketReady(this));
	}

	Socket::~Socket()
	{
		if (mHandle != INVALID_SOCKET) {
			closesocket(mHandle);
		}
	}

	Pointer<Socket> Socket::Accept()
	{
		return mState->Accept(this);
	}

	void Socket::Bind(Pointer<IPEndPoint> localEndPoint)
	{
		mState->Bind(this, localEndPoint);
	}

	void Socket::Close()
	{
		mState->Close(this);
	}

	void Socket::Close(U32 timeout)
	{
		mState->Close(this, timeout);
	}

	void Socket::Connect(Pointer<IPEndPoint> remoteEndPoint)
	{
		mState->Connect(this, remoteEndPoint);
	}

	void Socket::Connect(Pointer<IPAddress> address, U16 port)
	{
		mState->Connect(this, IPEndPointPtr(new IPEndPoint(address, port)));
	}

    void Socket::Connect(const Vector<Pointer<IPEndPoint>>& endPoints)
	{
        for (const IPEndPointPtr& endPoint : endPoints) {
            try {
                mState->Connect(this, endPoint);
                break;
            } catch (socket_error&) {
                continue;
            }
        }
	}

	void Socket::Connect(const String& host, U16 port)
	{
        mState->Connect(this, IPEndPointPtr(new IPEndPoint(IPAddress::Parse(host), port)));
	}

    SocketInformation Socket::DuplicateAndClose()
    {
        return mState->DuplicateAndClose(this);
    }

	void Socket::Listen(U32 backlog)
	{
		mState->Listen(this, backlog);
	}

    SocketPollFlags Socket::Poll(U32 milliSeconds, SocketPollFlags mode)
	{
        pollfd fd = { mHandle, (short)mode, 0 };
        pollfd fdarray[] = { fd };

        switch (poll(fdarray, 1, (int)milliSeconds)) {
            case SOCKET_ERROR: throw socket_error(GetLastSocketErrorString);
            case 0: return SocketPollFlags::Timeout;
            default: return (SocketPollFlags)fdarray[0].revents;
        }
	}

	S32 Socket::Receive(Vector<Byte>& buffer)
	{
		SocketError errorCode;
		return mState->Receive(this, buffer, 0, buffer.size(), SocketFlags::None, errorCode);
	}

	S32 Socket::Receive(Vector<Byte>& buffer, U32 offset)
	{
		SocketError errorCode;
		return mState->Receive(this, buffer, offset, buffer.size() - offset, SocketFlags::None, errorCode);
	}

	S32 Socket::Receive(Vector<Byte>& buffer, U32 offset, U32 size)
	{
		SocketError errorCode;
		return mState->Receive(this, buffer, offset, size, SocketFlags::None, errorCode);
	}

	S32 Socket::Receive(Vector<Byte>& buffer, U32 offset, U32 size, SocketFlags socketFlags)
	{
		SocketError errorCode;
		return mState->Receive(this, buffer, offset, size, socketFlags, errorCode);
	}

	S32 Socket::Receive(Vector<Byte>& buffer, U32 offset, U32 size, SocketFlags socketFlags, SocketError& errorCode)
	{
		return mState->Receive(this, buffer, offset, size, socketFlags, errorCode);
	}

	S32 Socket::ReceiveFrom(Vector<Byte>& buffer, Pointer<IPEndPoint>& remoteEndPoint)
	{
		return mState->ReceiveFrom(this, buffer, 0, buffer.size(), SocketFlags::None, remoteEndPoint);
	}

	S32 Socket::ReceiveFrom(Vector<Byte>& buffer, U32 offset, Pointer<IPEndPoint>& remoteEndPoint)
	{
		return mState->ReceiveFrom(this, buffer, offset, buffer.size() - offset, SocketFlags::None, remoteEndPoint);
	}

	S32 Socket::ReceiveFrom(Vector<Byte>& buffer, U32 offset, U32 size, Pointer<IPEndPoint>& remoteEndPoint)
	{
		return mState->ReceiveFrom(this, buffer, offset, size, SocketFlags::None, remoteEndPoint);
	}

	S32 Socket::ReceiveFrom(Vector<Byte>& buffer, U32 offset, U32 size, SocketFlags socketFlags, Pointer<IPEndPoint>& remoteEndPoint)
	{
		return mState->ReceiveFrom(this, buffer, offset, size, socketFlags, remoteEndPoint);
	}

	S32 Socket::Send(const Vector<Byte>& buffer)
	{
		SocketError errorCode;
		return mState->Send(this, buffer, 0, buffer.size(), SocketFlags::None, errorCode);
	}

	S32 Socket::Send(const Vector<Byte>& buffer, U32 offset)
	{
		SocketError errorCode;
		return mState->Send(this, buffer, offset, buffer.size() - offset, SocketFlags::None, errorCode);
	}

	S32 Socket::Send(const Vector<Byte>& buffer, U32 offset, U32 size)
	{
		SocketError errorCode;
		return mState->Send(this, buffer, offset, size, SocketFlags::None, errorCode);
	}

	S32 Socket::Send(const Vector<Byte>& buffer, U32 offset, U32 size, SocketFlags socketFlags)
	{
		SocketError errorCode;
		return mState->Send(this, buffer, offset, size, socketFlags, errorCode);
	}

	S32 Socket::Send(const Vector<Byte>& buffer, U32 offset, U32 size, SocketFlags socketFlags, SocketError& errorCode)
	{
		return mState->Send(this, buffer, offset, size, socketFlags, errorCode);
	}

	S32 Socket::SendTo(const Vector<Byte>& buffer, Pointer<IPEndPoint> remoteEndPoint)
	{
		return mState->SendTo(this, buffer, 0, buffer.size(), SocketFlags::None, remoteEndPoint);
	}

	S32 Socket::SendTo(const Vector<Byte>& buffer, U32 offset, Pointer<IPEndPoint> remoteEndPoint)
	{
		return mState->SendTo(this, buffer, offset, buffer.size() - offset, SocketFlags::None, remoteEndPoint);
	}

	S32 Socket::SendTo(const Vector<Byte>& buffer, U32 offset, U32 size, Pointer<IPEndPoint> remoteEndPoint)
	{
		return mState->SendTo(this, buffer, offset, size, SocketFlags::None, remoteEndPoint);
	}

	S32 Socket::SendTo(const Vector<Byte>& buffer, U32 offset, U32 size, SocketFlags socketFlags, Pointer<IPEndPoint> remoteEndPoint)
	{
		return mState->SendTo(this, buffer, offset, size, socketFlags, remoteEndPoint);
	}

	void Socket::Shutdown(SocketShutdown how)
	{
		mState->Shutdown(this, how);
	}

	SocketHandle Socket::Handle() const
	{
		return mHandle;
	}

	bool Socket::IsConnected() const
	{
        return mConnected;
	}

	bool Socket::IsBound() const
	{
        return mBound;
	}

	bool Socket::IsListening() const
    {
        S32 result, length = 4;

        if (getsockopt(mHandle, SOL_SOCKET, SO_ACCEPTCONN, (char*)&result, (int*)&length) != 0) {
            throw socket_error(GetLastSocketErrorString);
        }
        
        return (result == 1);
	}

	AddressFamily Socket::Family() const
	{
        return (AddressFamily)Internal::GetSocketDomain(mHandle);
	}
	
	ProtocolType Socket::Protocol() const
	{
        return (ProtocolType)Internal::GetSocketProtocol(mHandle);
	}
	
	SocketType Socket::Type() const
	{
		S32 result, length = 4;

		if (getsockopt(mHandle, SOL_SOCKET, SO_TYPE, (char*)&result, (int*)&length) != 0) {
			throw socket_error(GetLastSocketErrorString);
		}

		return (SocketType)result;
	}

	U32 Socket::Available() const
	{
		u_long arg = 0;
		
		if (ioctlsocket(mHandle, FIONREAD, &(arg)) != 0) {
			throw socket_error(GetLastSocketErrorString);
		}

		return (U32)arg;
	}

	bool Socket::Blocking() const
	{
		return mBlocking;
	}

	void Socket::Blocking(bool value)
	{
		u_long arg = value ? 1 : 0;
		mBlocking = value;

		if (ioctlsocket(mHandle, FIONBIO, &(arg)) != 0) {
			throw socket_error(GetLastSocketErrorString);
		}
    }

    Pointer<IPEndPoint> Socket::LocalEndPoint() const
    {
        return mLocal;
    }

    Pointer<IPEndPoint> Socket::RemoteEndPoint() const
    {
        return mRemote;
    }
	
	S32 Socket::SendBuffer() const
	{
		S32 result, length = 4;
		
		if (getsockopt(mHandle, SOL_SOCKET, SO_SNDBUF, (char*)&result, (int*)&length) != 0) {
			throw socket_error(GetLastSocketErrorString);
		}

		return result;
	}
	
	void Socket::SendBuffer(S32 value)
	{
		if (setsockopt(mHandle, SOL_SOCKET, SO_SNDBUF, (char*)&value, 4) != 0) {
			throw socket_error(GetLastSocketErrorString);
		}
	}
	
	S32 Socket::ReceiveBuffer() const
	{
		S32 result, length = 4;

		if (getsockopt(mHandle, SOL_SOCKET, SO_RCVBUF, (char*)&result, (int*)&length) != 0) {
			throw socket_error(GetLastSocketErrorString);
		}

		return result;
	}
	
	void Socket::ReceiveBuffer(S32 value)
	{
		if (setsockopt(mHandle, SOL_SOCKET, SO_RCVBUF, (char*)&value, 4) != 0) {
			throw socket_error(GetLastSocketErrorString);
		}
	}
	
	S32 Socket::SendTimeout() const
	{
		return mSendTime;
	}
	
	void Socket::SendTimeout(S32 value)
	{
		if (setsockopt(mHandle, SOL_SOCKET, SO_SNDTIMEO, (char*)&value, 4) != 0) {
			throw socket_error(GetLastSocketErrorString);
		}

		mSendTime = value;
	}
	
	S32 Socket::ReceiveTimeout() const
	{
		return mRecvTime;
	}
	
	void Socket::ReceiveTimeout(S32 value)
	{
		if (setsockopt(mHandle, SOL_SOCKET, SO_RCVTIMEO, (char*)&value, 4) != 0) {
			throw socket_error(GetLastSocketErrorString);
		}

		mRecvTime = value;
	}

	void Socket::Select(const Vector<Pointer<Socket>>& checkRead, const Vector<Pointer<Socket>>& checkWrite, const Vector<Pointer<Socket>>& checkError, U32 microSeconds)
	{
		throw socket_error("Select is not implemented");
	}
}
