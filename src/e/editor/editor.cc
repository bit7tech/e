//----------------------------------------------------------------------------------------------------------------------
// Demo game to demonstrate AGF's features.
// Designed to switch screens using function keys F1-F12.
//----------------------------------------------------------------------------------------------------------------------

#include <e/editor/editor.h>

#include <algorithm>

namespace e
{

    static f64 kCursorTime = 0.75;

    //------------------------------------------------------------------------------------------------------------------
    // Constructor

    Editor::Editor(const e::CommandLine& commandLine)
        : App(commandLine)
        , m_console()
        , m_topLine(0)
    {
        m_console.outputLine("");
        m_console.outputLine("Hello, World!");
    }

    //------------------------------------------------------------------------------------------------------------------
    // Destructor

    Editor::~Editor()
    {

    }

    //------------------------------------------------------------------------------------------------------------------
    // Simulation

    bool Editor::simulate(const e::SimulateIn& sim)
    {
        if (sim.key.down && sim.key.vkey == e::Key::Escape)
        {
            return false;
        }

        //
        // Update cursor
        //
        m_cursor.time += sim.dt;
        if (m_cursor.time > kCursorTime)
        {
            m_cursor.time = 0.0;
        }

        return true;
    }

    //------------------------------------------------------------------------------------------------------------------
    // Presentation

    void Editor::present(const e::PresentIn& pin)
    {
        u32* textImage = pin.textImage;
        u32* foreImage = pin.foreImage;
        u32* backImage = pin.backImage;

        for (int line = 0; line < pin.height; ++line)
        {
            const auto lineStr = m_console.getLine(m_topLine + line);
            if (lineStr.first == nullptr)
            {
                // Blank line
                for (int i = 0; i < pin.width; ++i)
                {
                    *textImage++ = u32(' ');
                    *foreImage++ = 0xff000000;
                    *backImage++ = 0xff000000;
                }
            }
            else
            {
                int lineSize = min(int(lineStr.second - lineStr.first), pin.width);
                for (int i = 0; i < lineSize; ++i)
                {
                    *textImage++ = u32(lineStr.first[i].ch);
                    *foreImage++ = lineStr.first[i].fore;
                    *backImage++ = lineStr.first[i].back;
                }
                for (int i = lineSize; i < pin.width; ++i)
                {
                    *textImage++ = u32(' ');
                    *foreImage++ = 0xff000000;
                    *backImage++ = 0xff000000;
                }
            }
        }

        //
        // Render cursor
        //
        if (m_cursor.time < (kCursorTime / 2.0))
        {
            pin.backImage[m_cursor.y * pin.width + m_cursor.x] = m_cursor.colour;
        }
    }

}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
