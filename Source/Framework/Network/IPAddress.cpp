#include <Lupus/Network/IPAddress.h>
#include <Lupus/Network/Utility.h>

namespace Lupus {
	IPAddress::IPAddress(U32 ipv4) :
        mFamily(AddressFamily::InterNetwork)
	{
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
	}

	Vector<Byte> IPAddress::AddressBytes() const 
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

	bool IPAddress::IsLoopback(IPAddressPtr address) 
	{
        Vector<Byte> addressBytes = address->AddressBytes();
        Vector<Byte> comparer;

        switch (address->Family()) {
            case AddressFamily::InterNetwork:
                comparer = IPAddress::Loopback->AddressBytes();

                for (size_t i = 0; i < comparer.size(); i++) {
                    if (addressBytes[i] != comparer[i]) {
                        return false;
                    }
                }

                break;

            case AddressFamily::InterNetworkV6:
                comparer = IPAddress::IPv6Loopback->AddressBytes();

                for (size_t i = 0; i < comparer.size(); i++) {
                    if (addressBytes[i] != comparer[i]) {
                        return false;
                    }
                }

                break;

            default:
                return false;
        }

        return true;
	}
	
	IPAddressPtr IPAddress::Parse(const String& ipString) 
	{
		return PresentationToNetwork(ipString);
	}
	
	bool IPAddress::TryParse(const String& ipString, IPAddressPtr& address) 
	{
		try {
			address = PresentationToNetwork(ipString);
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
