#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <imgui.h>
#include <libgen.h>
#include <unistd.h> //replace this, not cross-platform
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <Quanta/CLI/Log/Log.h>

#include "TerminalEmulator.h"

namespace Quanta
{
    static Pty Open()
    {
        Pty pty;

        pty.master = posix_openpt(O_RDWR | O_NOCTTY);

        if (pty.master == -1)
        {
            Log::Write(Log::Level::Error, "Failed to open pseudoterminal");

            return pty;
        };

        if (grantpt(pty.master) == -1)
        {
            Log::Write(Log::Level::Error, "Failed to grant pseudoterminal privelages");

            return pty;
        }

        if (unlockpt(pty.master) == -1)
        {
            return pty;
        }

        const auto slaveName = ptsname(pty.master);

        if (slaveName == nullptr) 
        {
            Log::Write(Log::Level::Error, "Failed to create pseudoterminal slave");

            return pty;
        }

        pty.slave = open(slaveName, O_RDWR | O_NOCTTY);

        if (pty.slave == -1) 
        {
            Log::Write(Log::Level::Error, "Failed to open pseudoterminal slave");
            
            return pty;
        }

        Log::Write(Log::Level::Debug, "Successfully opened pseudoterminal");

        return pty;
    };

    bool Spawn(const Pty& pty, const char* const shellPath)
    {
        const auto pid = fork();

        char* env[] = { nullptr };

        if (pid == 0)
        {
            close(pty.master);

            setsid();

            if (ioctl(pty.slave, TIOCSCTTY, nullptr) == -1)
            {
                Log::Write(Log::Level::Error, "Failed to create terminal session");

                return false;
            }

            dup2(pty.slave, STDIN_FILENO);
            dup2(pty.slave, STDOUT_FILENO);
            dup2(pty.slave, STDERR_FILENO);

            close(pty.slave);

            execle(shellPath, "-" "/bin/bash", nullptr, env);
        }
        else if (pid > 0)
        {
            close(pty.slave);

            return true;
        }

        Log::Write(Log::Level::Error, "Failed to fork");

        return false;
    }

    void TerminalEmulator::Render()
    {
        static auto createdPty = false;

        if (!createdPty)
        {
            pty = Open();

            if (!Spawn(pty, shell))
            {
                Log::Write(Log::Level::Error, "Failed to spawn shell");
            }

            createdPty = true;
        }
        
        if (!open)
        {
            return;
        }

        if (ImGui::Begin(name, &open))
        {
            ImGui::End();

            return;
        }

        static char buffer[100];

        fd_set readable;

        std::memset(buffer, '\0', sizeof(buffer));

        static bool done = false;

        for (int i = 0; i < sizeof(buffer) && !done; i++)
        {
            FD_ZERO(&readable);

            FD_SET(pty.master, &readable);

            if (select(pty.master + 1, &readable, nullptr, nullptr, nullptr) == -1) 
            {
                Log::Write(Log::Level::Error, "Failed to select");

                break;
            }

            if (!FD_ISSET(pty.master, &readable)) 
            {
                break;
            }

            continue;

            if (read(pty.master, &buffer[i], 1) <= 0)
            {
                Log::Write(Log::Level::Error, "Nothing to read from child process");

                break;
            }
        }
        
        done = true;

        ImGui::TextUnformatted(buffer, buffer + sizeof(buffer));

        ImGui::BeginChild("Input Area");

        if (ImGui::InputText("Shell", input, sizeof(input), ImGuiInputTextFlags_EnterReturnsTrue))
        {   
            std::memset(input, '\0', sizeof(input));
        }

        ImGui::EndChild();

        ImGui::End();
    }
}