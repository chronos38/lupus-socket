#include <Lupus/Network/IPEndPoint.h>
#include <Lupus/Network/IPAddress.h>
#include <Lupus/Network/Utility.h>

namespace Lupus {
	IPEndPoint::IPEndPoint(U32 address, U16 port) :
        IPEndPoint(IPAddressPtr(new IPAddress(address)), port)
    {
	}
	
	IPEndPoint::IPEndPoint(IPAddressPtr address, U16 port)
    {
        if (!address) {
            throw null_pointer("Can't set an address that points to NULL");
        }

        AddrIn* addr;
        AddrIn6* addr6;

        memset(&mAddrStorage, 0, sizeof(AddrStorage));

        switch (address->Family()) {
            case AddressFamily::InterNetwork:
                addr = (AddrIn*)&mAddrStorage;
                addr->sin_family = AF_INET;
                addr->sin_port = HostToNetworkOrder(port);
                memcpy(&addr->sin_addr, address->Bytes().data(), 4);
                break;

            case AddressFamily::InterNetworkV6:
                addr6 = (AddrIn6*)&mAddrStorage;
                addr6->sin6_family = AF_INET;
                addr6->sin6_port = HostToNetworkOrder(port);
                memcpy(&addr6->sin6_addr, address->Bytes().data(), 16);
                break;
        }

        mAddress = address;
	}

    IPEndPoint::IPEndPoint(const Vector<Byte>& buffer)
    {
        if (buffer.size() != sizeof(AddrStorage)) {
            throw std::invalid_argument("buffer contains invalid data");
        }

        memset(&mAddrStorage, 0, sizeof(AddrStorage));
        memcpy(&mAddrStorage, buffer.data(), sizeof(AddrStorage));
    }

	AddressFamily IPEndPoint::Family() const
	{
		return mAddress->Family();
	}
	
	IPAddressPtr IPEndPoint::Address() const
	{
		return mAddress;
	}
	
	void IPEndPoint::Address(IPAddressPtr address)
	{
        if (!address) {
            throw null_pointer("Cannot set an address that points to NULL");
        }

        U16 port = Port();
        AddrIn* addr;
        AddrIn6* addr6;

        memset(&mAddrStorage, 0, sizeof(AddrStorage));

        switch (address->Family()) {
            case AddressFamily::InterNetwork:
                addr = (AddrIn*)&mAddrStorage;
                addr->sin_family = AF_INET;
                addr->sin_port = HostToNetworkOrder(port);
                memcpy(&addr->sin_addr, address->Bytes().data(), 4);
                break;

            case AddressFamily::InterNetworkV6:
                addr6 = (AddrIn6*)&mAddrStorage;
                addr6->sin6_family = AF_INET;
                addr6->sin6_port = HostToNetworkOrder(port);
                memcpy(&addr6->sin6_addr, address->Bytes().data(), 16);
                break;
        }

        mAddress = address;
	}
	
	U16 IPEndPoint::Port() const
	{
        switch (mAddrStorage.ss_family) {
            case AF_INET:
                return NetworkToHostOrder(((AddrIn*)&mAddrStorage)->sin_port);
            
            case AF_INET6:
                return NetworkToHostOrder(((AddrIn6*)&mAddrStorage)->sin6_port);
        }

        return 0;
	}
	
	void IPEndPoint::Port(U16 port)
    {
        switch (mAddrStorage.ss_family) {
            case AF_INET:
                ((AddrIn*)&mAddrStorage)->sin_port = HostToNetworkOrder(port);

            case AF_INET6:
                ((AddrIn6*)&mAddrStorage)->sin6_port = HostToNetworkOrder(port);
        }
	}

    Vector<Byte> IPEndPoint::Serialize() const
    {
        return Vector<Byte>((Byte*)&mAddrStorage, (Byte*)&mAddrStorage + sizeof(AddrStorage));
    }
}
