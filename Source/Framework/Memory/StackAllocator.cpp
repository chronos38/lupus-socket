#include <Lupus/Memory/StackAllocator.h>

namespace Lupus {
    StackAllocator::StackAllocator(U32 maxBytes)
    {
        mMaxSize = mSize = maxBytes;
        mHead = mBlock = new Byte[maxBytes];
    }

    StackAllocator::~StackAllocator()
    {
        if (mBlock) {
            delete[] mBlock;
        }
    }

    UIntPtr StackAllocator::GetMarker() const
    {
        return (UIntPtr)mHead;
    }

    void StackAllocator::FreeToMarker(UIntPtr marker)
    {
        if (marker < (UIntPtr)mBlock || marker > (UIntPtr)mBlock + mMaxSize) {
            throw std::out_of_range("Given marker is out of range");
        }

        Byte* head = mHead;
        mHead = (Byte*)marker;
        mSize += (U32)(head - mHead);
    }

    void StackAllocator::Clear()
    {
        mHead = mBlock;
    }
}
