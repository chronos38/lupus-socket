#include <Lupus/Network/Socket.h>
#include <Internal/Network/SocketState.h>
#include <Lupus/Network/IPAddress.h>
#include <Lupus/Network/IPEndPoint.h>
#include <Lupus/Network/SocketInformation.h>

namespace Lupus {
	namespace Internal {
		Pointer<Socket> SocketState::Accept(Socket* socket)
		{
			throw socket_error("Socket is not in an valid state for Accept");
		}
		
		void SocketState::Bind(Socket* socket, Pointer<IPEndPoint> localEndPoint)
		{
			throw socket_error("Socket is not in an valid state for Bind");
		}

		void SocketState::Close(Socket* socket)
        {
            if (closesocket(socket->Handle()) != 0) {
                throw socket_error(GetLastSocketErrorString);
            }

            socket->mHandle = INVALID_SOCKET;
            socket->mSendTime = 0;
            socket->mRecvTime = 0;
            socket->mBlocking = true;
            socket->mLocal = IPEndPointPtr(nullptr);
            socket->mRemote = IPEndPointPtr(nullptr);
            socket->mBound = false;
            socket->mConnected = false;
            ChangeState(socket, Pointer<SocketState>(new SocketClosed()));
		}

		void SocketState::Close(Socket* socket, U32 timeout)
        {
            if (socket->Handle() == INVALID_SOCKET) {
                throw socket_error("Cannot close invalid socket handle");
            }

            Thread t([this, &socket, timeout]() {
                std::this_thread::sleep_for(std::chrono::seconds(timeout));
                this->Close(socket);
            });

            t.detach();
		}
		
		void SocketState::Connect(Socket* socket, Pointer<IPEndPoint> remoteEndPoint)
		{
			throw socket_error("Socket is not in an valid state for Connect");
		}

        SocketInformation SocketState::DuplicateAndClose(Socket* socket)
        {
            IPEndPointPtr point;
            S32 family = (S32)socket->Family();
            S32 type = (S32)socket->Type();
            S32 protocol = (S32)socket->Protocol();
            SocketInformation info = {
                SocketInformationOption::None,
                Vector<Byte>()
            };

            if (!socket->mLocal && !socket->mRemote) {
                throw null_pointer("Socket endpoint points to NULL");
            } else if (!socket->mLocal) {
                point = socket->mRemote;
            } else {
                point = socket->mLocal;
            }

            info.ProtocolInformation.insert(std::end(info.ProtocolInformation), (Byte*)&family, (Byte*)&family + 4);
            info.ProtocolInformation.insert(std::end(info.ProtocolInformation), (Byte*)&type, (Byte*)&type + 4);
            info.ProtocolInformation.insert(std::end(info.ProtocolInformation), (Byte*)&protocol, (Byte*)&protocol + 4);

            if (socket->IsBound()) {
                Vector<Byte> bytes = point->Address()->Bytes();
                info.Options = SocketInformationOption::Bound;
                info.ProtocolInformation.insert(std::end(info.ProtocolInformation), std::begin(bytes), std::end(bytes));
            } else if (socket->IsConnected()) {
                Vector<Byte> bytes = socket->mRemote->Address()->Bytes();
                info.Options = SocketInformationOption::Connected;
                info.ProtocolInformation.insert(std::end(info.ProtocolInformation), std::begin(bytes), std::end(bytes));
            } else {
                info.ProtocolInformation.resize(info.ProtocolInformation.size() + sizeof(AddrStorage));
            }

            Close(socket);
            return info;
        }
		
		void SocketState::Listen(Socket* socket, U32 backlog)
		{
			throw socket_error("Socket is not bound to an end point");
		}
		
		S32 SocketState::Receive(Socket* socket, Vector<Byte>& buffer, U32 offset, U32 size, SocketFlags socketFlags, SocketError& errorCode)
		{
			throw socket_error("Socket is not in an valid state for Receive");
		}
		
		S32 SocketState::ReceiveFrom(Socket* socket, Vector<Byte>& buffer, U32 offset, U32 size, SocketFlags socketFlags, Pointer<IPEndPoint>& remoteEndPoint)
		{
			throw socket_error("Socket is not in an valid state for ReceiveFrom");
		}
		
