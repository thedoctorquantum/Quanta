#pragma once

#include "lib.as"

#define HELLO "HELLO WORLD I LOVE YOU"

#define nameof(x) #x

#ifdef HELLO
#warning "ummm.. your code says hello"
#endif

funcdef int AddCallback(int, int);

int Main() 
{
    Std::String str = "" + nameof(Main);
    
    str += ", world! ";

    Std::LogInfo(str);

    SayHello();

    AddCallback@ add = function (int a, int b) { return a + b; };

    str = Std::ToString(add(5, 10));

    Std::LogInfo(str);
    
    return 0;
}