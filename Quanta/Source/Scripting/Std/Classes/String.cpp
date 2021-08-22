#include <assert.h> 
#include <sstream>  
#include <string.h> 
#include <stdio.h>
#include <stdlib.h> 
#include <unordered_map>
#include <map>     

#include "String.h"

namespace Quanta::As_Std
{
    void CString::Register(asIScriptEngine* engine)
    {

    }
}

namespace Quanta::As_Std::String
{
    class CStdStringFactory : public asIStringFactory
    {
    public:
        CStdStringFactory() {}
        ~CStdStringFactory() 
        {
            // The script engine must release each string 
            // constant that it has requested
            assert(stringCache.size() == 0);
        }

        const void *GetStringConstant(const char *data, asUINT length)
        {
            // The string factory might be modified from multiple 
            // threads, so it is necessary to use a mutex.
            asAcquireExclusiveLock();
            
            std::string str(data, length);
            std::unordered_map<std::string, int>::iterator it = stringCache.find(str);
            if (it != stringCache.end())
                it->second++;
            else
                it = stringCache.insert(std::unordered_map<std::string, int>::value_type(str, 1)).first;

            asReleaseExclusiveLock();
            
            return reinterpret_cast<const void*>(&it->first);
        }

        int  ReleaseStringConstant(const void *str)
        {
            if (str == 0)
                return asERROR;

            int ret = asSUCCESS;
            
            // The string factory might be modified from multiple 
            // threads, so it is necessary to use a mutex.
            asAcquireExclusiveLock();
            
            std::unordered_map<std::string, int>::iterator it = stringCache.find(*reinterpret_cast<const std::string*>(str));
            if (it == stringCache.end())
                ret = asERROR;
            else
            {
                it->second--;
                if (it->second == 0)
                    stringCache.erase(it);
            }
            
            asReleaseExclusiveLock();
            
            return ret;
        }

        int  GetRawStringData(const void *str, char *data, asUINT *length) const
        {
            if (str == 0)
                return asERROR;

            if (length)
                *length = (asUINT)reinterpret_cast<const std::string*>(str)->length();

            if (data)
                memcpy(data, reinterpret_cast<const std::string*>(str)->c_str(), reinterpret_cast<const std::string*>(str)->length());

            return asSUCCESS;
        }

        // THe access to the string cache is protected with the common mutex provided by AngelScript
        std::unordered_map<std::string, int> stringCache;
    };

    static CStdStringFactory *stringFactory = 0;

    // TODO: Make this public so the application can also use the string 
    //       factory and share the string constants if so desired, or to
    //       monitor the size of the string factory cache.
    CStdStringFactory *GetStdStringFactorySingleton()
    {
        if( stringFactory == 0 )
        {
            // The following instance will be destroyed by the global 
            // CStdStringFactoryCleaner instance upon application shutdown
            stringFactory = new CStdStringFactory();
        }
        return stringFactory;
    }

    class CStdStringFactoryCleaner
    {
    public:
        ~CStdStringFactoryCleaner()
        {
            if (stringFactory)
            {
                // Only delete the string factory if the stringCache is empty
                // If it is not empty, it means that someone might still attempt
                // to release string constants, so if we delete the string factory
                // the application might crash. Not deleting the cache would
                // lead to a memory leak, but since this is only happens when the
                // application is shutting down anyway, it is not important.
                if (stringFactory->stringCache.empty())
                {
                    delete stringFactory;
                    stringFactory = 0;
                }
            }
        }
    };

    static CStdStringFactoryCleaner cleaner;


    static void ConstructString(std::string *thisPointer)
    {
        new(thisPointer) std::string();
    }

    static void CopyConstructString(const std::string &other, std::string *thisPointer)
    {
        new(thisPointer) std::string(other);
    }

    static void DestructString(std::string *thisPointer)
    {
        thisPointer->~basic_string();
    }

    static std::string &AddAssignStringToString(const std::string &str, std::string &dest)
    {
        // We don't register the method directly because some compilers
        // and standard libraries inline the definition, resulting in the
        // linker being unable to find the declaration.
        // Example: CLang/LLVM with XCode 4.3 on OSX 10.7
        dest += str;
        return dest;
    }

    // bool string::isEmpty()
    // bool string::empty() // if AS_USE_STLNAMES == 1
    static bool StringIsEmpty(const std::string &str)
    {
        // We don't register the method directly because some compilers
        // and standard libraries inline the definition, resulting in the
        // linker being unable to find the declaration
        // Example: CLang/LLVM with XCode 4.3 on OSX 10.7
        return str.empty();
    }

    static std::string &AssignUInt64ToString(asQWORD i, std::string &dest)
    {
        std::ostringstream stream;
        stream << i;
        dest = stream.str();
        return dest;
    }

    static std::string &AddAssignUInt64ToString(asQWORD i, std::string &dest)
    {
        std::ostringstream stream;
        stream << i;
        dest += stream.str();
        return dest;
    }

    static std::string AddStringUInt64(const std::string &str, asQWORD i)
    {
        std::ostringstream stream;
        stream << i;
        return str + stream.str();
    }

    static std::string AddInt64String(asINT64 i, const std::string &str)
    {
        std::ostringstream stream;
        stream << i;
        return stream.str() + str;
    }

    static std::string &AssignInt64ToString(asINT64 i, std::string &dest)
    {
        std::ostringstream stream;
        stream << i;
        dest = stream.str();
        return dest;
    }

    static std::string &AddAssignInt64ToString(asINT64 i, std::string &dest)
    {
        std::ostringstream stream;
        stream << i;
        dest += stream.str();
        return dest;
    }

    static std::string AddStringInt64(const std::string &str, asINT64 i)
    {
        std::ostringstream stream;
        stream << i;
        return str + stream.str();
    }

    static std::string AddUInt64String(asQWORD i, const std::string &str)
    {
        std::ostringstream stream;
        stream << i;
        return stream.str() + str;
    }

    static std::string &AssignDoubleToString(double f, std::string &dest)
    {
        std::ostringstream stream;
        stream << f;
        dest = stream.str();
        return dest;
    }

    static std::string &AddAssignDoubleToString(double f, std::string &dest)
    {
        std::ostringstream stream;
        stream << f;
        dest += stream.str();
        return dest;
    }

    static std::string &AssignFloatToString(float f, std::string &dest)
    {
        std::ostringstream stream;
        stream << f;
        dest = stream.str();
        return dest;
    }

    static std::string &AddAssignFloatToString(float f, std::string &dest)
    {
        std::ostringstream stream;
        stream << f;
        dest += stream.str();
        return dest;
    }

    static std::string &AssignBoolToString(bool b, std::string &dest)
    {
        std::ostringstream stream;
        stream << (b ? "true" : "false");
        dest = stream.str();
        return dest;
    }

    static std::string &AddAssignBoolToString(bool b, std::string &dest)
    {
        std::ostringstream stream;
        stream << (b ? "true" : "false");
        dest += stream.str();
        return dest;
    }

    static std::string AddStringDouble(const std::string &str, double f)
    {
        std::ostringstream stream;
        stream << f;
        return str + stream.str();
    }

    static std::string AddDoubleString(double f, const std::string &str)
    {
        std::ostringstream stream;
        stream << f;
        return stream.str() + str;
    }

    static std::string AddStringFloat(const std::string &str, float f)
    {
        std::ostringstream stream;
        stream << f;
        return str + stream.str();
    }

    static std::string AddFloatString(float f, const std::string &str)
    {
        std::ostringstream stream;
        stream << f;
        return stream.str() + str;
    }

    static std::string AddStringBool(const std::string &str, bool b)
    {
        std::ostringstream stream;
        stream << (b ? "true" : "false");
        return str + stream.str();
    }