		S32 SocketState::Send(Socket* socket, const Vector<Byte>& buaffer, U32 offset, U32 size, SocketFlags socketFlags, SocketError& errorCode)
		{
			throw socket_error("Socket is not in an valid state for Send");
		}
		
		S32 SocketState::SendTo(Socket* socket, const Vector<Byte>& buffer, U32 offset, U32 size, SocketFlags socketFlags, Pointer<IPEndPoint> remoteEndPoint)
		{
			throw socket_error("Socket is not in an valid state for SendTo");
		}
		
		void SocketState::Shutdown(Socket* socket, SocketShutdown how)
		{
			throw socket_error("Socket is not in an valid state for Shutdown");
		}

        Pointer<Socket> SocketState::CreateSocket(SocketHandle h, AddrStorage s)
        {
            Socket* sock = new Socket();
            sock->mHandle = h;
            sock->mConnected = true;
            sock->mRemote = IPEndPointPtr(new IPEndPoint(Vector<Byte>((Byte*)&s, (Byte*)&s + sizeof(AddrStorage))));
            sock->mState = Pointer<SocketState>(new SocketConnected(sock));
            return SocketPtr(sock);
        }

		void SocketState::ChangeState(Socket* socket, Pointer<SocketState> state)
		{
            socket->mState = state;
        }

        void SocketState::SetLocalEndPoint(Socket* socket, Pointer<IPEndPoint> remote)
        {
            socket->mLocal = remote;
        }

        void SocketState::SetRemoteEndPoint(Socket* socket, Pointer<IPEndPoint> remote)
        {
            socket->mRemote = remote;
        }

        Pointer<IPEndPoint> SocketState::GetLocalEndPoint(Socket* socket) const
        {
            return socket->mLocal;
        }

        Pointer<IPEndPoint> SocketState::GetRemoteEndPoint(Socket* socket) const
        {
            return socket->mRemote;
        }

        void SocketState::SetConnected(Socket* socket, bool value)
        {
            socket->mConnected = value;
        }

        void SocketState::SetBound(Socket* socket, bool value)
        {
            socket->mBound = value;
        }

        SocketBound::SocketBound(Socket* socket)
        {
            SetBound(socket, true);
        }

        SocketBound::SocketBound(Socket* socket, Pointer<IPEndPoint> endPoint)
        {
            SetBound(socket, true);
            SetLocalEndPoint(socket, endPoint);
        }

        void SocketBound::Connect(Socket* socket, Pointer<IPEndPoint> remoteEndPoint)
        {
            if (!remoteEndPoint) {
                throw null_pointer("remoteEndPoint points to NULL");
            }

            AddrStorage storage;

            memset(&storage, 0, sizeof(AddrStorage));
            memcpy(&storage, remoteEndPoint->Serialize().data(), sizeof(AddrStorage));

            if (connect(socket->Handle(), (Addr*)&storage, sizeof(AddrStorage)) != 0) {
                throw socket_error(GetLastSocketErrorString);
            }

            ChangeState(socket, Pointer<SocketState>(new SocketConnected(socket)));
        }

        void SocketBound::Listen(Socket* socket, U32 backlog)
        {
            if (listen(socket->Handle(), backlog) != 0) {
                throw socket_error(GetLastSocketErrorString);
            }

            ChangeState(socket, Pointer<SocketState>(new SocketListen(socket)));
        }

        SocketListen::SocketListen(Socket* s)
        {
            SetConnected(s, false);
            SetBound(s, true);
        }

        Pointer<Socket> SocketListen::Accept(Socket* socket)
        {
            SocketHandle handle;
            AddrStorage storage;
            AddrLength length;

            memset(&storage, 0, sizeof(AddrStorage));

            if ((handle = accept(socket->Handle(), (Addr*)&storage, &length)) != INVALID_SOCKET) {
                throw socket_error(GetLastSocketErrorString);
            }

            return CreateSocket(handle, storage);
        }

        SocketConnected::SocketConnected(Socket* s)
        {
            SetConnected(s, true);
        }

        SocketConnected::SocketConnected(Socket* s, Pointer<IPEndPoint> p)
        {
            SetConnected(s, true);
            SetRemoteEndPoint(s, p);
        }

