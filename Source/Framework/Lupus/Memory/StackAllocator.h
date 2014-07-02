#pragma once

#include <Lupus/Definitions.h>
#include <type_traits>

namespace Lupus {
    //! Stack basierter Allocator der das markieren von Speicher erlaubt.
    class LUPUS_API StackAllocator : public ReferenceType
    {
    public:

        /*!
         * Erstellt einen neuen Stack mit der angegebenen Bytegröße.
         *
         * \param[in]   maxBytes    Die Größe des Stacks in Bytes.
         */
        explicit StackAllocator(U32 maxBytes) NOEXCEPT;
        virtual ~StackAllocator();

        /*!
         * Fordert neuen Speicher an. Falls nicht genug Platz vorhanden ist,
         * dann wird ein nullptr retouniert.
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
            const U32 bytes = sizeof(T) * count;
            const U32 align = std::alignment_of<T>::value;

            void* ptr = (void*)mHead;

            if (std::align(align, bytes, ptr, mSize)) {
                T* result = (T*)ptr;
                mHead = (Byte*)ptr;
                mHead += bytes;
                mSize -= bytes;
                return result;
            }

            return nullptr;
        }

        /*!
         * Markiert den jetzigen Speicherbereich und retouniert den Wert als
         * UIntPtr.
         *
         * \returns Markierung als UIntPtr.
         */
        virtual UIntPtr GetMarker() const NOEXCEPT;

        /*!
         * Gibt den Speicher bis zur markierten Stelle frei. Der Speicher wird
         * nicht direkt freigegeben, da lediglich der Zeiger auf die Stelle
         * der Markierung gesetzt wird.
         *
         * Falls noch alte Werte im freigebenen Speicherbereich bearbeitet
         * werden dann führt dies zu undefinierten Verhalten.
         *
         * \param[in]   marker  Die markierte Stell im Speicher.
         */
        virtual void FreeToMarker(UIntPtr marker) throw(std::out_of_range);

        /*!
         * Setzt den Kopf auf die Basis zurück. Dadurch werden sümtliche noch
         * vorhandenen Werte/Zeiger im Bereich des Stacks ungültig.
         *
         * Falls noch alte Werte im freigebenen Speicherbereich bearbeitet
         * werden dann führt dies zu undefinierten Verhalten.
         */
        virtual void Clear() NOEXCEPT;

    private:

        //! Standardkonstruktor ist nicht erlaubt.
        StackAllocator() = delete;

        Byte* mBlock;
        Byte* mHead;
        U32 mSize;
        U32 mMaxSize;
    };

    typedef Pointer<StackAllocator> StackAllocatorPtr;
}
