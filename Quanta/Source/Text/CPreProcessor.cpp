#include <Quanta/Text/CPreProcessor.h>
#include <Quanta/Logging/Log.h>
#include <sstream>
#include <fstream>
#include <iostream>

#include "simplecpp/simplecpp.h"
#include "../Utilites/Symbols.h"

namespace Quanta
{
    static void RemoveDirectives(std::string& string)
    {
        std::istringstream inputStream(string);
        std::ostringstream outputStream; 

        std::string line;

        while (std::getline(inputStream, line))
        {
            for (char chr : line)
            {
                if (chr == '#')
                {
                    line = "";

                    break;
                }
            }
            
            outputStream << line << '\n';
        }

        string = outputStream.str();
    }

    CPreProcessor::CPreProcessor(const std::string& filepath) : filepath(filepath)
    {

    }

    CPreProcessor::~CPreProcessor()
    {

    }

    void CPreProcessor::Define(const std::string& declaration)
    {
        defines.push_back(declaration);
    }   

    std::string CPreProcessor::Process()
    {
        simplecpp::DUI dui;

        for (const auto& define : defines)
        {
            dui.defines.push_back(define);
        }

        simplecpp::OutputList outputList;

        std::vector<std::string> files;
        
        std::fstream file(filepath);

        simplecpp::TokenList rawTokens(file, files, filepath, &outputList);

        rawTokens.removeComments();

        std::map<std::string, simplecpp::TokenList*> included = simplecpp::load(rawTokens, files, dui, &outputList);
        
        for (std::pair<std::string, simplecpp::TokenList*> i : included)
        {
            i.second->removeComments();
        }
        
        simplecpp::TokenList outputTokens(files);
        
        simplecpp::preprocess(outputTokens, rawTokens, files, included, dui, &outputList);
        
        for (const simplecpp::Output &output : outputList) 
        {
            Log::Level level = Log::Level::Information;

            switch (output.type)
            {
                case simplecpp::Output::WARNING:
                    level = Log::Level::Warning;

                    break;
                case simplecpp::Output::INCLUDE_NESTED_TOO_DEEPLY:
                case simplecpp::Output::MISSING_HEADER:
                case simplecpp::Output::SYNTAX_ERROR:
                case simplecpp::Output::UNHANDLED_CHAR_ERROR:
                case simplecpp::Output::ERROR:
                case simplecpp::Output::EXPLICIT_INCLUDE_NOT_FOUND:
                    level = Log::Level::Error;

                    break;
            }

            std::ostringstream message;

            message << '[';
            message << nameof(CPreProcessor);
            message << "]: ";
            message << output.location.file();
            message << ": ";
            message << output.msg;

            Log::Write(level, message.str());
        }

        std::string output = outputTokens.stringify();

        simplecpp::cleanup(included);
        
        RemoveDirectives(output);

        return output;
    }
}