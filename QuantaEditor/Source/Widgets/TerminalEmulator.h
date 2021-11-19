#pragma once

namespace Quanta
{
    struct Pty 
    {
        int master = 0;
        int slave = 0;
    };

    struct TerminalEmulator
    {
        Pty pty;

        bool open = true;
        const char* name = "TerminalEmulator";
        char input[100] { };
        const char* shell = nullptr;

        void Render();
    };  
}