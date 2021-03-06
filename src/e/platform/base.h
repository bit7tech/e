//----------------------------------------------------------------------------------------------------------------------
// Defines the base class for all platforms.
//----------------------------------------------------------------------------------------------------------------------

#pragma once

#include <e/app/app.h>

namespace e
{

    class CommandLine;

    class Platform
    {
    public:
        Platform(App& game, const CommandLine& cmdLine)
            : m_app(game)
            , m_cmdLine(cmdLine)
            , m_keyState()
            , m_mouseState()
        {}

        virtual ~Platform()
        {}

        virtual int run() = 0;

        App& app() { return m_app; }
        const App& app() const { return m_app; }

        const CommandLine& cmdLine() const { return m_cmdLine; }

    protected:
        //
        // I/O
        //
        KeyState& keyState() { return m_keyState; }
        const KeyState& keyState() const { return m_keyState; }
        MouseState& mouseState() { return m_mouseState; }
        const MouseState& mouseState() const { return m_mouseState; }

        //
        // Time
        //
        virtual TimePoint timeNow() = 0;
        virtual TimePeriod timePeriod(TimePoint a, TimePoint b) = 0;
        virtual f64 timeToSecs(TimePeriod period) = 0;

    private:
        App&                m_app;
        const CommandLine&  m_cmdLine;
        KeyState            m_keyState;
        MouseState          m_mouseState;
    };

}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