    static std::string AddBoolString(bool b, const std::string &str)
    {
        std::ostringstream stream;
        stream << (b ? "true" : "false");
        return stream.str() + str;
    }

    static char *StringCharAt(unsigned int i, std::string &str)
    {
        if( i >= str.size() )
        {
            // Set a script exception
            asIScriptContext *ctx = asGetActiveContext();
            ctx->SetException("Out of range");

            // Return a null pointer
            return 0;
        }

        return &str[i];
    }

    // AngelScript signature:
    // int string::opCmp(const string &in) const
    static int StringCmp(const std::string &a, const std::string &b)
    {
        int cmp = 0;
        if( a < b ) cmp = -1;
        else if( a > b ) cmp = 1;
        return cmp;
    }

    static std::string StringCat(const std::string& a, const std::string& b)
    {
        std::string result = a;

        result += b;

        return result;
    }

    // This function returns the index of the first position where the substring
    // exists in the input string. If the substring doesn't exist in the input
    // string -1 is returned.
    //
    // AngelScript signature:
    // int string::findFirst(const string &in sub, uint start = 0) const
    static int StringFindFirst(const std::string &sub, asUINT start, const std::string &str)
    {
        // We don't register the method directly because the argument types change between 32bit and 64bit platforms
        return (int)str.find(sub, (size_t)(start < 0 ? std::string::npos : start));
    }

    // This function returns the index of the first position where the one of the bytes in substring
    // exists in the input string. If the characters in the substring doesn't exist in the input
    // string -1 is returned.
    //
    // AngelScript signature:
    // int string::findFirstOf(const string &in sub, uint start = 0) const
    static int StringFindFirstOf(const std::string &sub, asUINT start, const std::string &str)
    {
        // We don't register the method directly because the argument types change between 32bit and 64bit platforms
        return (int)str.find_first_of(sub, (size_t)(start < 0 ? std::string::npos : start));
    }

    // This function returns the index of the last position where the one of the bytes in substring
    // exists in the input string. If the characters in the substring doesn't exist in the input
    // string -1 is returned.
    //
    // AngelScript signature:
    // int string::findLastOf(const string &in sub, uint start = -1) const
    static int StringFindLastOf(const std::string &sub, asUINT start, const std::string &str)
    {
        // We don't register the method directly because the argument types change between 32bit and 64bit platforms
        return (int)str.find_last_of(sub, (size_t)(start < 0 ? std::string::npos : start));
    }

    // This function returns the index of the first position where a byte other than those in substring
    // exists in the input string. If none is found -1 is returned.
    //
    // AngelScript signature:
    // int string::findFirstNotOf(const string &in sub, uint start = 0) const
    static int StringFindFirstNotOf(const std::string &sub, asUINT start, const std::string &str)
    {
        // We don't register the method directly because the argument types change between 32bit and 64bit platforms
        return (int)str.find_first_not_of(sub, (size_t)(start < 0 ? std::string::npos : start));
    }

    // This function returns the index of the last position where a byte other than those in substring
    // exists in the input string. If none is found -1 is returned.
    //
    // AngelScript signature:
    // int string::findLastNotOf(const string &in sub, uint start = -1) const
    static int StringFindLastNotOf(const std::string &sub, asUINT start, const std::string &str)
    {
        // We don't register the method directly because the argument types change between 32bit and 64bit platforms
        return (int)str.find_last_not_of(sub, (size_t)(start < 0 ? std::string::npos : start));
    }

    // This function returns the index of the last position where the substring
    // exists in the input string. If the substring doesn't exist in the input
    // string -1 is returned.
    //
    // AngelScript signature:
    // int string::findLast(const string &in sub, int start = -1) const
    static int StringFindLast(const std::string &sub, int start, const std::string &str)
    {
        // We don't register the method directly because the argument types change between 32bit and 64bit platforms
        return (int)str.rfind(sub, (size_t)(start < 0 ? std::string::npos : start));
    }

    // AngelScript signature:
    // void string::insert(uint pos, const string &in other)
    static void StringInsert(unsigned int pos, const std::string &other, std::string &str)
    {
        // We don't register the method directly because the argument types change between 32bit and 64bit platforms
        str.insert(pos, other);
    }

    // AngelScript signature:
    // void string::erase(uint pos, int count = -1)
    static void StringErase(unsigned int pos, int count, std::string &str)
    {
        // We don't register the method directly because the argument types change between 32bit and 64bit platforms
        str.erase(pos, (size_t)(count < 0 ? std::string::npos : count));
    }


    // AngelScript signature:
    // uint string::length() const
    static asUINT StringLength(const std::string &str)
    {
        // We don't register the method directly because the return type changes between 32bit and 64bit platforms
        return (asUINT)str.length();
    }


    // AngelScript signature:
    // void string::resize(uint l)
    static void StringResize(asUINT l, std::string &str)
    {
        // We don't register the method directly because the argument types change between 32bit and 64bit platforms
        str.resize(l);
    }

    // AngelScript signature:
    // string formatInt(int64 val, const string &in options, uint width)
    static std::string formatInt(asINT64 value, const std::string &options, asUINT width)
    {
        bool leftJustify = options.find("l") != std::string::npos;
        bool padWithZero = options.find("0") != std::string::npos;
        bool alwaysSign  = options.find("+") != std::string::npos;
        bool spaceOnSign = options.find(" ") != std::string::npos;
        bool hexSmall    = options.find("h") != std::string::npos;
        bool hexLarge    = options.find("H") != std::string::npos;

        std::string fmt = "%";
        if( leftJustify ) fmt += "-";
        if( alwaysSign ) fmt += "+";
        if( spaceOnSign ) fmt += " ";
        if( padWithZero ) fmt += "0";

    #ifdef _WIN32
        fmt += "*I64";
    #else
    #ifdef _LP64
        fmt += "*l";
    #else
        fmt += "*ll";
    #endif
    #endif

        if( hexSmall ) fmt += "x";
        else if( hexLarge ) fmt += "X";
        else fmt += "d";

        std::string buf;
        buf.resize(width+30);
    #if _MSC_VER >= 1400 && !defined(__S3E__)
        // MSVC 8.0 / 2005 or newer
        sprintf_s(&buf[0], buf.size(), fmt.c_str(), width, value);
    #else
        sprintf(&buf[0], fmt.c_str(), width, value);
    #endif
        buf.resize(strlen(&buf[0]));

        return buf;
    }

    // AngelScript signature:
    // string formatUInt(uint64 val, const string &in options, uint width)
    static std::string formatUInt(asQWORD value, const std::string &options, asUINT width)
    {
        bool leftJustify = options.find("l") != std::string::npos;
        bool padWithZero = options.find("0") != std::string::npos;
        bool alwaysSign  = options.find("+") != std::string::npos;
        bool spaceOnSign = options.find(" ") != std::string::npos;
        bool hexSmall    = options.find("h") != std::string::npos;
        bool hexLarge    = options.find("H") != std::string::npos;

        std::string fmt = "%";
        if( leftJustify ) fmt += "-";
        if( alwaysSign ) fmt += "+";
        if( spaceOnSign ) fmt += " ";
        if( padWithZero ) fmt += "0";

    #ifdef _WIN32
        fmt += "*I64";
    #else
    #ifdef _LP64
        fmt += "*l";
    #else
        fmt += "*ll";
    #endif
    #endif

        if( hexSmall ) fmt += "x";
        else if( hexLarge ) fmt += "X";
        else fmt += "u";

        std::string buf;
        buf.resize(width+30);
    #if _MSC_VER >= 1400 && !defined(__S3E__)
        // MSVC 8.0 / 2005 or newer
        sprintf_s(&buf[0], buf.size(), fmt.c_str(), width, value);
    #else
        sprintf(&buf[0], fmt.c_str(), width, value);
    #endif
        buf.resize(strlen(&buf[0]));

        return buf;
    }

