
int32 Main() 
{
    std::vec2f32 vec = { 50, 50 };

    vec += { 68583.0f, 2 };

    vec *= 100;

    vec[0] = 69.420f;
    
    try 
    {
        if (vec[0] > 2)
        {
            std::throw("Oh no x is greater than two!!!!");
        }
    }
    catch
    {
        std::String e = std::get_exception();

        std::print(e);
    }   

    std::String str = "Hello, world! vec: " + (vec + { 5, 5 });

    std::String str2 = str;

    str[3] = 'a';

    std::print(str);
    std::print(str2);

    std::String@ str3 = str2;

    std::print(str3);

    std::char chr = str[3];

    std::print(++chr);
    std::print('\n');

    return 0;
}