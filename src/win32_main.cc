//----------------------------------------------------------------------------------------------------------------------
// Entry point for AGF game.
//
// In the main you should create two things:
//
//      1) The Game object instance (a C++ class derived from e::Game).
//      2) The Platform object instance (a C++ class dervied from e::Platform), which binds the OS to the game object.
//
// Finally, you should call run() on your platform object.
//----------------------------------------------------------------------------------------------------------------------

#include <e/platform/win32.h>
#include <e/app/app.h>
#include <e/cmdline.h>

// For the purposes of this framework, this is a demo "game".  When using this as a basis for your own game, delete
// the folder src/e/demo, and create your own Game-derived class.  Also change the reference to it in the main
// function.
#include <e/editor/editor.h>

#include <crtdbg.h>

//----------------------------------------------------------------------------------------------------------------------
// eMain
//
// The purpose of this function is to construct the Game and Platform objects and link them together, then bootstrap
// the game.
//----------------------------------------------------------------------------------------------------------------------

int eMain(int argc, char** argv)
{
    // This is a simple utility that processes and categorises each token in the command line.
    e::CommandLine cmdLine(argc, argv);

    e::Editor e(cmdLine);
    e::Win32Platform platform(e, cmdLine);

    return platform.run();
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    _CrtSetBreakAlloc(0);
    int result = eMain(__argc, __argv);
    _CrtDumpMemoryLeaks();
    return result;
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