    // AngelScript signature:
    // string formatFloat(double val, const string &in options, uint width, uint precision)
    static std::string formatFloat(double value, const std::string &options, asUINT width, asUINT precision)
    {
        bool leftJustify = options.find("l") != std::string::npos;
        bool padWithZero = options.find("0") != std::string::npos;
        bool alwaysSign  = options.find("+") != std::string::npos;
        bool spaceOnSign = options.find(" ") != std::string::npos;
        bool expSmall    = options.find("e") != std::string::npos;
        bool expLarge    = options.find("E") != std::string::npos;

        std::string fmt = "%";
        if( leftJustify ) fmt += "-";
        if( alwaysSign ) fmt += "+";
        if( spaceOnSign ) fmt += " ";
        if( padWithZero ) fmt += "0";

        fmt += "*.*";

        if( expSmall ) fmt += "e";
        else if( expLarge ) fmt += "E";
        else fmt += "f";

        std::string buf;
        buf.resize(width+precision+50);
    #if _MSC_VER >= 1400 && !defined(__S3E__)
        // MSVC 8.0 / 2005 or newer
        sprintf_s(&buf[0], buf.size(), fmt.c_str(), width, precision, value);
    #else
        sprintf(&buf[0], fmt.c_str(), width, precision, value);
    #endif
        buf.resize(strlen(&buf[0]));

        return buf;
    }

    // AngelScript signature:
    // int64 parseInt(const string &in val, uint base = 10, uint &out byteCount = 0)
    static asINT64 parseInt(const std::string &val, asUINT base, asUINT *byteCount)
    {
        // Only accept base 10 and 16
        if( base != 10 && base != 16 )
        {
            if( byteCount ) *byteCount = 0;
            return 0;
        }

        const char *end = &val[0];

        // Determine the sign
        bool sign = false;
        if( *end == '-' )
        {
            sign = true;
            end++;
        }
        else if( *end == '+' )
            end++;

        asINT64 res = 0;
        if( base == 10 )
        {
            while( *end >= '0' && *end <= '9' )
            {
                res *= 10;
                res += *end++ - '0';
            }
        }
        else if( base == 16 )
        {
            while( (*end >= '0' && *end <= '9') ||
                (*end >= 'a' && *end <= 'f') ||
                (*end >= 'A' && *end <= 'F') )
            {
                res *= 16;
                if( *end >= '0' && *end <= '9' )
                    res += *end++ - '0';
                else if( *end >= 'a' && *end <= 'f' )
                    res += *end++ - 'a' + 10;
                else if( *end >= 'A' && *end <= 'F' )
                    res += *end++ - 'A' + 10;
            }
        }

        if( byteCount )
            *byteCount = asUINT(size_t(end - val.c_str()));

        if( sign )
            res = -res;

        return res;
    }

    // AngelScript signature:
    // uint64 parseUInt(const string &in val, uint base = 10, uint &out byteCount = 0)
    static asQWORD parseUInt(const std::string &val, asUINT base, asUINT *byteCount)
    {
        // Only accept base 10 and 16
        if (base != 10 && base != 16)
        {
            if (byteCount) *byteCount = 0;
            return 0;
        }

        const char *end = &val[0];

        asQWORD res = 0;
        if (base == 10)
        {
            while (*end >= '0' && *end <= '9')
            {
                res *= 10;
                res += *end++ - '0';
            }
        }
        else if (base == 16)
        {
            while ((*end >= '0' && *end <= '9') ||
                (*end >= 'a' && *end <= 'f') ||
                (*end >= 'A' && *end <= 'F'))
            {
                res *= 16;
                if (*end >= '0' && *end <= '9')
                    res += *end++ - '0';
                else if (*end >= 'a' && *end <= 'f')
                    res += *end++ - 'a' + 10;
                else if (*end >= 'A' && *end <= 'F')
                    res += *end++ - 'A' + 10;
            }
        }

        if (byteCount)
            *byteCount = asUINT(size_t(end - val.c_str()));

        return res;
    }

    // AngelScript signature:
    // double parseFloat(const string &in val, uint &out byteCount = 0)
    double parseFloat(const std::string &val, asUINT *byteCount)
    {
        char *end;

        // WinCE doesn't have setlocale. Some quick testing on my current platform
        // still manages to parse the numbers such as "3.14" even if the decimal for the
        // locale is ",".
    #if !defined(_WIN32_WCE) && !defined(ANDROID) && !defined(__psp2__)
        // Set the locale to C so that we are guaranteed to parse the float value correctly
        char *tmp = setlocale(LC_NUMERIC, 0);
        std::string orig = tmp ? tmp : "C";
        setlocale(LC_NUMERIC, "C");
    #endif

        double res = strtod(val.c_str(), &end);

    #if !defined(_WIN32_WCE) && !defined(ANDROID) && !defined(__psp2__)
        // Restore the locale
        setlocale(LC_NUMERIC, orig.c_str());
    #endif

        if( byteCount )
            *byteCount = asUINT(size_t(end - val.c_str()));

        return res;
    }

    // This function returns a string containing the substring of the input string
    // determined by the starting index and count of characters.
    //
    // AngelScript signature:
    // string string::substr(uint start = 0, int count = -1) const
    static std::string StringSubString(asUINT start, int count, const std::string &str)
    {
        // Check for out-of-bounds
        std::string ret;
        if( start < str.length() && count != 0 )
            ret = str.substr(start, (size_t)(count < 0 ? std::string::npos : count));

        return ret;
    }

    // String equality comparison.
    // Returns true iff lhs is equal to rhs.
    //
    // For some reason gcc 4.7 has difficulties resolving the
    // asFUNCTIONPR(operator==, (const string &, const string &)
    // makro, so this wrapper was introduced as work around.
    static bool StringEquals(const std::string& lhs, const std::string& rhs)
    {
        return lhs == rhs;
    }

