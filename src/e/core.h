//----------------------------------------------------------------------------------------------------------------------
// Basic types and definitions
//----------------------------------------------------------------------------------------------------------------------

#pragma once

#include <codecvt>
#include <cstdint>
#include <sstream>
#include <string>
#include <vector>

#define func auto

//----------------------------------------------------------------------------------------------------------------------
// Platform detection
//----------------------------------------------------------------------------------------------------------------------

#pragma once

#define YES (1)
#define NO (0)

// Compiler defines
#define AGF_COMPILER_MSVC     NO

// OS defines
#define AGF_OS_WIN32          NO

// CPU defines
#define AGF_CPU_X86           NO
#define AGF_CPU_X64           NO

// Configuration defines
#define AGF_DEBUG             NO
#define AGF_RELEASE           NO

// Endianess
#define AGF_LITTLE_ENDIAN     YES
#define AGF_BIG_ENDIAN        NO

//----------------------------------------------------------------------------------------------------------------------
// Compiler determination

#ifdef _MSC_VER
#   undef AGF_COMPILER_MSVC
#   define AGF_COMPILER_MSVC YES
#else
#   error Unknown compiler.  Please define COMPILE_XXX macro for your compiler.
#endif

//----------------------------------------------------------------------------------------------------------------------
// OS determination

#ifdef _WIN32
#   undef AGF_OS_WIN32
#   define AGF_OS_WIN32 YES
#else
#   error Unknown OS.  Please define OS_XXX macro for your operating system.
#endif

//----------------------------------------------------------------------------------------------------------------------
// CPU determination

#if AGF_COMPILER_MSVC
#   if defined(_M_X64)
#       undef AGF_CPU_X64
#       define AGF_CPU_X64 YES
#   elif defined(_M_IX86)
#       undef AGF_CPU_X86
#       define AGF_CPU_X86 YES
#   else
#       error Can not determine processor - something's gone very wrong here!
#   endif
#else
#   error Add CPU determination code for your compiler.
#endif

//----------------------------------------------------------------------------------------------------------------------
// Configuration

#ifdef _DEBUG
#   undef AGF_DEBUG
#   define AGF_DEBUG YES
#else
#   undef AGF_RELEASE
#   define AGF_RELEASE YES
#endif

//----------------------------------------------------------------------------------------------------------------------
// Standard headers

#if AGF_OS_WIN32
#   define WIN32_LEAN_AND_MEAN
#   define NOMINMAX
#   include <Windows.h>
#   ifdef _DEBUG
#       include <crtdbg.h>
#   endif
#endif

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
// Debugging
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------

#if AGF_OS_WIN32 && AGF_DEBUG
#   define AGF_BREAK() ::DebugBreak()
#else
#   define AGF_BREAK() do {} while(0)
#endif

#define AGF_ASSERT(x, ...) assert(x)

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------

namespace e
{

    using u8 = uint8_t;
    using u16 = uint16_t;
    using u32 = uint32_t;
    using u64 = uint64_t;

    using i8 = int8_t;
    using i16 = int16_t;
    using i32 = int32_t;
    using i64 = int64_t;

    using f32 = float;
    using f64 = double;

    using string = std::string;
    template <typename T> using vector = std::vector<T>;

    //------------------------------------------------------------------------------------------------------------------
    // Time
    //------------------------------------------------------------------------------------------------------------------

#if AGF_OS_WIN32
    using TimePoint = LARGE_INTEGER;
    using TimePeriod = LARGE_INTEGER;
#else
#   error TimePoint and TimePeriod undefined for your platform.
#endif

    //------------------------------------------------------------------------------------------------------------------
    // String format
    //------------------------------------------------------------------------------------------------------------------

    //------------------------------------------------------------------------------------------------------------------
    // Character conversion routines
    //------------------------------------------------------------------------------------------------------------------

    inline func isDigit(char c) -> bool
    {
        return ('0' <= c) && (c <= '9');
    }

    inline func toDigit(char c) -> int
    {
        return c - '0';
    }

    //------------------------------------------------------------------------------------------------------------------
    // Internal code
    //------------------------------------------------------------------------------------------------------------------

    namespace {

        template <int N>
        struct Counter
        {
            int m_count[N];
            int m_braceEscapes;
            int m_plainChars;

            Counter() : m_braceEscapes(0), m_plainChars(0)
            {
                memset(m_count, 0, sizeof(int) * N);
            }

            func onMarker(int markerNumber) -> void
            {
                ++m_count[markerNumber];
            }

