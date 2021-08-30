#include <imgui.h>

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
        Log::AddCallback([&](const Log::Level level, const std::string& text) 
        {
            this->messages.push_back({ text, level });
        });
    }

    void LogWidget::Render(const char* const title, bool* const open)
    {   
        if (!*open)
        {
            return;
        }     

        if (ImGui::Begin(title, open))
        {
            if (ImGui::SmallButton("Clear"))           
            { 
                messages.clear();
            }
    
            ImGui::SameLine();

            bool enableTrace = mask & Log::Level::Trace;
            bool enableInfo = mask & Log::Level::Information;
            bool enableWarn = mask & Log::Level::Warning;
            bool enableError = mask & Log::Level::Error;
            bool enableFatal = mask & Log::Level::Fatal;    

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

            if (ImGui::BeginChild("Scroll Area", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar))
            {
                for (const Message& message : messages)
                {
                    if (!(mask & message.level))
                    {
                        continue;
                    }

                    LogFormat format = GetLogFormat(message.level);
                    
                    ImGui::TextColored(format.color, format.message, message.text.c_str());
                }
            }
            
            ImGui::EndChild();
        }

        ImGui::End();
    }
}