    void RegisterStdString_Native(asIScriptEngine *engine)
    {
        int r = 0;

        // Register the string type
    #if AS_CAN_USE_CPP11
        // With C++11 it is possible to use asGetTypeTraits to automatically determine the correct flags to use
        r = engine->RegisterObjectType("String", sizeof(std::string), asOBJ_VALUE | asGetTypeTraits<std::string>()); assert( r >= 0 );
    #else
        r = engine->RegisterObjectType("String", sizeof(string), asOBJ_VALUE | asOBJ_APP_CLASS_CDAK); assert( r >= 0 );
    #endif

        r = engine->RegisterStringFactory("String", GetStdStringFactorySingleton());

        // Register the object operator overloads
        r = engine->RegisterObjectBehaviour("String", asBEHAVE_CONSTRUCT,  "void f()",                    asFUNCTION(ConstructString), asCALL_CDECL_OBJLAST); assert( r >= 0 );
        r = engine->RegisterObjectBehaviour("String", asBEHAVE_CONSTRUCT,  "void f(const String &in)",    asFUNCTION(CopyConstructString), asCALL_CDECL_OBJLAST); assert( r >= 0 );
        r = engine->RegisterObjectBehaviour("String", asBEHAVE_DESTRUCT,   "void f()",                    asFUNCTION(DestructString),  asCALL_CDECL_OBJLAST); assert( r >= 0 );
        r = engine->RegisterObjectMethod("String", "String &opAssign(const String &in)", asMETHODPR(std::string, operator =, (const std::string&), std::string&), asCALL_THISCALL); assert( r >= 0 );
        // Need to use a wrapper on Mac OS X 10.7/XCode 4.3 and CLang/LLVM, otherwise the linker fails
        r = engine->RegisterObjectMethod("String", "String &opAddAssign(const String &in)", asFUNCTION(AddAssignStringToString), asCALL_CDECL_OBJLAST); assert( r >= 0 );
    //	r = engine->RegisterObjectMethod("string", "string &opAddAssign(const string &in)", asMETHODPR(string, operator+=, (const string&), string&), asCALL_THISCALL); assert( r >= 0 );

        // Need to use a wrapper for operator== otherwise gcc 4.7 fails to compile
        r = engine->RegisterObjectMethod("String", "bool opEquals(const String &in) const", asFUNCTIONPR(StringEquals, (const std::string &, const std::string &), bool), asCALL_CDECL_OBJFIRST); assert( r >= 0 );
        r = engine->RegisterObjectMethod("String", "int opCmp(const String &in) const", asFUNCTION(StringCmp), asCALL_CDECL_OBJFIRST); assert( r >= 0 );
        r = engine->RegisterObjectMethod("String", "String opAdd(const String &in) const", asFUNCTIONPR(StringCat, (const std::string &, const std::string &), std::string), asCALL_CDECL_OBJFIRST); assert( r >= 0 );

        // The string length can be accessed through methods or through virtual property
        // TODO: Register as size() for consistency with other types
    #if AS_USE_ACCESSORS != 1
        r = engine->RegisterObjectMethod("String", "uint length() const", asFUNCTION(StringLength), asCALL_CDECL_OBJLAST); assert( r >= 0 );
    #endif
        r = engine->RegisterObjectMethod("String", "void resize(uint)", asFUNCTION(StringResize), asCALL_CDECL_OBJLAST); assert( r >= 0 );
    #if AS_USE_STLNAMES != 1 && AS_USE_ACCESSORS == 1
        // Don't register these if STL names is used, as they conflict with the method size()
        r = engine->RegisterObjectMethod("String", "uint get_length() const property", asFUNCTION(StringLength), asCALL_CDECL_OBJLAST); assert( r >= 0 );
        r = engine->RegisterObjectMethod("String", "void set_length(uint) property", asFUNCTION(StringResize), asCALL_CDECL_OBJLAST); assert( r >= 0 );
    #endif
        // Need to use a wrapper on Mac OS X 10.7/XCode 4.3 and CLang/LLVM, otherwise the linker fails
    //	r = engine->RegisterObjectMethod("string", "bool isEmpty() const", asMETHOD(string, empty), asCALL_THISCALL); assert( r >= 0 );
        r = engine->RegisterObjectMethod("String", "bool isEmpty() const", asFUNCTION(StringIsEmpty), asCALL_CDECL_OBJLAST); assert( r >= 0 );

        // Register the index operator, both as a mutator and as an inspector
        // Note that we don't register the operator[] directly, as it doesn't do bounds checking
        r = engine->RegisterObjectMethod("String", "uint8 &opIndex(uint)", asFUNCTION(StringCharAt), asCALL_CDECL_OBJLAST); assert( r >= 0 );
        r = engine->RegisterObjectMethod("String", "const uint8 &opIndex(uint) const", asFUNCTION(StringCharAt), asCALL_CDECL_OBJLAST); assert( r >= 0 );

        // Automatic conversion from values
        r = engine->RegisterObjectMethod("String", "String &opAssign(double)", asFUNCTION(AssignDoubleToString), asCALL_CDECL_OBJLAST); assert( r >= 0 );
        r = engine->RegisterObjectMethod("String", "String &opAddAssign(double)", asFUNCTION(AddAssignDoubleToString), asCALL_CDECL_OBJLAST); assert( r >= 0 );
        r = engine->RegisterObjectMethod("String", "String opAdd(double) const", asFUNCTION(AddStringDouble), asCALL_CDECL_OBJFIRST); assert( r >= 0 );
        r = engine->RegisterObjectMethod("String", "String opAdd_r(double) const", asFUNCTION(AddDoubleString), asCALL_CDECL_OBJLAST); assert( r >= 0 );

        r = engine->RegisterObjectMethod("String", "String &opAssign(float)", asFUNCTION(AssignFloatToString), asCALL_CDECL_OBJLAST); assert( r >= 0 );
        r = engine->RegisterObjectMethod("String", "String &opAddAssign(float)", asFUNCTION(AddAssignFloatToString), asCALL_CDECL_OBJLAST); assert( r >= 0 );
        r = engine->RegisterObjectMethod("String", "String opAdd(float) const", asFUNCTION(AddStringFloat), asCALL_CDECL_OBJFIRST); assert( r >= 0 );
        r = engine->RegisterObjectMethod("String", "String opAdd_r(float) const", asFUNCTION(AddFloatString), asCALL_CDECL_OBJLAST); assert( r >= 0 );

        r = engine->RegisterObjectMethod("String", "String &opAssign(int64)", asFUNCTION(AssignInt64ToString), asCALL_CDECL_OBJLAST); assert( r >= 0 );
        r = engine->RegisterObjectMethod("String", "String &opAddAssign(int64)", asFUNCTION(AddAssignInt64ToString), asCALL_CDECL_OBJLAST); assert( r >= 0 );
        r = engine->RegisterObjectMethod("String", "String opAdd(int64) const", asFUNCTION(AddStringInt64), asCALL_CDECL_OBJFIRST); assert( r >= 0 );
        r = engine->RegisterObjectMethod("String", "String opAdd_r(int64) const", asFUNCTION(AddInt64String), asCALL_CDECL_OBJLAST); assert( r >= 0 );

        r = engine->RegisterObjectMethod("String", "String &opAssign(uint64)", asFUNCTION(AssignUInt64ToString), asCALL_CDECL_OBJLAST); assert( r >= 0 );
        r = engine->RegisterObjectMethod("String", "String &opAddAssign(uint64)", asFUNCTION(AddAssignUInt64ToString), asCALL_CDECL_OBJLAST); assert( r >= 0 );
        r = engine->RegisterObjectMethod("String", "String opAdd(uint64) const", asFUNCTION(AddStringUInt64), asCALL_CDECL_OBJFIRST); assert( r >= 0 );
        r = engine->RegisterObjectMethod("String", "String opAdd_r(uint64) const", asFUNCTION(AddUInt64String), asCALL_CDECL_OBJLAST); assert( r >= 0 );

        r = engine->RegisterObjectMethod("String", "String &opAssign(bool)", asFUNCTION(AssignBoolToString), asCALL_CDECL_OBJLAST); assert( r >= 0 );
        r = engine->RegisterObjectMethod("String", "String &opAddAssign(bool)", asFUNCTION(AddAssignBoolToString), asCALL_CDECL_OBJLAST); assert( r >= 0 );
        r = engine->RegisterObjectMethod("String", "String opAdd(bool) const", asFUNCTION(AddStringBool), asCALL_CDECL_OBJFIRST); assert( r >= 0 );
        r = engine->RegisterObjectMethod("String", "String opAdd_r(bool) const", asFUNCTION(AddBoolString), asCALL_CDECL_OBJLAST); assert( r >= 0 );

        // Utilities
        r = engine->RegisterObjectMethod("String", "String substr(uint start = 0, int count = -1) const", asFUNCTION(StringSubString), asCALL_CDECL_OBJLAST); assert( r >= 0 );
        r = engine->RegisterObjectMethod("String", "int findFirst(const String &in, uint start = 0) const", asFUNCTION(StringFindFirst), asCALL_CDECL_OBJLAST); assert( r >= 0 );
        r = engine->RegisterObjectMethod("String", "int findFirstOf(const String &in, uint start = 0) const", asFUNCTION(StringFindFirstOf), asCALL_CDECL_OBJLAST); assert(r >= 0);
        r = engine->RegisterObjectMethod("String", "int findFirstNotOf(const String &in, uint start = 0) const", asFUNCTION(StringFindFirstNotOf), asCALL_CDECL_OBJLAST); assert(r >= 0);
        r = engine->RegisterObjectMethod("String", "int findLast(const String &in, int start = -1) const", asFUNCTION(StringFindLast), asCALL_CDECL_OBJLAST); assert( r >= 0 );
        r = engine->RegisterObjectMethod("String", "int findLastOf(const String &in, int start = -1) const", asFUNCTION(StringFindLastOf), asCALL_CDECL_OBJLAST); assert(r >= 0);
        r = engine->RegisterObjectMethod("String", "int findLastNotOf(const String &in, int start = -1) const", asFUNCTION(StringFindLastNotOf), asCALL_CDECL_OBJLAST); assert(r >= 0);
        r = engine->RegisterObjectMethod("String", "void insert(uint pos, const String &in other)", asFUNCTION(StringInsert), asCALL_CDECL_OBJLAST); assert(r >= 0);
        r = engine->RegisterObjectMethod("String", "void erase(uint pos, int count = -1)", asFUNCTION(StringErase), asCALL_CDECL_OBJLAST); assert(r >= 0);


        r = engine->RegisterGlobalFunction("String formatInt(int64 val, const String &in options = \"\", uint width = 0)", asFUNCTION(formatInt), asCALL_CDECL); assert(r >= 0);
        r = engine->RegisterGlobalFunction("String formatUInt(uint64 val, const String &in options = \"\", uint width = 0)", asFUNCTION(formatUInt), asCALL_CDECL); assert(r >= 0);
        r = engine->RegisterGlobalFunction("String formatFloat(double val, const String &in options = \"\", uint width = 0, uint precision = 0)", asFUNCTION(formatFloat), asCALL_CDECL); assert(r >= 0);
        r = engine->RegisterGlobalFunction("int64 parseInt(const String &in, uint base = 10, uint &out byteCount = 0)", asFUNCTION(parseInt), asCALL_CDECL); assert(r >= 0);
        r = engine->RegisterGlobalFunction("uint64 parseUInt(const String &in, uint base = 10, uint &out byteCount = 0)", asFUNCTION(parseUInt), asCALL_CDECL); assert(r >= 0);
        r = engine->RegisterGlobalFunction("double parseFloat(const String &in, uint &out byteCount = 0)", asFUNCTION(parseFloat), asCALL_CDECL); assert(r >= 0);

    #if AS_USE_STLNAMES == 1
        // Same as length
        r = engine->RegisterObjectMethod("string", "uint size() const", asFUNCTION(StringLength), asCALL_CDECL_OBJLAST); assert( r >= 0 );
        // Same as isEmpty
        r = engine->RegisterObjectMethod("string", "bool empty() const", asFUNCTION(StringIsEmpty), asCALL_CDECL_OBJLAST); assert( r >= 0 );
        // Same as findFirst
        r = engine->RegisterObjectMethod("string", "int find(const string &in, uint start = 0) const", asFUNCTION(StringFindFirst), asCALL_CDECL_OBJLAST); assert( r >= 0 );
        // Same as findLast
        r = engine->RegisterObjectMethod("string", "int rfind(const string &in, int start = -1) const", asFUNCTION(StringFindLast), asCALL_CDECL_OBJLAST); assert( r >= 0 );
    #endif

        // TODO: Implement the following
        // findAndReplace - replaces a text found in the string
        // replaceRange - replaces a range of bytes in the string
        // multiply/times/opMul/opMul_r - takes the string and multiplies it n times, e.g. "-".multiply(5) returns "-----"
    }

