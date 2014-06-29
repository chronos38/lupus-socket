#include <Lupus/Network/Socket.h>
#include <Internal/Network/SocketState.h>

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

            ChangeState(socket, Pointer<SocketState>(new SocketClosed()));
		}

		void SocketState::Close(Socket* socket, U32 timeout)
        {
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
		
		void SocketState::Disconnect(Socket* socket, bool reuseSocket)
		{
			throw socket_error("Socket has no connection");
		}
		
		void SocketState::Listen(Socket* socket, U32 backlog)
		{
			throw socket_error("Socket is not bound to an end point");
		}
		
		bool SocketState::Poll(Socket* socket, U32 microSeconds, SelectMode mode)
		{
			throw socket_error("Socket is not in an valid state for Poll");
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

		bool SocketState::IsConnected() const
		{
			return false;
		}

		bool SocketState::IsBound() const
		{
			return false;
		}

		bool SocketState::IsListening() const
		{
			return false;
		}

		bool SocketConnected::IsConnected() const
		{
			return true;
        }

        bool SocketBound::IsBound() const
        {
            return true;
        }

        bool SocketListen::IsBound() const
        {
            return true;
        }

		bool SocketListen::IsListening() const
		{
			return true;
		}

		void SocketState::ChangeState(Socket* socket, Pointer<SocketState> state)
		{
            socket->mState = state;
		}

		void SocketState::SetRemoteEndPoint(Socket* socket, Pointer<IPEndPoint> remote)
		{
            socket->mRemote = remote;
		}
		
		void SocketState::SetLocalEndPoint(Socket* socket, Pointer<IPEndPoint> local)
		{
            socket->mLocal = local;
        }

        Pointer<IPEndPoint> SocketState::GetRemoteEndPoint(Socket* socket) const
        {
            return socket->mRemote;
        }

        Pointer<IPEndPoint> SocketState::GetLocalEndPoint(Socket* socket) const
        {
            return socket->mLocal;
        }

        void SocketBound::Connect(Socket* socket, Pointer<IPEndPoint> remoteEndPoint)
        {

        }

        void SocketBound::Listen(Socket* socket, U32 backlog)
        {

        }
    }
}
