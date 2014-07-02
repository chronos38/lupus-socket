#include "stdafx.h"
#include "CppUnitTest.h"
#include <Lupus\Network\IPAddress.h>
#include <Lupus\Network\IPEndPoint.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Lupus;

namespace FrameworkTest
{
    class IPAddressMock : public IPAddress
    {
        bool mIpv6 = false;

    public:

        explicit IPAddressMock(U32 ipv4) NOEXCEPT :
            IPAddress(ipv4)
        {
        }

        IPAddressMock(const Vector<Byte>& ipv6) throw(std::length_error) :
            IPAddress(ipv6)
        {
            mIpv6 = true;
        }

        IPAddressMock(const Vector<Byte>& ipv6, U32 scopeid) throw(std::length_error) :
            IPAddress(ipv6, scopeid)
        {
            mIpv6 = true;
        }

        IPAddressMock(std::initializer_list<Byte> ilist) throw(std::length_error) :
            IPAddress(ilist)
        {
            mIpv6 = true;
        }

        virtual Vector<Byte> Bytes() const NOEXCEPT override
        {
            if (mIpv6) {
                return Vector<Byte>({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 });
            } else {
                return Vector<Byte>({ 127, 0, 0, 1 });
            }
        }

        virtual AddressFamily Family() const NOEXCEPT override
        {
            if (mIpv6) {
                return AddressFamily::InterNetworkV6;
            } else {
                return AddressFamily::InterNetwork;
            }
        }

        virtual String ToString() const override
        {
            if (mIpv6) {
                return "::1";
            } else {
                return "127.0.0.1";
            }
        }
    };

    TEST_CLASS(IPEndPointTest)
    {
    public:

        TEST_METHOD(IPEndPoint_Constructor)
        {
            IPEndPoint(0x12345678, 12345);
            IPEndPoint(IPAddressPtr(new IPAddressMock(0)), 12345);

            Assert::ExpectException<null_pointer>([](){
                IPEndPoint(nullptr, 12345);
            });
        }

        TEST_METHOD(IPEndPoint_Family)
        {
            IPEndPoint point(IPAddressPtr(new IPAddressMock(0)), 12345);
            IPEndPoint point6(IPAddressPtr(new IPAddressMock({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 })), 12345);

            Assert::IsTrue(point.Family() == AddressFamily::InterNetwork);
            Assert::IsTrue(point6.Family() == AddressFamily::InterNetworkV6);
        }

        TEST_METHOD(IPEndPoint_Address)
        {
            IPAddressPtr addr = IPAddressPtr(new IPAddressMock(0));
            IPAddressPtr addr6 = IPAddressPtr(new IPAddressMock({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 }));
            IPEndPoint point(IPAddressPtr(new IPAddressMock(0)), 12345);

            point.Address(addr);
            Assert::IsTrue(addr->Family() == AddressFamily::InterNetwork);

            point.Address(addr6);
            Assert::IsTrue(addr6->Family() == AddressFamily::InterNetworkV6);

            Assert::ExpectException<null_pointer>([&point](){
                point.Address(nullptr);
            });
        }

        TEST_METHOD(IPEndPoint_GetPort)
        {
            IPEndPoint point(IPAddressPtr(new IPAddressMock(0)), 12345);
            IPEndPoint point6(IPAddressPtr(new IPAddressMock({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 })), 12345);
            Assert::IsTrue(12345 == point.Port());
            Assert::IsTrue(12345 == point6.Port());

            point.Port(54321);
            point6.Port(54321);
            Assert::IsTrue(54321 == point.Port());
            Assert::IsTrue(54321 == point6.Port());
        }
    };
}