    static void ConstructStringGeneric(asIScriptGeneric * gen)
    {
        new (gen->GetObject()) std::string();
    }

    static void CopyConstructStringGeneric(asIScriptGeneric * gen)
    {
        std::string * a = static_cast<std::string *>(gen->GetArgObject(0));
        new (gen->GetObject()) std::string(*a);
    }

    static void DestructStringGeneric(asIScriptGeneric * gen)
    {
        std::string * ptr = static_cast<std::string *>(gen->GetObject());
        ptr->~basic_string();
    }

    static void AssignStringGeneric(asIScriptGeneric *gen)
    {
        std::string * a = static_cast<std::string*>(gen->GetArgObject(0));
        std::string * self = static_cast<std::string*>(gen->GetObject());
        *self = *a;
        gen->SetReturnAddress(self);
    }

    static void AddAssignStringGeneric(asIScriptGeneric *gen)
    {
        std::string * a = static_cast<std::string *>(gen->GetArgObject(0));
        std::string * self = static_cast<std::string *>(gen->GetObject());
        *self += *a;
        gen->SetReturnAddress(self);
    }

    static void StringEqualsGeneric(asIScriptGeneric * gen)
    {
        std::string * a = static_cast<std::string *>(gen->GetObject());
        std::string * b = static_cast<std::string *>(gen->GetArgAddress(0));
        *(bool*)gen->GetAddressOfReturnLocation() = (*a == *b);
    }

    static void StringCmpGeneric(asIScriptGeneric * gen)
    {
        std::string * a = static_cast<std::string *>(gen->GetObject());
        std::string * b = static_cast<std::string *>(gen->GetArgAddress(0));

        int cmp = 0;
        if( *a < *b ) cmp = -1;
        else if( *a > *b ) cmp = 1;

        *(int*)gen->GetAddressOfReturnLocation() = cmp;
    }

    static void StringAddGeneric(asIScriptGeneric * gen)
    {
        std::string * a = static_cast<std::string *>(gen->GetObject());
        std::string * b = static_cast<std::string *>(gen->GetArgAddress(0));
        std::string ret_val = *a + *b;
        gen->SetReturnObject(&ret_val);
    }

    static void StringLengthGeneric(asIScriptGeneric * gen)
    {
        std::string * self = static_cast<std::string *>(gen->GetObject());
        *static_cast<asUINT *>(gen->GetAddressOfReturnLocation()) = (asUINT)self->length();
    }

    static void StringIsEmptyGeneric(asIScriptGeneric * gen)
    {
        std::string * self = reinterpret_cast<std::string *>(gen->GetObject());
        *reinterpret_cast<bool *>(gen->GetAddressOfReturnLocation()) = StringIsEmpty(*self);
    }

    static void StringResizeGeneric(asIScriptGeneric * gen)
    {
        std::string * self = static_cast<std::string *>(gen->GetObject());
        self->resize(*static_cast<asUINT *>(gen->GetAddressOfArg(0)));
    }

    static void StringInsert_Generic(asIScriptGeneric *gen)
    {
        std::string * self = static_cast<std::string *>(gen->GetObject());
        asUINT pos = gen->GetArgDWord(0);
        std::string *other = reinterpret_cast<std::string*>(gen->GetArgAddress(1));
        StringInsert(pos, *other, *self);
    }

    static void StringErase_Generic(asIScriptGeneric *gen)
    {
        std::string * self = static_cast<std::string *>(gen->GetObject());
        asUINT pos = gen->GetArgDWord(0);
        int count = int(gen->GetArgDWord(1));
        StringErase(pos, count, *self);
    }

    static void StringFindFirst_Generic(asIScriptGeneric * gen)
    {
        std::string *find = reinterpret_cast<std::string*>(gen->GetArgAddress(0));
        asUINT start = gen->GetArgDWord(1);
        std::string *self = reinterpret_cast<std::string *>(gen->GetObject());
        *reinterpret_cast<int *>(gen->GetAddressOfReturnLocation()) = StringFindFirst(*find, start, *self);
    }

    static void StringFindLast_Generic(asIScriptGeneric * gen)
    {
        std::string *find = reinterpret_cast<std::string*>(gen->GetArgAddress(0));
        asUINT start = gen->GetArgDWord(1);
        std::string *self = reinterpret_cast<std::string *>(gen->GetObject());
        *reinterpret_cast<int *>(gen->GetAddressOfReturnLocation()) = StringFindLast(*find, start, *self);
    }

