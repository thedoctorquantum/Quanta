#include <imgui.h>
#include <Quanta/CLI/Shell/Shell.h>
#include <cstring>

#include "LogWidget.h"

namespace Quanta
{
    struct LogFormat final
    {
        ImVec4 color { 1.0f, 1.0f, 1.0f, 1.0f };
        const char* message = "%s\n";
    };
    
    static LogFormat GetLogFormat(const Log::Level level)
    {
        switch (level)
        {
            case Log::Level::Trace:
                return { { 1.0f, 1.0f, 1.0f, 0.8f }, "[Trace] %s" };
            case Log::Level::Information:
                return { { 0.0f, 1.0f, 0.0f, 0.8f }, "[Information] %s" };
            case Log::Level::Debug:
                return { { 0.0f, 1.0f, 0.0f, 0.8f }, "[Debug] %s" };
            case Log::Level::Warning:
                return { { 1.0f, 1.0f, 0.0f, 0.8f }, "[Warning] %s" };
            case Log::Level::Error:
                return { { 1.0f, 0.0f, 0.0f, 0.8f }, "[Error] %s" };
            case Log::Level::Fatal:
                return { { 1.0f, 0.0f, 0.0f, 0.8f }, "[Fatal] %s" };
            default:
                return { };
        }
    }

    LogWidget::LogWidget()
    {
        Log::AddCallback([&](const auto level, const auto& text) 
        {
            this->messages.push_back({ text.data(), level });
        });
    }

    void LogWidget::Render(const char* const title)
    {   
        if (!open)
        {
            return;
        }     

        if (ImGui::Begin(title, &open))
        {
            if (ImGui::SmallButton("Clear"))           
            { 
                messages.clear();
            }
    
            ImGui::SameLine();

            auto enableTrace = mask & Log::Level::Trace;
            auto enableInfo = mask & Log::Level::Information;
            auto enableDebug = mask & Log::Level::Debug;
            auto enableWarn = mask & Log::Level::Warning;
            auto enableError = mask & Log::Level::Error;
            auto enableFatal = mask & Log::Level::Fatal;    

            if (ImGui::Checkbox("Trace", &enableTrace))
            {
                if (enableTrace)
                {
                    mask += Log::LevelMask::Trace;
                }
                else
                {
                    mask -= Log::LevelMask::Trace;
                }
            }
            
            ImGui::SameLine();

            if (ImGui::Checkbox("Info", &enableInfo))
            {
                if (enableInfo)
                {
                    mask += Log::LevelMask::Information;
                }
                else
                {
                    mask -= Log::LevelMask::Information;
                }
            }

            ImGui::SameLine();

            if (ImGui::Checkbox("Debug", &enableDebug))
            {
                if (enableDebug)
                {
                    mask += Log::LevelMask::Debug;
                }
                else
                {
                    mask -= Log::LevelMask::Debug;
                }
            }

            ImGui::SameLine();

            if (ImGui::Checkbox("Warn", &enableWarn))
            {
                if (enableWarn)
                {
                    mask += Log::LevelMask::Warning;
                }
                else
                {
                    mask -= Log::LevelMask::Warning;
                }
            }

            ImGui::SameLine();

            if (ImGui::Checkbox("Error", &enableError))
            {
                if (enableError)
                {
                    mask += Log::LevelMask::Error;
                }
                else
                {
                    mask -= Log::LevelMask::Error;
                }
            }

            ImGui::SameLine();

            if (ImGui::Checkbox("Fatal", &enableFatal))
            {
                if (enableFatal)
                {
                    mask += Log::LevelMask::Fatal;
                }
                else
                {
                    mask -= Log::LevelMask::Fatal;
                }
            }

            ImGui::Separator();

            const auto reserveHeight = (ImGui::GetStyle().ItemSpacing.y * 2) + ImGui::GetFrameHeightWithSpacing();

            ImGui::BeginChild("Scroll Area", ImVec2(0, -reserveHeight), false, ImGuiWindowFlags_HorizontalScrollbar);

            for (const auto& message : messages)
            {
                if (!(mask & message.level))
                {
                    continue;
                }

                const auto format = GetLogFormat(message.level);
                
                ImGui::TextColored(format.color, format.message, message.text.c_str());
            }
            
            ImGui::EndChild();

            ImGui::Separator();

            ImGui::BeginChild("Input Area");

            if (ImGui::InputText("Shell", input, sizeof(input), ImGuiInputTextFlags_EnterReturnsTrue))
            {
                const auto result = Shell::Execute(input);

                std::memset(input, '\0', sizeof(input));
            }

            ImGui::EndChild();
        }

        ImGui::End();
    }
}