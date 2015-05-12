#include <iostream>
#include "url.h"

bool urlSchemeTest();

int main()
{
    std::cout << "START TESTS" << std::endl;

    if(urlSchemeTest())
    {
        std::cout << "urlSchemeTest OK" << std::endl;
    }
    else
    {
        std::cout << "urlSchemeTest FAILED" << std::endl;
    }

    std::cout << "FINISH TESTS" << std::endl;
    return 0;
}

bool urlSchemeTest()
{
    URL url("https://");
    if(url.scheme() != "https")
        return false;

    if(url.host() != "")
        return false;

    if(url.path() != "")
        return false;

    if(url.query() != "")
        return false;

    if(url.fragment() != "")
        return false;

    return true;
}
