#include "stdafx.h"
#include "CppUnitTest.h"
#include <Lupus\Memory\StackAllocator.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Lupus;

namespace FrameworkTest
{		
	TEST_CLASS(StackAllocatorTest)
	{
	public:
		
		TEST_METHOD(StackAllocatorConstructor)
		{
            StackAllocator(32 * KiB);
		}

        TEST_METHOD(StackAllocatorAllocate)
        {
            StackAllocator allocator(32 * KiB);
            Assert::IsNotNull(allocator.Allocate<U8>(32));
            Assert::IsNotNull(allocator.Allocate<U16>(32));
            Assert::IsNotNull(allocator.Allocate<U32>(32));
            Assert::IsNotNull(allocator.Allocate<U64>(32));
        }

        TEST_METHOD(StackAllocatorFreeToMarker)
        {
            StackAllocator allocator(32 * KiB);
            UIntPtr marker = allocator.GetMarker();
            allocator.Allocate<U32>(128);
            allocator.FreeToMarker(marker);
            Assert::AreEqual(marker, allocator.GetMarker());

            Assert::ExpectException<std::out_of_range>([]() {
                StackAllocator allocator(32 * KiB);
                UIntPtr marker = allocator.GetMarker();
                marker += 64 * KiB;
                allocator.FreeToMarker(marker);
            });
        }

        TEST_METHOD(StackAllocatorClear)
        {
            StackAllocator allocator(32 * KiB);
            UIntPtr m1 = allocator.GetMarker();
            allocator.Allocate<U32>(128);
            UIntPtr m2 = allocator.GetMarker();
            allocator.Allocate<U32>(128);
            UIntPtr m3 = allocator.GetMarker();
            allocator.Clear();
            Assert::AreEqual(m1, allocator.GetMarker());
            Assert::AreNotEqual(m2, allocator.GetMarker());
            Assert::AreNotEqual(m3, allocator.GetMarker());
        }
	};
}
