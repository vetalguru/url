#include <iostream>
#include "url.h"

/**

URL list
Empty URL
https://
http://[2a00:11d8:1201:0:962b:18:e716:fb97]
http://[2a00:11d8:1201:0:962b:18:e716:fb97]:8080
http://user@[2a00:11d8:1201:0:962b:18:e716:fb97]:8080
http://user:password@[2a00:11d8:1201:0:962b:18:e716:fb97]:8080

*/

bool urlEmptyTest();
bool urlSchemeTest();
bool urlIPv6Test();
bool urlIPv6PortTest();
bool urlUserIPv6PortTest();
bool urlUserPasswordIPv6PortTest();

int main()
{
    std::cout << "START TESTS" << std::endl;

    if(urlEmptyTest())
        std::cout << "urlEmptyTest \tOK" << std::endl;
    else
        std::cout << "urlEmptyTest \tFAILED" << std::endl;

    if(urlSchemeTest())
        std::cout << "urlSchemeTest \tOK" << std::endl;
    else
        std::cout << "urlSchemeTest \tFAILED" << std::endl;

    if(urlIPv6Test())
        std::cout << "urlIPv6Test \tOK" << std::endl;
    else
        std::cout << "urlIPv6Test \tFAILED" << std::endl;

    if(urlIPv6PortTest())
        std::cout << "urlIPv6PortTest \tOK" << std::endl;
    else
        std::cout << "urlIPv6PortTest \tFAILED" << std::endl;

    if(urlUserIPv6PortTest())
        std::cout << "urlUserIPv6PortTest \tOK" << std::endl;
    else
        std::cout << "urlUserIPv6PortTest \tFAILED" << std::endl;

    if(urlUserPasswordIPv6PortTest())
        std::cout << "urlUserPasswordIPv6PortTest \tOK" << std::endl;
    else
        std::cout << "urlUserPasswordIPv6PortTest \tFAILED" << std::endl;

    std::cout << "FINISH TESTS" << std::endl;
    return 0;
}


//===================================================================

bool urlEmptyTest()
{
    URL url;
    if(url.scheme() != L"")
        return false;

    if(url.user() != L"")
        return false;

    if(url.password() != L"")
        return false;

    if(url.host() != L"")
        return false;

    if(url.port() != 0)
        return false;

    if(url.path() != L"")
        return false;

    if(url.query() != L"")
        return false;

    if(url.fragment() != L"")
        return false;

    return true;
}

bool urlSchemeTest()
{
    URL url(L"https://");
    if(url.scheme() != L"https")
        return false;

    if(url.user() != L"")
        return false;

    if(url.password() != L"")
        return false;

    if(url.host() != L"")
        return false;

    if(url.port() != 0)
        return false;

    if(url.path() != L"")
        return false;

    if(url.query() != L"")
        return false;

    if(url.fragment() != L"")
        return false;

    return true;
}

bool urlIPv6Test()
{
    URL url(L"http://[2a00:11d8:1201:0:962b:18:e716:fb97]");
    if(url.scheme() != L"http")
        return false;

    if(url.user() != L"")
        return false;

    if(url.password() != L"")
        return false;

    if(url.host() != L"[2a00:11d8:1201:0:962b:18:e716:fb97]")
        return false;

    if(url.port() != 0)
        return false;

    if(url.path() != L"")
        return false;

    if(url.query() != L"")
        return false;

    if(url.fragment() != L"")
        return false;

    return true;
}

bool urlIPv6PortTest()
{
    URL url(L"http://[2a00:11d8:1201:0:962b:18:e716:fb97]:8080");
    if(url.scheme() != L"http")
        return false;

    if(url.user() != L"")
        return false;

    if(url.password() != L"")
        return false;

    if(url.host() != L"[2a00:11d8:1201:0:962b:18:e716:fb97]")
        return false;

    if(url.port() != 8080)
        return false;

    if(url.path() != L"")
        return false;

    if(url.query() != L"")
        return false;

    if(url.fragment() != L"")
        return false;

    return true;
}

bool urlUserIPv6PortTest()
{
    URL url(L"http://user@[2a00:11d8:1201:0:962b:18:e716:fb97]:8080");
    if(url.scheme() != L"http")
        return false;

    if(url.user() != L"user")
        return false;

    if(url.password() != L"")
        return false;

    if(url.host() != L"[2a00:11d8:1201:0:962b:18:e716:fb97]")
        return false;

    if(url.port() != 8080)
        return false;

    if(url.path() != L"")
        return false;

    if(url.query() != L"")
        return false;

    if(url.fragment() != L"")
        return false;

    return true;
}

bool urlUserPasswordIPv6PortTest()
{
    URL url(L"http://user:password@[2a00:11d8:1201:0:962b:18:e716:fb97]:8080");
    if(url.scheme() != L"http")
        return false;

    if(url.user() != L"user")
        return false;

    if(url.password() != L"password")
        return false;

    if(url.host() != L"[2a00:11d8:1201:0:962b:18:e716:fb97]")
        return false;

    if(url.port() != 8080)
        return false;

    if(url.path() != L"")
        return false;

    if(url.query() != L"")
        return false;

    if(url.fragment() != L"")
        return false;

    return true;
}

