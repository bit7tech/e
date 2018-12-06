//----------------------------------------------------------------------------------------------------------------------
// Demo game to demonstrate AGF's features.
// Designed to switch screens using function keys F1-F12.
//----------------------------------------------------------------------------------------------------------------------

#include <e/editor/editor.h>

namespace e
{

    static f64 kCursorTime = 0.75;

    //------------------------------------------------------------------------------------------------------------------
    // Constructor

    Editor::Editor(const e::CommandLine& commandLine)
        : App(commandLine)
    {

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
        for (int i = 0; i < pin.width * pin.height; ++i)
        {
            pin.foreImage[i] = 0xffffffff;
            pin.backImage[i] = 0xff000000;
            pin.textImage[i] = 0;
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
