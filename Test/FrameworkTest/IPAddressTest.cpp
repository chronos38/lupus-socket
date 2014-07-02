#include "stdafx.h"
#include "CppUnitTest.h"
#include <Lupus\Network\IPAddress.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Lupus;

namespace FrameworkTest
{
    TEST_CLASS(IPAddressTest)
    {
    public:

        TEST_METHOD(IPAddress_ConstructorIPv4)
        {
            IPAddress(0x12345678);
        }

        TEST_METHOD(IPAddress_ConstructorIPv6)
        {
            IPAddress({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 });

            Assert::ExpectException<std::length_error>([](){
                IPAddress({ 0, 0 });
            });

            IPAddress({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 0);
        }

        TEST_METHOD(IPAddress_Bytes)
        {
            IPAddress addr(0x12345678);
            Vector<Byte> bytes = addr.Bytes();

            Assert::AreEqual<Byte>(0x12, bytes[0]);
            Assert::AreEqual<Byte>(0x34, bytes[1]);
            Assert::AreEqual<Byte>(0x56, bytes[2]);
            Assert::AreEqual<Byte>(0x78, bytes[3]);
        }

        TEST_METHOD(IPAddress_Family)
        {
            IPAddress addr(0x12345678);
            IPAddress addr6({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 0);

            Assert::IsTrue(AddressFamily::InterNetwork == addr.Family());
            Assert::IsTrue(AddressFamily::InterNetworkV6 == addr6.Family());
        }

        TEST_METHOD(IPAddress_ToString)
        {
            IPAddress addr(0x12345678);
            IPAddress addr6({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 }, 0);

            Assert::AreEqual<String>("18.52.86.120", addr.ToString());
            Assert::AreEqual<String>("::1", addr6.ToString());
        }

        TEST_METHOD(IPAddress_Parse_1)
        {
            IPAddressPtr address = IPAddress::Parse("18.52.86.120");
            Vector<Byte> bytes = address->Bytes();

            Assert::AreEqual<Byte>(0x12, bytes[0]);
            Assert::AreEqual<Byte>(0x34, bytes[1]);
            Assert::AreEqual<Byte>(0x56, bytes[2]);
            Assert::AreEqual<Byte>(0x78, bytes[3]);
        }

        TEST_METHOD(IPAddress_Parse_2)
        {
            IPAddressPtr address = IPAddress::Parse("::1");
            Vector<Byte> bytes = address->Bytes();

            for (size_t i = 0; i < 15; i++) {
                Assert::AreEqual<Byte>(0, bytes[i]);
            }

            Assert::AreEqual<Byte>(0x01, bytes[15]);
        }

        TEST_METHOD(IPAddress_Parse_3)
        {
            Assert::ExpectException<std::invalid_argument>([](){
                IPAddress::Parse("foo");
            });
        }

        TEST_METHOD(IPAddress_TryParse_1)
        {
            IPAddressPtr address;

            Assert::IsTrue(IPAddress::TryParse("18.52.86.120", address));
            Assert::IsTrue(address.get() != nullptr);
        }

        TEST_METHOD(IPAddress_TryParse_2)
        {
            IPAddressPtr address;

            Assert::IsTrue(IPAddress::TryParse("::1", address));
            Assert::IsTrue(address.get() != nullptr);
        }

        TEST_METHOD(IPAddress_TryParse_3)
        {
            IPAddressPtr address;

            Assert::IsFalse(IPAddress::TryParse("foo", address));
            Assert::IsTrue(address.get() == nullptr);
        }

        TEST_METHOD(IPAddress_IsLoopback_1)
        {
            Assert::IsTrue(IPAddress::IsLoopback(IPAddress::Loopback));
            Assert::IsTrue(IPAddress::IsLoopback(IPAddress::IPv6Loopback));
        }

        TEST_METHOD(IPAddress_IsLoopback_2)
        {
            Assert::IsTrue(IPAddress::IsLoopback(IPAddressPtr(new IPAddress(0x7F000001))));
            Assert::IsFalse(IPAddress::IsLoopback(IPAddressPtr(new IPAddress(0x12345678))));
        }

        TEST_METHOD(IPAddress_IsLoopback_3)
        {
            Assert::IsTrue(IPAddress::IsLoopback(IPAddressPtr(new IPAddress({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 }))));
            Assert::IsFalse(IPAddress::IsLoopback(IPAddressPtr(new IPAddress({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 }))));
        }
    };
}
