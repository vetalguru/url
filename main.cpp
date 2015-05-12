#include <iostream>
#include "url.h"

/**

URL list
https://
http://[2a00:11d8:1201:0:962b:18:e716:fb97]
http://[2a00:11d8:1201:0:962b:18:e716:fb97]:8080
http://user@[2a00:11d8:1201:0:962b:18:e716:fb97]:8080
http://user:password@[2a00:11d8:1201:0:962b:18:e716:fb97]:8080

*/


bool urlSchemeTest();
bool urlIPv6Test();
bool urlIPv6PortTest();
bool urlUserIPv6PortTest();
bool urlUserPasswordIPv6PortTest();

int main()
{
    std::cout << "START TESTS" << std::endl;

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

bool urlSchemeTest()
{
    URL url("https://");
    if(url.scheme() != "https")
        return false;

    if(url.user() != "")
        return false;

    if(url.password() != "")
        return false;

    if(url.host() != "")
        return false;

    if(url.port() != 0)
        return false;

    if(url.path() != "")
        return false;

    if(url.query() != "")
        return false;

    if(url.fragment() != "")
        return false;

    return true;
}

bool urlIPv6Test()
{
    URL url("http://[2a00:11d8:1201:0:962b:18:e716:fb97]");
    if(url.scheme() != "http")
        return false;

    if(url.user() != "")
        return false;

    if(url.password() != "")
        return false;

    if(url.host() != "[2a00:11d8:1201:0:962b:18:e716:fb97]")
        return false;

    if(url.port() != 0)
        return false;

    if(url.path() != "")
        return false;

    if(url.query() != "")
        return false;

    if(url.fragment() != "")
        return false;

    return true;
}

bool urlIPv6PortTest()
{
    URL url("http://[2a00:11d8:1201:0:962b:18:e716:fb97]:8080");
    if(url.scheme() != "http")
        return false;

    if(url.user() != "")
        return false;

    if(url.password() != "")
        return false;

    if(url.host() != "[2a00:11d8:1201:0:962b:18:e716:fb97]")
        return false;

    if(url.port() != 8080)
        return false;

    if(url.path() != "")
        return false;

    if(url.query() != "")
        return false;

    if(url.fragment() != "")
        return false;

    return true;
}

bool urlUserIPv6PortTest()
{
    URL url("http://user@[2a00:11d8:1201:0:962b:18:e716:fb97]:8080");
    if(url.scheme() != "http")
        return false;

    if(url.user() != "user")
        return false;

    if(url.password() != "")
        return false;

    if(url.host() != "[2a00:11d8:1201:0:962b:18:e716:fb97]")
        return false;

    if(url.port() != 8080)
        return false;

    if(url.path() != "")
        return false;

    if(url.query() != "")
        return false;

    if(url.fragment() != "")
        return false;

    return true;
}

bool urlUserPasswordIPv6PortTest()
{
    URL url("http://user:password@[2a00:11d8:1201:0:962b:18:e716:fb97]:8080");
    if(url.scheme() != "http")
        return false;

    if(url.user() != "user")
        return false;

    if(url.password() != "password")
        return false;

    if(url.host() != "[2a00:11d8:1201:0:962b:18:e716:fb97]")
        return false;

    if(url.port() != 8080)
        return false;

    if(url.path() != "")
        return false;

    if(url.query() != "")
        return false;

    if(url.fragment() != "")
        return false;

    return true;
}

