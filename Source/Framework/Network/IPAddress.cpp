#include <Lupus/Network/IPAddress.h>
#include <Lupus/Network/Utility.h>

namespace Lupus {
	IPAddress::IPAddress(U32 ipv4) :
        mFamily(AddressFamily::InterNetwork)
	{
        ipv4 = HostToNetworkOrder(ipv4);
        mAddress.insert(std::end(mAddress), (Byte*)&ipv4, (Byte*)&ipv4 + 4);
	}

    IPAddress::IPAddress(const Vector<Byte>& ipv6) :
        IPAddress(ipv6, 0)
	{
	}
	
    IPAddress::IPAddress(const Vector<Byte>& ipv6, U32 scopeid) :
        mFamily(AddressFamily::InterNetworkV6)
    {
        if (ipv6.size() != 16) {
            throw std::length_error("Vector must have exactly 16 bytes");
        }

        mAddress = ipv6;
        mScopeId = scopeid;
	}

    IPAddress::IPAddress(std::initializer_list<Byte> ilist) :
        IPAddress(Vector<Byte>(ilist))
    {
    }

	Vector<Byte> IPAddress::Bytes() const 
	{
        return mAddress;
	}
	
	AddressFamily IPAddress::Family() const 
	{
        return mFamily;
	}
	
	bool IPAddress::IsIPv6LinkLocal() const 
	{
		return false;
	}
	
	bool IPAddress::IsIPv6Multicast() const 
	{
		return false;
	}
	
	bool IPAddress::IsIPv6SiteLocal() const 
	{
		return false;
	}
	
	U32 IPAddress::ScopeId() const 
	{
        if (mFamily != AddressFamily::InterNetworkV6) {
            throw socket_error("Address is not of type IPv6");
        }

        return mScopeId;
	}
	
	void IPAddress::ScopeId(U32 value) 
    {
        if (mFamily != AddressFamily::InterNetworkV6) {
            throw socket_error("Address is not of type IPv6");
        }

        mScopeId = value;
	}

    String IPAddress::ToString() const
    {
        in_addr addr;
        in6_addr addr6;
        char str[INET6_ADDRSTRLEN];

        memset(&addr, 0, sizeof(in_addr));
        memset(&addr6, 0, sizeof(in6_addr));
        memset(str, 0, INET6_ADDRSTRLEN);

        switch (Family()) {
            case AddressFamily::InterNetwork:
                memcpy(&addr, Bytes().data(), 4);
                inet_ntop(AF_INET, &addr, str, INET6_ADDRSTRLEN);
                break;

            case AddressFamily::InterNetworkV6:
                memcpy(&addr6, Bytes().data(), 16);
                inet_ntop(AF_INET6, &addr6, str, INET6_ADDRSTRLEN);
                break;
        }

        return str;
    }

	bool IPAddress::IsLoopback(IPAddressPtr address) 
	{
        Vector<Byte> addressBytes = address->Bytes();
        Vector<Byte> comparer;

        switch (address->Family()) {
            case AddressFamily::InterNetwork:
                comparer = IPAddress::Loopback->Bytes();

                for (size_t i = 0; i < comparer.size(); i++) {
                    if (addressBytes[i] != comparer[i]) {
                        return false;
                    }
                }

                break;

            case AddressFamily::InterNetworkV6:
                comparer = IPAddress::IPv6Loopback->Bytes();

                for (size_t i = 0; i < comparer.size(); i++) {
                    if (addressBytes[i] != comparer[i]) {
                        return false;
                    }
                }

                break;
        }

        return true;
	}
	
	IPAddressPtr IPAddress::Parse(const String& ipString) 
    {
        IPAddress* address = new IPAddress(0);
        AddrIn addr;
        AddrIn6 addr6;

        memset(&addr, 0, sizeof(AddrIn));
        memset(&addr6, 0, sizeof(AddrIn6));
        address->mAddress.clear();

        if (inet_pton(AF_INET, ipString.c_str(), &(addr.sin_addr)) == 1) {
            address = new IPAddress(NetworkToHostOrder(*((U32*)&addr.sin_addr)));
        } else if (inet_pton(AF_INET6, ipString.c_str(), &(addr6.sin6_addr)) == 1) {
            Byte* begin = (Byte*)&addr6.sin6_addr;
            address = new IPAddress(0);
            address->mAddress.clear();
            address->mAddress.reserve(16);

            for (S32 i = 0; i < 16; i++) { // Konvertiere von Netzwerk zu Host.
                address->mAddress.push_back(*(begin++));
            }
        } else {
            throw std::invalid_argument("Not a valid IP address presentation");
        }

        return Pointer<IPAddress>(address);
	}
	
	bool IPAddress::TryParse(const String& ipString, IPAddressPtr& address) 
	{
		try {
			address = IPAddress::Parse(ipString);
		} catch (std::invalid_argument&) {
			return false;
		}

        return true;
	}

    const Pointer<IPAddress> IPAddress::Any(new IPAddress(0));
    const Pointer<IPAddress> IPAddress::Broadcast(new IPAddress(0xFFFFFFFF));
    const Pointer<IPAddress> IPAddress::IPv6Any(new IPAddress(Vector<Byte>({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 })));
    const Pointer<IPAddress> IPAddress::IPv6Loopback(new IPAddress(Vector<Byte>({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x01 })));
    const Pointer<IPAddress> IPAddress::IPv6None(new IPAddress(Vector<Byte>({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 })));
    const Pointer<IPAddress> IPAddress::Loopback(new IPAddress(0x7F000001));
    const Pointer<IPAddress> IPAddress::None(new IPAddress(0));
}
