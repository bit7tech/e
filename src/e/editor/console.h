//----------------------------------------------------------------------------------------------------------------------
// Console data structure
//----------------------------------------------------------------------------------------------------------------------

#pragma once

#include <e/core.h>

#include <vector>

namespace e
{

    class ConsoleData
    {
    public:
        ConsoleData();

        struct Char
        {
            char    ch;
            char    buffer[3];
            u32     fore;
            u32     back;

            Char(char ch, u32 fore, u32 back)
                : ch(ch), fore(fore), back(back)
            {}

            Char() : Char(0, 0, 0) {}
        };

        func getLine(size_t line) const -> std::pair<const Char*, const Char*>;
        func clear() -> void;

        // This will invalidate any data returned from getLine().
        func outputLine(const std::string& line) -> void;

    private:
        func startLine() -> void;
        func outputChar(char c) -> void;
        func pushColour(u32 fore, u32 back) -> void;
        func popColour() -> void;

    private:
        std::vector<Char>                   m_chars;
        std::vector<size_t>                 m_lines;
        std::vector<std::pair<u32, u32>>    m_colourStack;
    };

}

