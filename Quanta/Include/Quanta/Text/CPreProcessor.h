#pragma once

#include <string>
#include <vector>

namespace Quanta
{
    class CPreProcessor final
    {
    public:
        CPreProcessor(const std::string& filepath);
        ~CPreProcessor();

        void Define(const std::string& declaration);

        std::string Process();
    private:
        std::string filepath;

        std::vector<std::string> defines;
    };
}