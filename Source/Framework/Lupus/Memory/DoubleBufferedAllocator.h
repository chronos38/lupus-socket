#pragma once

#include <Lupus/Definitions.h>
#include <Lupus/Memory/StackAllocator.h>

namespace Lupus {
    class StackAllocator;

    //! Bietet einen persistenten Speicher über die Länge von zwei Frames.
    class LUPUS_API DoubleBufferAllocator : public ReferenceType
    {
    public:

        /*! 
         * Erstellt einen neuen DoubleBuffer. Die übergebene Größe gilt für
         * einen Stack. Die Gesamtgröße mach somit singlStackSize * 2 aus.
         *
         * @param[in]   singleStackSize Die Bytegröße pro Stack.
         */
        explicit DoubleBufferAllocator(U32 singleStackSize) NOEXCEPT;
        virtual ~DoubleBufferAllocator();

        /*!
         * Wechselt die Stacks aus. Alle neuen Speicheranforderungen werden vom
         * neuen Stack verarbeitet. Der Stack wird nicht automatisch bereinigt,
         * was bedeutet dass sich noch Daten darin befinden können.
         */
        virtual void Swap() NOEXCEPT;

        /*!
         * Bereinigt den aktiven Stack und setzt seine Größe auf Null.
         */
        virtual void Clear() NOEXCEPT;

        /*!
         * Fordert Speicher für den angegebenen Typ an. Falls nicht genug
         * Speicher vorhanden ist, dann wird nullptr retouniert.
         *
         * \tparam      T       Datentyp des Objekts.
         * \param[in]   count   Anzahl der angeforderten Objekte.
         *
         * \returns Zeiger auf das allozierte Objekt, oder einen nullptr wenn
         *          nicht genug Speicher vorhanden ist.
         */
        template <typename T>
        T* Allocate(U32 count = 1) NOEXCEPT
        {
            return mStackAllocator[mCurrent]->Allocate<T>(count);
        }

    private:

        //! Standardkonstruktor ist nicht erlaubt.
        DoubleBufferAllocator() = delete;

        U32 mCurrent = 0;
        StackAllocator* mStackAllocator[2];
    };

    typedef Pointer<DoubleBufferAllocator> DoubleBufferAllocatorPtr;
}
