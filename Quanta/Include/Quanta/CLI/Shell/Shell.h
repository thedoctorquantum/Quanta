#pragma one

#include <string>
#include <string_view>
#include <functional>

#include "PrimitiveType.h"
#include "Mode.h"
#include "PermissionMask.h"

namespace Quanta::Shell
{
    constexpr USize MaxArguments = 16u; 
    
    using Command = std::function<bool(void)>;

    void AddCommand(const std::string& signature, PrimitiveType returnType, const std::vector<PrimitiveType>& argTypes, const Command& callback);
    void RemoveCommand(const std::string& signature);

    void SetMode(Mode);
    Mode GetMode();

    void SetPermissionMask(PermissionMask);
    PermissionMask GetPermissionMask();

    const char* GetArgString(USize position);
    U32 GetArgInt(USize position);
    float GetArgFloat(USize position);

    void SetReturnString(const char* value);
    void SetReturnInt(U32 value);
    void SetReturnFloat(float value);

    const char* GetReturnString();
    U32 GetReturnInt();
    float GetReturnFloat();

    bool Execute(std::string command);
}