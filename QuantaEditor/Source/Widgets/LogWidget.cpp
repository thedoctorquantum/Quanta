#include <imgui.h>
#include <iostream>

#include "LogWidget.h"

namespace Quanta
{
    struct LogFormat final
    {
        ImVec4 color;
        const char* message = "%s\n";
    };
    
    static LogFormat GetLogFormat(Log::Level level)
    {
        LogFormat format;

        switch (level)
        {
            case Log::Level::Trace:
                format.message = "[Trace] %s";

                new(&format.color) ImVec4(1.0f, 1.0f, 1.0f, 0.8f);

                break;
            case Log::Level::Information:
                format.message = "[Information] %s";

                new(&format.color) ImVec4(0.0f, 1.0f, 0.0f, 0.8f);

                break;
            case Log::Level::Warning:
                format.message = "[Warning] %s";

                new(&format.color) ImVec4(1.0f, 1.0f, 0.0f, 0.8f);

                break;
            case Log::Level::Error:
                format.message = "[Error] %s";

                new(&format.color) ImVec4(1.0f, 0.0f, 0.0f, 0.8f);

                break;
        }

        return format;
    }

    LogWidget::LogWidget()
    {
        Log::AddCallback([&](Log::Level level, const std::string& text) 
        {
            this->messages.push_back({ text, level });
        });
    }

    LogWidget::~LogWidget()
    {

    }

    void LogWidget::Render(const char* title, bool* open)
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

            ImGui::NewLine();

            for (const auto& message : messages)
            {
                if (!(mask & message.level))
                {
                    continue;
                }

                LogFormat format = GetLogFormat(message.level);
                
                ImGui::TextColored(format.color, format.message, message.text.c_str());
            }
        }

        ImGui::End();
    }
}