    static void StringFindFirstOf_Generic(asIScriptGeneric * gen)
    {
        std::string *find = reinterpret_cast<std::string*>(gen->GetArgAddress(0));
        asUINT start = gen->GetArgDWord(1);
        std::string *self = reinterpret_cast<std::string *>(gen->GetObject());
        *reinterpret_cast<int *>(gen->GetAddressOfReturnLocation()) = StringFindFirstOf(*find, start, *self);
    }

    static void StringFindLastOf_Generic(asIScriptGeneric * gen)
    {
        std::string *find = reinterpret_cast<std::string*>(gen->GetArgAddress(0));
        asUINT start = gen->GetArgDWord(1);
        std::string *self = reinterpret_cast<std::string *>(gen->GetObject());
        *reinterpret_cast<int *>(gen->GetAddressOfReturnLocation()) = StringFindLastOf(*find, start, *self);
    }

    static void StringFindFirstNotOf_Generic(asIScriptGeneric * gen)
    {
        std::string *find = reinterpret_cast<std::string*>(gen->GetArgAddress(0));
        asUINT start = gen->GetArgDWord(1);
        std::string *self = reinterpret_cast<std::string *>(gen->GetObject());
        *reinterpret_cast<int *>(gen->GetAddressOfReturnLocation()) = StringFindFirstNotOf(*find, start, *self);
    }

    static void StringFindLastNotOf_Generic(asIScriptGeneric * gen)
    {
        std::string *find = reinterpret_cast<std::string*>(gen->GetArgAddress(0));
        asUINT start = gen->GetArgDWord(1);
        std::string *self = reinterpret_cast<std::string *>(gen->GetObject());
        *reinterpret_cast<int *>(gen->GetAddressOfReturnLocation()) = StringFindLastNotOf(*find, start, *self);
    }

    static void formatInt_Generic(asIScriptGeneric * gen)
    {
        asINT64 val = gen->GetArgQWord(0);
        std::string *options = reinterpret_cast<std::string*>(gen->GetArgAddress(1));
        asUINT width = gen->GetArgDWord(2);
        new(gen->GetAddressOfReturnLocation()) std::string(formatInt(val, *options, width));
    }

    static void formatUInt_Generic(asIScriptGeneric * gen)
    {
        asQWORD val = gen->GetArgQWord(0);
        std::string *options = reinterpret_cast<std::string*>(gen->GetArgAddress(1));
        asUINT width = gen->GetArgDWord(2);
        new(gen->GetAddressOfReturnLocation()) std::string(formatUInt(val, *options, width));
    }

    static void formatFloat_Generic(asIScriptGeneric *gen)
    {
        double val = gen->GetArgDouble(0);
        std::string *options = reinterpret_cast<std::string*>(gen->GetArgAddress(1));
        asUINT width = gen->GetArgDWord(2);
        asUINT precision = gen->GetArgDWord(3);
        new(gen->GetAddressOfReturnLocation()) std::string(formatFloat(val, *options, width, precision));
    }

    static void parseInt_Generic(asIScriptGeneric *gen)
    {
        std::string *str = reinterpret_cast<std::string*>(gen->GetArgAddress(0));
        asUINT base = gen->GetArgDWord(1);
        asUINT *byteCount = reinterpret_cast<asUINT*>(gen->GetArgAddress(2));
        gen->SetReturnQWord(parseInt(*str,base,byteCount));
    }

    static void parseUInt_Generic(asIScriptGeneric *gen)
    {
        std::string *str = reinterpret_cast<std::string*>(gen->GetArgAddress(0));
        asUINT base = gen->GetArgDWord(1);
        asUINT *byteCount = reinterpret_cast<asUINT*>(gen->GetArgAddress(2));
        gen->SetReturnQWord(parseUInt(*str, base, byteCount));
    }

    static void parseFloat_Generic(asIScriptGeneric *gen)
    {
        std::string *str = reinterpret_cast<std::string*>(gen->GetArgAddress(0));
        asUINT *byteCount = reinterpret_cast<asUINT*>(gen->GetArgAddress(1));
        gen->SetReturnDouble(parseFloat(*str,byteCount));
    }

    static void StringCharAtGeneric(asIScriptGeneric * gen)
    {
        unsigned int index = gen->GetArgDWord(0);
        std::string * self = static_cast<std::string *>(gen->GetObject());

        if (index >= self->size())
        {
            // Set a script exception
            asIScriptContext *ctx = asGetActiveContext();
            ctx->SetException("Out of range");

            gen->SetReturnAddress(0);
        }
        else
        {
            gen->SetReturnAddress(&(self->operator [](index)));
        }
    }

    static void AssignInt2StringGeneric(asIScriptGeneric *gen)
    {
        asINT64 *a = static_cast<asINT64*>(gen->GetAddressOfArg(0));
        std::string *self = static_cast<std::string*>(gen->GetObject());
        std::stringstream sstr;
        sstr << *a;
        *self = sstr.str();
        gen->SetReturnAddress(self);
    }

    static void AssignUInt2StringGeneric(asIScriptGeneric *gen)
    {
        asQWORD *a = static_cast<asQWORD*>(gen->GetAddressOfArg(0));
        std::string *self = static_cast<std::string*>(gen->GetObject());
        std::stringstream sstr;
        sstr << *a;
        *self = sstr.str();
        gen->SetReturnAddress(self);
    }

    static void AssignDouble2StringGeneric(asIScriptGeneric *gen)
    {
        double *a = static_cast<double*>(gen->GetAddressOfArg(0));
        std::string *self = static_cast<std::string*>(gen->GetObject());
        std::stringstream sstr;
        sstr << *a;
        *self = sstr.str();
        gen->SetReturnAddress(self);
    }

    static void AssignFloat2StringGeneric(asIScriptGeneric *gen)
    {
        float *a = static_cast<float*>(gen->GetAddressOfArg(0));
        std::string *self = static_cast<std::string*>(gen->GetObject());
        std::stringstream sstr;
        sstr << *a;
        *self = sstr.str();
        gen->SetReturnAddress(self);
    }

    static void AssignBool2StringGeneric(asIScriptGeneric *gen)
    {
        bool *a = static_cast<bool*>(gen->GetAddressOfArg(0));
        std::string *self = static_cast<std::string*>(gen->GetObject());
        std::stringstream sstr;
        sstr << (*a ? "true" : "false");
        *self = sstr.str();
        gen->SetReturnAddress(self);
    }

    static void AddAssignDouble2StringGeneric(asIScriptGeneric * gen)
    {
        double * a = static_cast<double *>(gen->GetAddressOfArg(0));
        std::string * self = static_cast<std::string *>(gen->GetObject());
        std::stringstream sstr;
        sstr << *a;
        *self += sstr.str();
        gen->SetReturnAddress(self);
    }

    static void AddAssignFloat2StringGeneric(asIScriptGeneric * gen)
    {
        float * a = static_cast<float *>(gen->GetAddressOfArg(0));
        std::string * self = static_cast<std::string *>(gen->GetObject());
        std::stringstream sstr;
        sstr << *a;
        *self += sstr.str();
        gen->SetReturnAddress(self);
    }

    static void AddAssignInt2StringGeneric(asIScriptGeneric * gen)
    {
        asINT64 * a = static_cast<asINT64 *>(gen->GetAddressOfArg(0));
        std::string * self = static_cast<std::string *>(gen->GetObject());
        std::stringstream sstr;
        sstr << *a;
        *self += sstr.str();
        gen->SetReturnAddress(self);
    }

    static void AddAssignUInt2StringGeneric(asIScriptGeneric * gen)
    {
        asQWORD * a = static_cast<asQWORD *>(gen->GetAddressOfArg(0));
        std::string * self = static_cast<std::string *>(gen->GetObject());
        std::stringstream sstr;
        sstr << *a;
        *self += sstr.str();
        gen->SetReturnAddress(self);
    }

