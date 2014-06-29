#pragma once

#ifdef _MSC_VER
#pragma warning(disable: 4290)

#ifdef LUPUS_EXPORT
#define LUPUS_API __declspec(dllexport)
#else
#define LUPUS_API __declspec(dllimport)
#endif

#define NOEXCEPT throw()

#elif __CYGWIN

#ifdef LUPUS_EXPORT
#define LUPUS_API __attribute__ ((dllexport))
#else
#define LUPUS_API __attribute__ ((dllimport))
#endif

#else

#if __GNUC__ >= 4
#define LUPUS_API __attribute__ ((visibility ("default")))
#else
#define LUPUS_API
#endif

#define NOEXCEPT noexcept
#endif

// STD

#include <cstdint>
#include <string>
#include <vector>
#include <deque>
#include <list>
#include <memory>
#include <fstream>
#include <functional>
#include <atomic>
#include <map>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <exception>

// Exception

#define DefineError(cls) \
    class cls : public virtual std::exception \
    { \
        Lupus::String mMessage; \
    public: \
        inline cls(const Lupus::String& message = "") : mMessage(message) { } \
        virtual ~cls() = default; \
        cls& operator=(const cls&) = default; \
        virtual inline const char* what() const override { return mMessage.c_str(); } \
    };

// Boost

#include <boost/noncopyable.hpp>

namespace Lupus {

    typedef int8_t S8; //!< Vorzeichenbehaftete 8-Bit Ganzzahl
    typedef uint8_t U8; //!< Vorzeichenlose 8-Bit Ganzzahl
    typedef int16_t S16; //!< Vorzeichenbehaftete 16-Bit Ganzzahl
    typedef uint16_t U16; //!< Vorzeichenlose 16-Bit Ganzzahl
    typedef int32_t S32; //!< Vorzeichenbehaftete 32-Bit Ganzzahl
    typedef uint32_t U32; //!< Vorzeichenlose 32-Bit Ganzzahl
    typedef int64_t S64; //!< Vorzeichenbehaftete 64-Bit Ganzzahl
    typedef uint64_t U64; //!< Vorzeichenlose 64-Bit Ganzzahl
    typedef intptr_t IntPtr; //!< Vorzeichenbehafteter Zeiger
    typedef uintptr_t UIntPtr; //!< Vorzeichenlose Zeiger

    typedef U8 Byte; //!< Repräsentiert einen einzelnen Byte (8-Bit)

    typedef std::string String;

    template <typename T>
    using Vector = std::vector < T > ;

    template <typename T>
    using Deque = std::deque < T > ;

    template <typename T>
    using List = std::list < T > ;

    template <typename T>
    using Pointer = std::shared_ptr < T > ;

    template <typename T>
    using WeakPointer = std::weak_ptr < T > ;

    template <typename T>
    using UniquePointer = std::unique_ptr < T > ;

    template <typename T>
    using Function = std::function < T > ;

    template <typename T>
    using Atomic = std::atomic < T > ;

    template <typename Key, typename Value>
    using Tree = std::map < Key, Value > ;

    template <typename Key, typename Value>
    using Hash = std::unordered_map < Key, Value > ;

    template <typename T>
    using LockGuard = std::lock_guard < T > ;

    typedef std::mutex Mutex;
    typedef std::thread Thread;
    typedef std::ifstream InputFileStream;
    typedef std::ofstream OutputFileStream;

    // Reference Type

    typedef boost::noncopyable ReferenceType;

    // Byte Types

    static const U32 KiB = 1024; //!< Kibibyte
    static const U32 MiB = 1024 * 1024; //!< Mebibyte
    static const U32 GiB = 1024 * 1024 * 1024; //!< Gibibyte
}

// Global errors

DefineError(null_pointer)
