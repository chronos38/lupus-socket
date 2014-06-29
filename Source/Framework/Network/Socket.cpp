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

		mFamily = (AddressFamily)family;
		mProtocol = (ProtocolType)protocol;

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

			    mState.reset(new Internal::SocketConnected());
			    break;

            case SocketInformationOption::Bound:
                if (bind(mHandle, (Addr*)&storage, addrsize) != 0) {
                    throw socket_error(GetLastSocketErrorString);
                }

			    mState.reset(new Internal::SocketBound());
			    break;

		    default:
			    mState.reset(new Internal::SocketClosed());
			    break;
		}
	}

	Socket::Socket(AddressFamily family, SocketType type, ProtocolType protocol)
	{
		if ((mHandle = socket((int)family, (int)type, (int)protocol)) == INVALID_SOCKET) {
			throw socket_error(GetLastSocketErrorString);
		}

		mFamily = family;
		mProtocol = protocol;
		mState.reset(new Internal::SocketClosed());
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

	void Socket::Disconnect(bool reuseSocket)
	{
		mState->Disconnect(this, reuseSocket);
	}

    SocketInformation Socket::DuplicateAndClose()
    {
        S32 family = (S32)Family();
        S32 type = (S32)Type();
        S32 protocol = (S32)Protocol();
        SocketInformation info = {
            SocketInformationOption::None,
            Vector<Byte>()
        };

        info.ProtocolInformation.insert(std::end(info.ProtocolInformation), (Byte*)&family, (Byte*)&family + 4);
        info.ProtocolInformation.insert(std::end(info.ProtocolInformation), (Byte*)&type, (Byte*)&type + 4);
        info.ProtocolInformation.insert(std::end(info.ProtocolInformation), (Byte*)&protocol, (Byte*)&protocol + 4);

        if (IsConnected()) {
            Vector<Byte> bytes = mLocal->Address()->AddressBytes();
            info.Options = SocketInformationOption::Connected;
            info.ProtocolInformation.insert(std::end(info.ProtocolInformation), std::begin(bytes), std::end(bytes));
        } else if (IsBound()) {
            Vector<Byte> bytes = mLocal->Address()->AddressBytes();
            info.Options = SocketInformationOption::Bound;
            info.ProtocolInformation.insert(std::end(info.ProtocolInformation), std::begin(bytes), std::end(bytes));
        }

        Close();
        return info;
    }

	void Socket::Listen(U32 backlog)
	{
		mState->Listen(this, backlog);
	}

	PollResult Socket::Poll(U32 milliSeconds, SelectMode mode)
	{
		short value = (short)mode;
		short events = 0;

		switch (mode) {
		case SelectMode::Read: events |= LU_POLLIN; break;
		case SelectMode::Write: events |= LU_POLLOUT; break;
		case SelectMode::OutOfBand: events |= LU_POLLPRI; break;
		default: break;
		}

		pollfd fd = { mHandle, events, 0 };
		pollfd fdarray[] = { fd };

		switch (poll(fdarray, 1, (int)milliSeconds)) {
		case SOCKET_ERROR: throw socket_error(GetLastSocketErrorString);
		case 0: return PollResult::Timeout;
		default: return ((fdarray[0].revents & value) == value) ? PollResult::True : PollResult::False;
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
		return mState->IsConnected();
	}

	bool Socket::IsBound() const
	{
		return mState->IsBound();
	}

	bool Socket::IsListening() const
	{
		return mState->IsListening();
	}

	AddressFamily Socket::Family() const
	{
		return mFamily;
	}
	
	ProtocolType Socket::Protocol() const
	{
		return mProtocol;
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
		if (!mLocal) {
			throw socket_error("Local end point is not set");
		}

		return mLocal;
	}
	
	Pointer<IPEndPoint> Socket::RemoteEndPoint() const
	{
		if (!mRemote) {
			throw socket_error("Remote end point is not set");
		}

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