    static void AddAssignBool2StringGeneric(asIScriptGeneric * gen)
    {
        bool * a = static_cast<bool *>(gen->GetAddressOfArg(0));
        std::string * self = static_cast<std::string *>(gen->GetObject());
        std::stringstream sstr;
        sstr << (*a ? "true" : "false");
        *self += sstr.str();
        gen->SetReturnAddress(self);
    }

    static void AddString2DoubleGeneric(asIScriptGeneric * gen)
    {
        std::string * a = static_cast<std::string *>(gen->GetObject());
        double * b = static_cast<double *>(gen->GetAddressOfArg(0));
        std::stringstream sstr;
        sstr << *a << *b;
        std::string ret_val = sstr.str();
        gen->SetReturnObject(&ret_val);
    }

    static void AddString2FloatGeneric(asIScriptGeneric * gen)
    {
        std::string * a = static_cast<std::string *>(gen->GetObject());
        float * b = static_cast<float *>(gen->GetAddressOfArg(0));
        std::stringstream sstr;
        sstr << *a << *b;
        std::string ret_val = sstr.str();
        gen->SetReturnObject(&ret_val);
    }

    static void AddString2IntGeneric(asIScriptGeneric * gen)
    {
        std::string * a = static_cast<std::string *>(gen->GetObject());
        asINT64 * b = static_cast<asINT64 *>(gen->GetAddressOfArg(0));
        std::stringstream sstr;
        sstr << *a << *b;
        std::string ret_val = sstr.str();
        gen->SetReturnObject(&ret_val);
    }

    static void AddString2UIntGeneric(asIScriptGeneric * gen)
    {
        std::string * a = static_cast<std::string *>(gen->GetObject());
        asQWORD * b = static_cast<asQWORD *>(gen->GetAddressOfArg(0));
        std::stringstream sstr;
        sstr << *a << *b;
        std::string ret_val = sstr.str();
        gen->SetReturnObject(&ret_val);
    }

    static void AddString2BoolGeneric(asIScriptGeneric * gen)
    {
        std::string * a = static_cast<std::string *>(gen->GetObject());
        bool * b = static_cast<bool *>(gen->GetAddressOfArg(0));
        std::stringstream sstr;
        sstr << *a << (*b ? "true" : "false");
        std::string ret_val = sstr.str();
        gen->SetReturnObject(&ret_val);
    }

    static void AddDouble2StringGeneric(asIScriptGeneric * gen)
    {
        double* a = static_cast<double *>(gen->GetAddressOfArg(0));
        std::string * b = static_cast<std::string *>(gen->GetObject());
        std::stringstream sstr;
        sstr << *a << *b;
        std::string ret_val = sstr.str();
        gen->SetReturnObject(&ret_val);
    }

    static void AddFloat2StringGeneric(asIScriptGeneric * gen)
    {
        float* a = static_cast<float *>(gen->GetAddressOfArg(0));
        std::string * b = static_cast<std::string *>(gen->GetObject());
        std::stringstream sstr;
        sstr << *a << *b;
        std::string ret_val = sstr.str();
        gen->SetReturnObject(&ret_val);
    }

    static void AddInt2StringGeneric(asIScriptGeneric * gen)
    {
        asINT64* a = static_cast<asINT64 *>(gen->GetAddressOfArg(0));
        std::string * b = static_cast<std::string *>(gen->GetObject());
        std::stringstream sstr;
        sstr << *a << *b;
        std::string ret_val = sstr.str();
        gen->SetReturnObject(&ret_val);
    }

    static void AddUInt2StringGeneric(asIScriptGeneric * gen)
    {
        asQWORD* a = static_cast<asQWORD *>(gen->GetAddressOfArg(0));
        std::string * b = static_cast<std::string *>(gen->GetObject());
        std::stringstream sstr;
        sstr << *a << *b;
        std::string ret_val = sstr.str();
        gen->SetReturnObject(&ret_val);
    }

    static void AddBool2StringGeneric(asIScriptGeneric * gen)
    {
        bool* a = static_cast<bool *>(gen->GetAddressOfArg(0));
        std::string * b = static_cast<std::string *>(gen->GetObject());
        std::stringstream sstr;
        sstr << (*a ? "true" : "false") << *b;
        std::string ret_val = sstr.str();
        gen->SetReturnObject(&ret_val);
    }

    static void StringSubString_Generic(asIScriptGeneric *gen)
    {
        // Get the arguments
        std::string *str   = (std::string*)gen->GetObject();
        asUINT  start = *(int*)gen->GetAddressOfArg(0);
        int     count = *(int*)gen->GetAddressOfArg(1);

        // Return the substring
        new(gen->GetAddressOfReturnLocation()) std::string(StringSubString(start, count, *str));
    }

