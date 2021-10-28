#pragma one

#include <string_view>
#include <functional>

#include "PrimitiveType.h"
#include "Mode.h"
#include "PermissionMask.h"

namespace Quanta::Shell
{
    constexpr USize MaxArguments = 16u; 
    
    using Command = std::function<bool(void)>;

    void AddCommand(const std::string_view& signature, PrimitiveType returnType, const std::vector<PrimitiveType>& argTypes, const Command& callback);
    void RemoveCommand(const std::string_view& signature);

    void SetMode(Mode);
    Mode GetMode();

    void SetPermissionMask(PermissionMask);
    PermissionMask GetPermissionMask();

    const char* GetArgString(USize position);
    U32 GetArgInt(USize position);
    float GetArgFloat(USize position);
    bool GetArgBool(USize position);

    void SetReturnString(const char* value);
    void SetReturnInt(U32 value);
    void SetReturnFloat(float value);
    void SetReturnBool(bool value);

    const char* GetReturnString();
    U32 GetReturnInt();
    float GetReturnFloat();
    bool GetReturnBool();
    
    bool Execute(const std::string_view& command);
}