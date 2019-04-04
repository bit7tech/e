//----------------------------------------------------------------------------------------------------------------------
// Console data structure
//----------------------------------------------------------------------------------------------------------------------

#include <e/editor/console.h>

using namespace std;

namespace e
{

    //------------------------------------------------------------------------------------------------------------------
    // Constructor

    ConsoleData::ConsoleData()
    {
        m_colourStack.emplace_back(make_pair<u32, u32>(0xff7f7f7f, 0xff000000));
    }

    //------------------------------------------------------------------------------------------------------------------
    // getLine

    func ConsoleData::getLine(size_t line) const -> pair<const Char*, const Char*>
    {
        if (line >= m_lines.size()) return { nullptr, nullptr };
        if (line == m_lines.size() - 1)
        {
            return {
                m_chars.data() + m_lines[line],
                m_chars.data() + m_chars.size()
            };
        }
        else
        {
            return {
                m_chars.data() + m_lines[line],
                m_chars.data() + m_lines[line + 1]
            };
        }
    }

    //------------------------------------------------------------------------------------------------------------------
    // clear

    func ConsoleData::clear() -> void
    {
        m_chars.clear();
        m_lines.clear();
        assert(m_colourStack.size() == 1);
    }

    //------------------------------------------------------------------------------------------------------------------
    // outputLine

    func ConsoleData::outputLine(const string& line) -> void
    {
        startLine();
        for (const auto& ch : line)
        {
            outputChar(ch);
        }
    }

    //------------------------------------------------------------------------------------------------------------------
    // startLine

    func ConsoleData::startLine() -> void
    {
        m_lines.emplace_back(m_chars.size());
    }

    //------------------------------------------------------------------------------------------------------------------
    // outputChar

    func ConsoleData::outputChar(char c) -> void
    {
        assert(m_colourStack.size() >= 1);

        m_chars.emplace_back(c, m_colourStack.back().first, m_colourStack.back().second);
    }

    //------------------------------------------------------------------------------------------------------------------
    // pushColour

    func ConsoleData::pushColour(u32 fore, u32 back) -> void
    {
        m_colourStack.emplace_back(make_pair(fore, back));
    }

    //------------------------------------------------------------------------------------------------------------------
    // popColour

    func ConsoleData::popColour() -> void
    {
        m_colourStack.erase(m_colourStack.end() - 1);
    }

    //------------------------------------------------------------------------------------------------------------------
    //------------------------------------------------------------------------------------------------------------------

}