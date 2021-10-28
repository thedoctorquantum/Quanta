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
            case PrimitiveType::Bool:
                return "Bool";
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
            bool boolValue;
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

    void AddCommand(const std::string_view& signature, const PrimitiveType returnType, const std::vector<PrimitiveType>& argTypes, const Command& callback)
    {
        state.commands.emplace(signature, CommandData { callback, returnType, argTypes });
    }
    
    void RemoveCommand(const std::string_view& signature)
    {
        auto command = state.commands.end();

        state.commands.erase(command);
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
    
    bool GetArgBool(const USize position)
    {
        DEBUG_ASSERT(position < MaxArguments);
        DEBUG_ASSERT(state.args[position].type == PrimitiveType::Bool);

        return state.args[position].boolValue;
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

    void SetReturnBool(const bool value)
    {
        state.returnValue.type = PrimitiveType::Bool;
        state.returnValue.boolValue = value;
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

    bool GetReturnBool()
    {
        DEBUG_ASSERT(state.returnValue.type == PrimitiveType::Bool);

        return state.returnValue.boolValue;
    }   

    struct Token 
    {   
        enum struct Type : std::uint8_t
        {
            Command,
            String,
            Float,
            Int,
            Bool,
            Colon,
            Semicolon,
            Comma
        };

        Type type = Type::Bool;

        std::string_view lexeme;
    };

    bool Execute(const std::string_view& string)
    {
        std::vector<Token> tokens;

        Token token;

        for (std::size_t i = 0; i < string.size(); i++)
        {
            const auto character = string[i];

            switch (character)
            {
                case 'A':
                case 'B':
                case 'C':
                case 'D':
                case 'E':
                case 'F':
                case 'G':
                case 'H':
                case 'I':
                case 'J':
                case 'K':
                case 'L':
                case 'M':
                case 'N':
                case 'O':
                case 'P':
                case 'Q':
                case 'R':
                case 'S':
                case 'T':
                case 'U':
                case 'V':
                case 'W':
                case 'X':
                case 'Y':
                case 'Z':
                case 'a':
                case 'b':
                case 'c':
                case 'd':
                case 'e':
                case 'f':
                case 'g':
                case 'h':
                case 'i':
                case 'j':
                case 'k':
                case 'l':
                case 'm':
                case 'n':
                case 'o':
                case 'p':
                case 'q':
                case 'r':
                case 's':
                case 't':
                case 'u':
                case 'v':
                case 'w':
                case 'x':
                case 'y':
                case 'z':
                case '_':
                    {
                        const auto begin = i;

                        for (;; i++)
                        {
                            const auto character = string[i];

                            if (isalpha(character) || character == '_')
                            {
                                continue;
                            }

                            token.lexeme = string.substr(begin, i - begin);

                            if (token.lexeme == "true" || token.lexeme == "false")
                            {
                                token.type = Token::Type::Bool;
                            }
                            else
                            {
                                token.type = Token::Type::Command;
                            }

                            tokens.push_back(token);

                            i--;

                            break;
                        }
                    }

                    break;
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                case '-':
                    {
                        const auto begin = i;

                        for (;; i++)
                        {
                            const auto character = string[i];

                            if (isalnum(character) || character == '-')
                            {
                                continue;
                            }

                            token.lexeme = string.substr(begin, i - begin);

                            token.type = Token::Type::Int;

                            tokens.push_back(token);

                            i--;

                            break;
                        }
                    }

                    break;
                case ':':
                    token.type = Token::Type::Colon;
                    token.lexeme = string.substr(i, 1);                    

                    tokens.push_back(token);

                    break;
                case ';':
                    token.type = Token::Type::Semicolon;
                    token.lexeme = string.substr(i, 1);

                    tokens.push_back(token);

                    break;
                case ',':
                    token.type = Token::Type::Comma;
                    token.lexeme = string.substr(i, 1);

                    tokens.push_back(token);

                    break;
                default:
                    break;
            }
        }

        if (tokens.empty())
        {
            Log::Write(Log::Level::Error, "Empty input string");

            return false;
        }

        for (const auto& token : tokens)
        {
            Log::Write(Log::Level::Debug, std::string("Tok: ") + std::string(token.lexeme));
        }

        auto iter = tokens.begin();

        if (iter->type != Token::Type::Command)
        {
            Log::Write(Log::Level::Error, "Expected token 'Command'");

            return false;
        }

        std::string commandName(iter->lexeme);

        const auto commandIter = state.commands.find(commandName);

        if (commandIter == state.commands.end())
        {
            Log::Write(Log::Level::Error, "Could not find command " + commandName);

            return false;
        }

        const auto& command = commandIter->second;

        switch ((iter + 1)->type)
        {
            case Token::Type::Colon:
                {
                    iter += 2;

                    std::size_t currentArg = 0;

                    for (;; iter++)
                    {
                        Log::Write(Log::Level::Debug, "arg!");

                        switch (iter->type)
                        {
                            case Token::Type::Int:

                                if (command.argTypes[currentArg] == PrimitiveType::Int)
                                {
                                    Log::Write(Log::Level::Debug, "Found int!");

                                    state.args[currentArg].intValue = atoi(iter->lexeme.data());
                                    state.args[currentArg].type = PrimitiveType::Int;
                                }
                                else
                                {
                                    Log::Write(Log::Level::Error, "Type mismatch!");
                                }

                                currentArg++;

                                break;
                            case Token::Type::Float:
                                if (command.argTypes[currentArg] == PrimitiveType::Float)
                                {
                                    state.args[currentArg].floatValue = 0.0f;
                                    state.args[currentArg].type = PrimitiveType::Float;
                                }
                                else
                                {
                                    Log::Write(Log::Level::Error, "Type mismatch!");
                                }

                                currentArg++;

                                break;
                            case Token::Type::Bool:
                                Log::Write(Log::Level::Debug, "Found bool!");

                                if (command.argTypes[currentArg] == PrimitiveType::Bool)
                                {
                                    state.args[currentArg].boolValue = iter->lexeme == "true" ? true : false;
                                    state.args[currentArg].type = PrimitiveType::Bool;
                                }
                                else
                                {
                                    Log::Write(Log::Level::Error, "Type mismatch!");
                                }

                                currentArg++;

                                break;
                            case Token::Type::String:
                                break;
                            default:
                                break;
                        }

                        if (iter->type == Token::Type::Semicolon)
                        {
                            break;
                        }

                        if (iter->type != Token::Type::Comma)
                        {
                            Log::Write(Log::Level::Error, "Expected token ','");

                            break;
                        }
                    }
                }

                break;
            case Token::Type::Semicolon:
                break;
            default:
                Log::Write(Log::Level::Error, "Expected token ':' or ';'");

                break;
        }

        command.command();

        if (state.returnValue.type != PrimitiveType::Void)
        {
            std::ostringstream output;

            switch (state.returnValue.type)
            {
                case PrimitiveType::Int:
                    output << state.returnValue.intValue;

                    break;
                case PrimitiveType::Float:
                    output << state.returnValue.floatValue;

                    break;
                case PrimitiveType::String:
                    output << state.returnValue.stringValue;

                    break;
                case PrimitiveType::Bool:
                    output << (state.returnValue.boolValue ? "true" : "false");

                    break;
            }

            Log::Write(Log::Level::Debug, "Command returned: " + output.str());
        }

        if (iter->type != Token::Type::Semicolon)
        {
            Log::Write(Log::Level::Error, "Expected token ';'");

            return false;
        }

        return true;
    }
}