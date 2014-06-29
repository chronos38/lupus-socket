#include <Lupus/Memory/DoubleBufferedAllocator.h>
#include <Lupus/Memory/StackAllocator.h>

namespace Lupus {
        DoubleBufferAllocator::DoubleBufferAllocator(U32 stackBytes)
        {
            mStackAllocator[0] = new StackAllocator(stackBytes);
            mStackAllocator[1] = new StackAllocator(stackBytes);
        }

        DoubleBufferAllocator::~DoubleBufferAllocator()
        {
            if (mStackAllocator[0]) {
                delete mStackAllocator[0];
            }

            if (mStackAllocator[1]) {
                delete mStackAllocator[1];
            }
        }

        void DoubleBufferAllocator::Swap()
        {
            (++mCurrent) %= 2;
        }

        void DoubleBufferAllocator::Clear()
        {
            mStackAllocator[mCurrent]->Clear();
        }
}
