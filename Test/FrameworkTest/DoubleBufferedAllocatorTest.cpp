#include "stdafx.h"
#include "CppUnitTest.h"
#include <Lupus\Memory\DoubleBufferedAllocator.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Lupus;

namespace FrameworkTest
{
    TEST_CLASS(DoubleBufferedAllocatorTest)
    {
    public:

        TEST_METHOD(DoubleBufferedAllocatorConstructor)
        {
            DoubleBufferAllocator(32 * KiB);
        }

        TEST_METHOD(DoubleBufferedAllocatorDestructor)
        {
            DoubleBufferAllocator* dba = new DoubleBufferAllocator(32 * KiB);
            delete dba;
        }

        TEST_METHOD(DoubleBufferedAllocatorAllocate)
        {
            DoubleBufferAllocator dba(32 * KiB);
            Assert::IsNotNull(dba.Allocate<S32>(32));
        }

        TEST_METHOD(DoubleBufferedAllocatorSwap)
        {
            DoubleBufferAllocator dba(32 * KiB);
            dba.Swap();
        }

        TEST_METHOD(DoubleBufferedAllocatorClear)
        {
            DoubleBufferAllocator dba(32 * KiB);
            dba.Clear();
        }
    };
}