            func onEscapeLeft() -> void { ++m_braceEscapes; }
            func onEscapeRight() -> void { ++m_braceEscapes; }
            func onChar(char c) -> void { ++m_plainChars; }
        };

        template <int N>
        class Formatter
        {
        public:
            Formatter(string& dest, string* values)
                : m_dest(dest)
                , m_size(0)
                , m_values(values)
            {}

            func onMarker(int markerNumber) -> void
            {
                size_t len = m_values[markerNumber].length();
                m_dest.replace(m_size, len, m_values[markerNumber]);
                m_size += len;
            }

            func onEscapeLeft() -> void
            {
                m_dest[m_size++] = '{';
            }

            func onEscapeRight() -> void
            {
                m_dest[m_size++] = '}';
            }

            func onChar(char c) -> void
            {
                m_dest[m_size++] = c;
            }

        private:
            string & m_dest;
            size_t m_size;
            string* m_values;
        };

        template <int N, typename Handler>
        inline func traverse(const char* format, Handler& callback) -> void
        {
            const char* c = format;
            while (*c != 0)
            {
                if (*c == '{')
                {
                    if (c[1] == '{')
                    {
                        callback.onEscapeLeft();
                        c += 2;
                    }
                    else
                    {
                        int number = 0;
                        ++c;

                        // Unexpected end of string
                        AGF_ASSERT(*c != 0);

                        // Invalid brace contents: must be a positive integer
                        AGF_ASSERT(isDigit(*c));

                        number = toDigit(*c++);
                        while (isDigit(*c))
                        {
                            number *= 10;
                            number += toDigit(*c++);
                        }

                        // Unexpected end of string
                        AGF_ASSERT(*c != 0);
                        // Invalid brace contents: must be a positive integer
                        AGF_ASSERT(*c == '}');
                        // Format value index is out of range
                        AGF_ASSERT(number <= N);

                        callback.onMarker(number);
                        ++c;
                    } // if (c[1] == '{')
                }
                else if (*c == '}')
                {
                    if (c[1] == '}')
                    {
                        callback.onEscapeRight();
                        c += 2;
                    }
                    else
                    {
                        // Unescaped right brace
                        AGF_ASSERT(0);
                    }
                }
                else
                {
                    callback.onChar(*c);
                    ++c;
                }
            }
        } // traverse

        template <int N>
        inline func formattedTotal(string values[N], int counts[N]) -> size_t
        {
            size_t total = 0;
            for (int i = 0; i < N; ++i)
            {
                total += values[i].length() * counts[i];
            }
            return total;
        }

        template <int N>
        inline func formatArray(const char* format, string values[N]) -> string
        {
            Counter<N> counter;
            traverse<N>(format, counter);

            size_t formatsSize = formattedTotal<N>(values, counter.m_count);
            size_t outputTotal = formatsSize + counter.m_braceEscapes + counter.m_plainChars;

            string output(outputTotal, 0);
            Formatter<N> formatter(output, values);
            traverse<N>(format, formatter);

            return output;
        }

        template <int N, typename T>
        inline func internalStringFormat(int index, string values[N], T t) -> void
        {
            std::ostringstream ss;
            ss << t;
            values[index] = ss.str();
        }

        template <int N, typename T, typename ...Args>
        inline func internalStringFormat(int index, string values[N], T t, Args... args) -> void
        {
            std::ostringstream ss;
            ss << t;
            values[index] = ss.str();
            internalStringFormat<N, Args...>(index + 1, values, args...);
        }

        template <int N>
        inline func internalStringFormat(int index, string values[N]) -> void
        {
        }

    } // namespace

    inline func stringFormat(const char* format) -> string
    {
        return string(format);
    }

    template <typename ...Args>
    inline func stringFormat(const char* format, Args... args) -> string
    {
        string values[sizeof...(args)];
        internalStringFormat<sizeof...(args), Args...>(0, values, args...);
        return formatArray<sizeof...(args)>(format, values);
    }

    template <typename... Args>
    inline func pr(Args... args) -> void
    {
        string s = stringFormat(args...);
#ifdef WIN32
        OutputDebugStringA(s.c_str());
#endif
    }

    template <typename... Args>
    inline func prn(Args... args) -> void
    {
        pr(args...);
        OutputDebugStringA("\n");
    }

    //------------------------------------------------------------------------------------------------------------------

    template <typename T>
    T min(T a, T b)
    {
        return a < b ? a : b;
    }

    template <typename T>
    T max(T a, T b)
    {
        return a < b ? b : a;
    }

    //------------------------------------------------------------------------------------------------------------------

}

