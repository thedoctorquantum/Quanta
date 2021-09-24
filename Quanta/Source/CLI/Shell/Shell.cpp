#include <Quanta/CLI/Shell/Shell.h>
#include <Quanta/CLI/Log/Log.h>
#include <cstring>
#include <cstdio>
#include <sstream>
#include <algorithm>
#include <unordered_map>

#include "../../Debugging/Validation.h"

namespace Quanta::Shell
{
    static void RemoveExcessWhitespace(const std::string& input, std::string& output)
    {
        output.clear();

        std::unique_copy(input.begin(), input.end(), std::back_insert_iterator<std::string>(output),
            [](const char a, const char b)
            {
                return isspace(a) && isspace(b);
            }
        ); 
    }

    static const char* PrimitiveTypeToString(const PrimitiveType type)
    {
        switch (type)
        {
            case PrimitiveType::String:
                return "String";
            case PrimitiveType::Int:
                return "Int";
            case PrimitiveType::Float:
                return "Float";
        }

        return nullptr;
    } 

    struct Value final
    {
        PrimitiveType type = PrimitiveType::String;

        union
        {
            const char* stringValue;
            U32 intValue;
            float floatValue;
        };
    };

    struct CommandData final
    {
        Command command;
        PrimitiveType returnType;
        std::vector<PrimitiveType> argTypes;
    };

    struct 
    {
        Mode mode = Mode::Loud;
        PermissionMask permissionMask = PermissionMask::All;

        std::unordered_map<std::string, CommandData> commands;

        std::string stringArgs[MaxArguments];

        Value args[MaxArguments];
        Value returnValue;
    } static state;

    void AddCommand(const std::string& signature, const PrimitiveType returnType, const std::vector<PrimitiveType>& argTypes, const Command& callback)
    {
        if (state.commands.find(signature) != state.commands.end())
        {
            return;
        }

        state.commands.emplace(signature, CommandData { callback, returnType, argTypes });
    }

    void RemoveCommand(const std::string& signature)
    {
        auto command = state.commands.end();

        if ((command = state.commands.find(signature)) != state.commands.end())
        {
            state.commands.erase(command);
        }
    }

    void SetMode(Mode value)
    {
        state.mode = value;
    }

    Mode GetMode()
    {
        return state.mode;
    }

    void SetPermissionMask(const PermissionMask value)
    {
        state.permissionMask = value;
    }

    PermissionMask GetPermissionMask()
    {   
        return state.permissionMask;
    }       

    const char* GetArgString(const USize position)
    {
        DEBUG_ASSERT(position < MaxArguments);
        DEBUG_ASSERT(state.args[position].type == PrimitiveType::String);

        return state.args[position].stringValue;
    }

    U32 GetArgInt(const USize position)
    {   
        DEBUG_ASSERT(position < MaxArguments);
        DEBUG_ASSERT(state.args[position].type == PrimitiveType::Int);

        return state.args[position].intValue;
    }

    float GetArgFloat(const USize position)
    {
        DEBUG_ASSERT(position < MaxArguments);
        DEBUG_ASSERT(state.args[position].type == PrimitiveType::Float);

        return state.args[position].floatValue;
    }
    
    void SetReturnString(const char* const value)
    {
        state.returnValue.type = PrimitiveType::String;
        state.returnValue.stringValue = value;
    }

    void SetReturnInt(const U32 value)
    {
        state.returnValue.type = PrimitiveType::Int;
        state.returnValue.intValue = value;
    }

    void SetReturnFloat(const float value)
    {
        state.returnValue.type = PrimitiveType::Float;
        state.returnValue.floatValue = value;
    }

    const char* GetReturnString()
    {
        DEBUG_ASSERT(state.returnValue.type == PrimitiveType::String);

        return state.returnValue.stringValue;
    }

    U32 GetReturnInt()
    {
        DEBUG_ASSERT(state.returnValue.type == PrimitiveType::Int);

        return state.returnValue.intValue;
    }

    float GetReturnFloat()
    {
        DEBUG_ASSERT(state.returnValue.type == PrimitiveType::Float);

        return state.returnValue.floatValue;
    }

    static bool ExecuteSingleCommand(const std::string& command)
    {
        std::istringstream input(command);

        std::string commandName;

        std::getline(input, commandName, ':');

        commandName.erase(remove(commandName.begin(), commandName.end(), ' '), commandName.end());

        std::string token;

        std::size_t i = 0;

        while (std::getline(input, token, ','))
        {
            state.stringArgs[i] = token;

            i++;
        }

        const auto argCount = i;

        const auto commandIterator = state.commands.find(commandName);

        if (commandIterator == state.commands.end())
        {
            Log::Write(Log::Level::Error, "Command '" + commandName + "' does not exist!");

            return false;
        }

        const auto& commandData = *commandIterator;

        if (argCount != commandData.second.argTypes.size() || argCount > MaxArguments)
        {
            std::ostringstream message;

            message << "Syntax Error! Expected: ";
            message << commandData.first;
            message << ": ";

            for (auto iter = commandData.second.argTypes.begin(); iter != commandData.second.argTypes.end(); iter++)
            {
                const auto asString = PrimitiveTypeToString(*iter);

                if (iter == commandData.second.argTypes.begin())
                {
                    message << asString;

                    continue;
                }

                message << ", " << asString; 
            }

            Log::Write(Log::Level::Error, message.str());

            return false;
        }

        for (std::size_t i = 0; i < commandData.second.argTypes.size(); i++)
        {
            const auto type = commandData.second.argTypes[i];
            const auto string = state.stringArgs[i].c_str();
            auto& value = state.args[i];

            value.type = type;

            switch (type)
            {
                case PrimitiveType::String:
                    value.stringValue = string;

                    break;
                case PrimitiveType::Int:
                    value.intValue = std::atoi(string);

                    break;
                case PrimitiveType::Float:
                    value.floatValue = std::atof(string);

                    break;
            }
        }

        const auto exit = commandData.second.command();

        Log::Write(Log::Level::Debug, "[Command] " + commandData.first);

        if (commandData.second.returnType != PrimitiveType::Void)
        {
            std::ostringstream message;

            message << "Command returned: ";

            switch (commandData.second.returnType)
            {
                case PrimitiveType::String:
                    message << state.returnValue.stringValue;

                    break;
                case PrimitiveType::Int:
                    message << state.returnValue.intValue;

                    break;
                case PrimitiveType::Float:
                    message << state.returnValue.floatValue;

                    break;
            }

            Log::Write(Log::Level::Debug, message.str());
        }

        return exit;
    }

    bool Execute(std::string commandString)
    {   
        std::replace(commandString.begin(), commandString.end(), '\n', ' ');

        std::string whitespaceRemoved;

        RemoveExcessWhitespace(commandString, whitespaceRemoved);

        commandString = std::move(whitespaceRemoved);

        std::istringstream commandStream(commandString);

        std::string command;

        auto exitCode = true; 

        while (std::getline(commandStream, command, ';'))
        {
            exitCode &= ExecuteSingleCommand(command);
        }

        return exitCode;
    }
}