        void SocketConnected::Connect(Socket* socket, Pointer<IPEndPoint> remoteEndPoint)
        {
            if (!remoteEndPoint) {
                throw null_pointer("remoteEndPoint points to NULL");
            }

            AddrStorage storage;

            memset(&storage, 0, sizeof(AddrStorage));
            memcpy(&storage, remoteEndPoint->Serialize().data(), sizeof(AddrStorage));

            if (connect(socket->Handle(), (Addr*)&storage, sizeof(AddrStorage)) != 0) {
                throw socket_error(GetLastSocketErrorString);
            }
        }
        
        S32 SocketConnected::Receive(Socket* socket, Vector<Byte>& buffer, U32 offset, U32 size, SocketFlags socketFlags, SocketError& errorCode)
        {
            if (offset > buffer.size() || size > buffer.size() - offset) {
                throw std::out_of_range("offset and size does not match buffer size");
            }

            return recv(socket->Handle(), (char*)buffer[offset], size, (int)socketFlags);
        }
        
        S32 SocketConnected::ReceiveFrom(Socket* socket, Vector<Byte>& buffer, U32 offset, U32 size, SocketFlags socketFlags, Pointer<IPEndPoint>& remoteEndPoint)
        {
            if (offset > buffer.size() || size > buffer.size() - offset) {
                throw std::out_of_range("offset and size does not match buffer size");
            }

            S32 result = 0;
            AddrStorage storage;
            AddrLength length = sizeof(AddrStorage);

            memset(&storage, 0, sizeof(AddrStorage));
            result = recvfrom(socket->Handle(), (char*)&buffer[offset], size, (int)socketFlags, (Addr*)&storage, &length);
            remoteEndPoint = IPEndPointPtr(new IPEndPoint(Vector<Byte>((Byte*)&storage, (Byte*)&storage + sizeof(AddrLength))));
            return result;
        }

        S32 SocketConnected::Send(Socket* socket, const Vector<Byte>& buffer, U32 offset, U32 size, SocketFlags socketFlags, SocketError& errorCode)
        {
            if (offset > buffer.size() || size > buffer.size() - offset) {
                throw std::out_of_range("offset and size does not match buffer size");
            }

            return send(socket->Handle(), (const char*)&buffer[offset], size, (int)socketFlags);
        }
        
        S32 SocketConnected::SendTo(Socket* socket, const Vector<Byte>& buffer, U32 offset, U32 size, SocketFlags socketFlags, Pointer<IPEndPoint> remoteEndPoint)
        {
            if (offset > buffer.size() || size > buffer.size() - offset) {
                throw std::out_of_range("offset and size does not match buffer size");
            }

            Vector<Byte> address = remoteEndPoint->Serialize();

            return sendto(socket->Handle(), (const char*)buffer[offset], size, (int)socketFlags, (const Addr*)address.data(), buffer.size());
        }
        
        void SocketConnected::Shutdown(Socket* socket, SocketShutdown how)
        {
            if (shutdown(socket->Handle(), (int)how) != 0) {
                throw socket_error(GetLastSocketErrorString);
            }
        }

        SocketReady::SocketReady(Socket* s)
        {
            SetBound(s, false);
            SetConnected(s, false);
        }

        void SocketReady::Bind(Socket* socket, Pointer<IPEndPoint> localEndPoint) throw(socket_error)
        {
            int yes = 1;
            Vector<Byte> address = localEndPoint->Serialize();

            if (setsockopt(socket->Handle(), SOL_SOCKET, SO_REUSEADDR, (const char*)yes, sizeof(int)) != 0) {
                throw socket_error(GetLastSocketErrorString);
            }

            ChangeState(socket, Pointer<SocketState>(new SocketBound(socket, localEndPoint)));
        }

        void SocketReady::Connect(Socket* socket, Pointer<IPEndPoint> remoteEndPoint) throw(socket_error, null_pointer)
        {
            ChangeState(socket, Pointer<SocketState>(new SocketConnected(socket, remoteEndPoint)));
        }

        void SocketClosed::Close(Socket* socket)
        {
            throw socket_error("Socket is not in an valid state for Close");
        }

        void SocketClosed::Close(Socket* socket, U32 timeout)
        {
            throw socket_error("Socket is not in an valid state for Close");
        }
        
        SocketInformation SocketClosed::DuplicateAndClose(Socket* socket)
        {
            throw socket_error("Socket is not in an valid state for DuplicateAndClose");
        }
    }
}