    void RegisterStdString_Generic(asIScriptEngine *engine)
    {
        int r = 0;

        // Register the string type
        r = engine->RegisterObjectType("String", sizeof(std::string), asOBJ_VALUE | asOBJ_APP_CLASS_CDAK); assert( r >= 0 );

        r = engine->RegisterStringFactory("String", GetStdStringFactorySingleton());

        // Register the object operator overloads
        r = engine->RegisterObjectBehaviour("String", asBEHAVE_CONSTRUCT,  "void f()",                    asFUNCTION(ConstructStringGeneric), asCALL_GENERIC); assert( r >= 0 );
        r = engine->RegisterObjectBehaviour("String", asBEHAVE_CONSTRUCT,  "void f(const String &in)",    asFUNCTION(CopyConstructStringGeneric), asCALL_GENERIC); assert( r >= 0 );
        r = engine->RegisterObjectBehaviour("String", asBEHAVE_DESTRUCT,   "void f()",                    asFUNCTION(DestructStringGeneric),  asCALL_GENERIC); assert( r >= 0 );
        r = engine->RegisterObjectMethod("String", "String &opAssign(const String &in)", asFUNCTION(AssignStringGeneric),    asCALL_GENERIC); assert( r >= 0 );
        r = engine->RegisterObjectMethod("String", "String &opAddAssign(const String &in)", asFUNCTION(AddAssignStringGeneric), asCALL_GENERIC); assert( r >= 0 );

        r = engine->RegisterObjectMethod("String", "bool opEquals(const String &in) const", asFUNCTION(StringEqualsGeneric), asCALL_GENERIC); assert( r >= 0 );
        r = engine->RegisterObjectMethod("String", "int opCmp(const String &in) const", asFUNCTION(StringCmpGeneric), asCALL_GENERIC); assert( r >= 0 );
        r = engine->RegisterObjectMethod("String", "String opAdd(const String &in) const", asFUNCTION(StringAddGeneric), asCALL_GENERIC); assert( r >= 0 );

        // Register the object methods
    #if AS_USE_ACCESSORS != 1
        r = engine->RegisterObjectMethod("String", "uint length() const", asFUNCTION(StringLengthGeneric), asCALL_GENERIC); assert( r >= 0 );
    #endif
        r = engine->RegisterObjectMethod("String", "void resize(uint)",   asFUNCTION(StringResizeGeneric), asCALL_GENERIC); assert( r >= 0 );
    #if AS_USE_STLNAMES != 1 && AS_USE_ACCESSORS == 1
        r = engine->RegisterObjectMethod("string", "uint get_length() const property", asFUNCTION(StringLengthGeneric), asCALL_GENERIC); assert( r >= 0 );
        r = engine->RegisterObjectMethod("string", "void set_length(uint) property", asFUNCTION(StringResizeGeneric), asCALL_GENERIC); assert( r >= 0 );
    #endif
        r = engine->RegisterObjectMethod("String", "bool isEmpty() const", asFUNCTION(StringIsEmptyGeneric), asCALL_GENERIC); assert( r >= 0 );

        // Register the index operator, both as a mutator and as an inspector
        r = engine->RegisterObjectMethod("String", "uint8 &opIndex(uint)", asFUNCTION(StringCharAtGeneric), asCALL_GENERIC); assert( r >= 0 );
        r = engine->RegisterObjectMethod("String", "const uint8 &opIndex(uint) const", asFUNCTION(StringCharAtGeneric), asCALL_GENERIC); assert( r >= 0 );

        // Automatic conversion from values
        r = engine->RegisterObjectMethod("String", "String &opAssign(double)", asFUNCTION(AssignDouble2StringGeneric), asCALL_GENERIC); assert( r >= 0 );
        r = engine->RegisterObjectMethod("String", "String &opAddAssign(double)", asFUNCTION(AddAssignDouble2StringGeneric), asCALL_GENERIC); assert( r >= 0 );
        r = engine->RegisterObjectMethod("String", "String opAdd(double) const", asFUNCTION(AddString2DoubleGeneric), asCALL_GENERIC); assert( r >= 0 );
        r = engine->RegisterObjectMethod("String", "String opAdd_r(double) const", asFUNCTION(AddDouble2StringGeneric), asCALL_GENERIC); assert( r >= 0 );

        r = engine->RegisterObjectMethod("String", "String &opAssign(float)", asFUNCTION(AssignFloat2StringGeneric), asCALL_GENERIC); assert( r >= 0 );
        r = engine->RegisterObjectMethod("String", "String &opAddAssign(float)", asFUNCTION(AddAssignFloat2StringGeneric), asCALL_GENERIC); assert( r >= 0 );
        r = engine->RegisterObjectMethod("String", "String opAdd(float) const", asFUNCTION(AddString2FloatGeneric), asCALL_GENERIC); assert( r >= 0 );
        r = engine->RegisterObjectMethod("String", "String opAdd_r(float) const", asFUNCTION(AddFloat2StringGeneric), asCALL_GENERIC); assert( r >= 0 );

        r = engine->RegisterObjectMethod("String", "String &opAssign(int64)", asFUNCTION(AssignInt2StringGeneric), asCALL_GENERIC); assert( r >= 0 );
        r = engine->RegisterObjectMethod("String", "String &opAddAssign(int64)", asFUNCTION(AddAssignInt2StringGeneric), asCALL_GENERIC); assert( r >= 0 );
        r = engine->RegisterObjectMethod("String", "String opAdd(int64) const", asFUNCTION(AddString2IntGeneric), asCALL_GENERIC); assert( r >= 0 );
        r = engine->RegisterObjectMethod("String", "String opAdd_r(int64) const", asFUNCTION(AddInt2StringGeneric), asCALL_GENERIC); assert( r >= 0 );

        r = engine->RegisterObjectMethod("String", "String &opAssign(uint64)", asFUNCTION(AssignUInt2StringGeneric), asCALL_GENERIC); assert( r >= 0 );
        r = engine->RegisterObjectMethod("String", "String &opAddAssign(uint64)", asFUNCTION(AddAssignUInt2StringGeneric), asCALL_GENERIC); assert( r >= 0 );
        r = engine->RegisterObjectMethod("String", "String opAdd(uint64) const", asFUNCTION(AddString2UIntGeneric), asCALL_GENERIC); assert( r >= 0 );
        r = engine->RegisterObjectMethod("String", "String opAdd_r(uint64) const", asFUNCTION(AddUInt2StringGeneric), asCALL_GENERIC); assert( r >= 0 );

        r = engine->RegisterObjectMethod("String", "String &opAssign(bool)", asFUNCTION(AssignBool2StringGeneric), asCALL_GENERIC); assert( r >= 0 );
        r = engine->RegisterObjectMethod("String", "String &opAddAssign(bool)", asFUNCTION(AddAssignBool2StringGeneric), asCALL_GENERIC); assert( r >= 0 );
        r = engine->RegisterObjectMethod("String", "String opAdd(bool) const", asFUNCTION(AddString2BoolGeneric), asCALL_GENERIC); assert( r >= 0 );
        r = engine->RegisterObjectMethod("String", "String opAdd_r(bool) const", asFUNCTION(AddBool2StringGeneric), asCALL_GENERIC); assert( r >= 0 );

        r = engine->RegisterObjectMethod("String", "String substr(uint start = 0, int count = -1) const", asFUNCTION(StringSubString_Generic), asCALL_GENERIC); assert(r >= 0);
        r = engine->RegisterObjectMethod("String", "int findFirst(const String &in, uint start = 0) const", asFUNCTION(StringFindFirst_Generic), asCALL_GENERIC); assert(r >= 0);
        r = engine->RegisterObjectMethod("String", "int findFirstOf(const String &in, uint start = 0) const", asFUNCTION(StringFindFirstOf_Generic), asCALL_GENERIC); assert(r >= 0);
        r = engine->RegisterObjectMethod("String", "int findFirstNotOf(const String &in, uint start = 0) const", asFUNCTION(StringFindFirstNotOf_Generic), asCALL_GENERIC); assert(r >= 0);
        r = engine->RegisterObjectMethod("String", "int findLast(const String &in, int start = -1) const", asFUNCTION(StringFindLast_Generic), asCALL_GENERIC); assert(r >= 0);
        r = engine->RegisterObjectMethod("String", "int findLastOf(const String &in, int start = -1) const", asFUNCTION(StringFindLastOf_Generic), asCALL_GENERIC); assert(r >= 0);
        r = engine->RegisterObjectMethod("String", "int findLastNotOf(const String &in, int start = -1) const", asFUNCTION(StringFindLastNotOf_Generic), asCALL_GENERIC); assert(r >= 0);
        r = engine->RegisterObjectMethod("String", "void insert(uint pos, const String &in other)", asFUNCTION(StringInsert_Generic), asCALL_GENERIC); assert(r >= 0);
        r = engine->RegisterObjectMethod("String", "void erase(uint pos, int count = -1)", asFUNCTION(StringErase_Generic), asCALL_GENERIC); assert(r >= 0);


        r = engine->RegisterGlobalFunction("String formatInt(int64 val, const String &in options = \"\", uint width = 0)", asFUNCTION(formatInt_Generic), asCALL_GENERIC); assert(r >= 0);
        r = engine->RegisterGlobalFunction("String formatUInt(uint64 val, const String &in options = \"\", uint width = 0)", asFUNCTION(formatUInt_Generic), asCALL_GENERIC); assert(r >= 0);
        r = engine->RegisterGlobalFunction("String formatFloat(double val, const String &in options = \"\", uint width = 0, uint precision = 0)", asFUNCTION(formatFloat_Generic), asCALL_GENERIC); assert(r >= 0);
        r = engine->RegisterGlobalFunction("int64 parseInt(const String &in, uint base = 10, uint &out byteCount = 0)", asFUNCTION(parseInt_Generic), asCALL_GENERIC); assert(r >= 0);
        r = engine->RegisterGlobalFunction("uint64 parseUInt(const String &in, uint base = 10, uint &out byteCount = 0)", asFUNCTION(parseUInt_Generic), asCALL_GENERIC); assert(r >= 0);
        r = engine->RegisterGlobalFunction("double parseFloat(const String &in, uint &out byteCount = 0)", asFUNCTION(parseFloat_Generic), asCALL_GENERIC); assert(r >= 0);
    }

    void Register(asIScriptEngine* engine)
    {
        if (strstr(asGetLibraryOptions(), "AS_MAX_PORTABILITY"))
        {
            RegisterStdString_Generic(engine);
        }
        else 
        {
            RegisterStdString_Native(engine);
        }
    }
}