#include <Lupus/Network/Utility.h>
#include <Lupus/Network/IPAddress.h>
#include <Lupus/Network/IPEndPoint.h>
#include <Lupus/Network/Socket.h>

namespace Lupus {
	U16 HostToNetworkOrder(U16 host)
	{
		return htons(host);
	}

	U32 HostToNetworkOrder(U32 host)
	{
		return htonl(host);
	}

	U64 HostToNetworkOrder(U64 host)
	{
		return htonll(host);
	}

	U16 NetworkToHostOrder(U16 network)
	{
		return ntohs(network);
	}

	U32 NetworkToHostOrder(U32 network)
	{
		return ntohl(network);
	}

	U64 NetworkToHostOrder(U64 network)
	{
		return ntohll(network);
	}

    Vector<Pointer<IPEndPoint>> GetAddressInformation(const String& node, const String& service)
	{
		return GetAddressInformation(node, service, AddressFamily::Unspecified, SocketType::Unspecified, ProtocolType::Unspecified);
	}

    Vector<Pointer<IPEndPoint>> GetAddressInformation(const String& node, const String& service, AddressFamily family)
	{
		return GetAddressInformation(node, service, family, SocketType::Unspecified, ProtocolType::Unspecified);
	}

    Vector<Pointer<IPEndPoint>> GetAddressInformation(const String& node, const String& service, SocketType type)
	{
		return GetAddressInformation(node, service, AddressFamily::Unspecified, type, ProtocolType::Unspecified);
	}

    Vector<Pointer<IPEndPoint>> GetAddressInformation(const String& node, const String& service, ProtocolType protocol)
	{
		return GetAddressInformation(node, service, AddressFamily::Unspecified, SocketType::Unspecified, protocol);
	}

    Vector<Pointer<IPEndPoint>> GetAddressInformation(const String& node, const String& service, AddressFamily family, SocketType type, ProtocolType protocol)
	{
        if (service.empty()) {
            throw std::invalid_argument("service argument must have a valid value");
        }

        Vector<Pointer<IPEndPoint>> addresses;
		const char* nodename = node.empty() ? node.c_str() : nullptr;
		AddrInfo hints, *begin = nullptr, *it = nullptr;

		memset(&hints, 0, sizeof(hints));
		hints.ai_family = (int)family;
		hints.ai_socktype = (int)type;
		hints.ai_protocol = (int)protocol;

		if (!nodename) {
			hints.ai_flags = AI_PASSIVE;
		}

		if (getaddrinfo(nodename, service.c_str(), &hints, &begin) != 0) {
            if (begin) {
                freeaddrinfo(begin);
            }

			throw std::runtime_error(GetLastAddressInfoErrorString);
		}

		for (it = begin; it; it = it->ai_next) {
            AddrStorage storage;

            memset(&storage, 0, sizeof(AddrStorage));
            memcpy(&storage, it->ai_addr, it->ai_addrlen);
            addresses.push_back(IPEndPointPtr(new IPEndPoint(Vector<Byte>((Byte*)&storage, (Byte*)&storage + sizeof(AddrStorage)))));
		}

		freeaddrinfo(begin);
		return addresses;
	}
}
