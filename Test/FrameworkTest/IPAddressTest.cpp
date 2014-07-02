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

        TEST_METHOD(IPAddressConstructorIPv4)
        {
            IPAddress(0x12345678);
        }

        TEST_METHOD(IPAddressConstructorIPv6)
        {
            IPAddress({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 });

            Assert::ExpectException<std::length_error>([](){
                IPAddress({ 0, 0 });
            });

            IPAddress({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 0);
        }

        TEST_METHOD(IPAddressAddressBytes)
        {
            IPAddress addr(0x12345678);
            Vector<Byte> bytes = addr.Bytes();

            Assert::AreEqual<Byte>(0x12, bytes[0]);
            Assert::AreEqual<Byte>(0x34, bytes[1]);
            Assert::AreEqual<Byte>(0x56, bytes[2]);
            Assert::AreEqual<Byte>(0x78, bytes[3]);
        }

        TEST_METHOD(IPAddressFamily)
        {
            IPAddress addr(0x12345678);
            IPAddress addr6({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 0);

            Assert::IsTrue(AddressFamily::InterNetwork == addr.Family());
            Assert::IsTrue(AddressFamily::InterNetworkV6 == addr6.Family());
        }
    };
}
