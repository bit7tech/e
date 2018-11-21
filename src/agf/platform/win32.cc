//----------------------------------------------------------------------------------------------------------------------
// Implementation of Win32 platform layer.
//----------------------------------------------------------------------------------------------------------------------

#include <agf/platform/win32.h>

namespace agf
{

    //------------------------------------------------------------------------------------------------------------------
    // Constructor

    Win32Platform::Win32Platform(Game& game, const CommandLine& cmdLine)
        : Platform(game, cmdLine)
    {

    }

    //------------------------------------------------------------------------------------------------------------------
    // Destructor

    Win32Platform::~Win32Platform()
    {

    }

    //------------------------------------------------------------------------------------------------------------------
    // run

    int Win32Platform::run()
    {
        return 0;
    }

    //------------------------------------------------------------------------------------------------------------------
    //------------------------------------------------------------------------------------------------------------------

}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
