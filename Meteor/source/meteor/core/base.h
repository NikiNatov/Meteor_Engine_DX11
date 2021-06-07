#pragma once

#include <cstdint>
#include <memory>
#include "logger.h"

#if defined(METEOR_DEBUG)
    #define METEOR_ENGINE_ASSERT(x,...)  if(!(x)) { METEOR_ENGINE_LOG_ERROR(__VA_ARGS__); __debugbreak(); }
    #define METEOR_ASSERT(x,...)         if(!(x)) { METEOR_LOG_ERROR(__VA_ARGS__); __debugbreak(); }
#else
    #define METEOR_ENGINE_ASSERT(x,...)
    #define METEOR_ASSERT(x,...)
#endif // METEOR_DEBUG

#define METEOR_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }
#define BIT(x) 1 << x

namespace meteor
{
    // Types
    using mtrS8     = std::int8_t;
    using mtrS16    = std::int16_t;
    using mtrS32    = std::int32_t;
    using mtrS64    = std::int64_t;
    using mtrSByte  = mtrS8;
    using mtrInt    = mtrS32;
    
    using mtrU8     = std::uint8_t;
    using mtrU16    = std::uint16_t;
    using mtrU32    = std::uint32_t;
    using mtrU64    = std::uint64_t;
    using mtrByte   = mtrU8;
    using mtrUInt   = mtrU32;

    using mtrChar   = char;
    using mtrBool   = bool;
    using mtrFloat  = float;
    using mtrDouble = double;
    using mtrString = std::string;

    template<typename T>
    using Ref = std::shared_ptr<T>;

    template<typename T, typename... Args>
    Ref<T> CreateRef(Args&&... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    using ScopedPtr = std::unique_ptr<T>;

    template<typename T, typename... Args>
    ScopedPtr<T> CreateScoped(Args&&... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }
}