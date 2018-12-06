//----------------------------------------------------------------------------------------------------------------------
// Demo Game for testing AGF
//----------------------------------------------------------------------------------------------------------------------

#include <e/app/app.h>

class CommandLine;

class Editor : public e::App
{
public:
    Editor(const e::CommandLine& commandLine);
    ~Editor() override;

    bool simulate(const e::SimulateIn& sim) override;
    void present(const e::PresentIn& pin) override;
};

