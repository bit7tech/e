//----------------------------------------------------------------------------------------------------------------------
// Demo Game for testing AGF
//----------------------------------------------------------------------------------------------------------------------

#include <e/app/app.h>

class CommandLine;

namespace e
{

    struct CursorInfo
    {
        bool    enable;     // Cursor showing?
        f64     time;       // Current animation time.
        int     x;          // Cursor's local x position.
        int     y;          // Cursor's local y position.
        u32     colour;     // Cursor's colour.

        CursorInfo()
            : enable(true)
            , time(0.0)
            , x(0)
            , y(0)
            , colour(0xff0000ff)
        {}
    };

    class Editor : public App
    {
    public:
        Editor(const CommandLine& commandLine);
        ~Editor() override;

        bool simulate(const SimulateIn& sim) override;
        void present(const PresentIn& pin) override;

    private:
        //
        // Cursor information
        //
        CursorInfo  m_cursor;
    };

}
