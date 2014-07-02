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

        TEST_METHOD(DoubleBufferedAllocator_Constructor)
        {
            DoubleBufferAllocator(32 * KiB);
        }

        TEST_METHOD(DoubleBufferedAllocator_Destructor)
        {
            DoubleBufferAllocator* dba = new DoubleBufferAllocator(32 * KiB);
            dba->~DoubleBufferAllocator();
            delete dba;
        }

        TEST_METHOD(DoubleBufferedAllocator_Allocate)
        {
            DoubleBufferAllocator dba(32 * KiB);
            Assert::IsNotNull(dba.Allocate<S32>(32));
        }

        TEST_METHOD(DoubleBufferedAllocator_Swap)
        {
            DoubleBufferAllocator dba(32 * KiB);
            dba.Swap();
        }

        TEST_METHOD(DoubleBufferedAllocator_Clear)
        {
            DoubleBufferAllocator dba(32 * KiB);
            dba.Clear();
        }
    };
}
