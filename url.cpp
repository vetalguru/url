#include "url.h"

#include <iostream>
#include <cctype>
#include <cstdlib>

URL::URL()
    : m_port(0)
{
}

URL::URL(const std::string& aUrl)
    : m_port(0)
{
    parseUrl(aUrl);
}

URL::~URL()
{
}

bool URL::isNonReserved(const char aSymbol)
{
    if(isalpha(aSymbol) || isdigit(aSymbol))
        return true;

    switch(aSymbol)
    {
        case '-': case '.': case '_' : case '~':
        return true;
    }

    return false;
}

bool URL::isDelims(const char aSymbol)
{
    switch(aSymbol)
    {
        case ':': case '/': case '?': case '#':
        case '[': case ']': case '@':
            return true;
    }

    return false;
}

bool URL::isSubDelims(const char aSymbol)
{
    switch(aSymbol)
    {
        case '!': case '$': case '&': case '\\':
        case '(': case ')': case '*': case '+':
        case ',': case ';': case '=':
            {
                return true;
            }
    }

    return false;
}

bool URL::isHexDigit(const char aDigit)
{
    if(isdigit(aDigit) || (aDigit >= 'a' && aDigit <= 'f') || (aDigit >= 'A' && aDigit <= 'F'))
        return true;

    return false;
}

bool URL::parseUrl(const std::string& aUrl)
{
    if(aUrl.empty())
        return false;

    char *pptr = (char*)aUrl.c_str();
    char **ptr = &pptr;

    // <scheme>:<scheme-specific-part>

    // option
    parseScheme(ptr, m_scheme);

    // //<user>:<password>@<host>:<port>/<url-path>
    std::string userInfo;
    parseAuthorityInfo(ptr, userInfo, m_host, m_port);

    if(!userInfo.empty())
    {
        bool isUserName = true;
        for(size_t i = 0; i < userInfo.size(); i++)
        {
            if(userInfo.at(i) == ':')
            {
                isUserName = false;
                continue;
            }

            if(isUserName)
                m_user += userInfo.at(i);
            else
                m_password += userInfo.at(i);
        }
    }

    parsePath(ptr, m_path);

    parseQuery(ptr, m_query);

    parseFragment(ptr, m_fragment);

    return true;
}

bool URL::parseScheme(char **ptr, std::string& aScheme)
{
    char *ptrBackup = *ptr;
    bool first = true;
    for (;;)
    {
        char ch = **ptr;
        if (isalpha(ch))
            aScheme += ch;
        else if (!first && (isdigit(ch) || ch == '+' || ch == '-' || ch == '.'))
            aScheme += ch;
        else
            break;

        ++(*ptr);
        first = false;
    }

    char ch = *((*ptr)++);
    if(ch != ':')
    {
        *ptr = ptrBackup;
        aScheme.clear();
        return false;
    }

    return true;
}

bool URL::parseAuthorityInfo(char **ptr, std::string& aUserInfo, std::string& aHostInfo, unsigned& aPort)
{
    if(*((*ptr)++) == '/' && *((*ptr)++) == '/')
    {
        char *ptrBackup = *ptr;

        if(parseUserInfo(ptr, aUserInfo))
        {
            if(*((*ptr)++) != '@')
            {
                *ptr = ptrBackup;
                aUserInfo.clear();
            }
        }

        if(!parseHostInfo(ptr, aHostInfo))
        {
            *ptr = ptrBackup;
            return false;
        }

        char *ptrBackup2 = *ptr;
        if(*((*ptr)++) != ':')
        {
            *ptr = ptrBackup2;
            return true;
        }

        if(!parsePortInfo(ptr, aPort))
        {
            *ptr = ptrBackup2;
            return false;
        }
    }

    return true;
}

bool URL::parseUserInfo(char **ptr, std::string& aUserInfo)
{
    for(;;)
    {
        char ch = **ptr;
        if(isalpha(ch) || isdigit(ch) || ch == '-' || ch == '.' || ch == '_' || ch == '~' || isSubDelims(ch) || ch == ':')
        {
            aUserInfo += ch;
            (*ptr)++;
        }
        else
            break;
    }

    return true;
}

bool URL::parseHostInfo(char **ptr, std::string& aHostInfo)
{
    aHostInfo.clear();

    char *ptrBackup = *ptr;
    if(!parseIPv6Address(ptr, aHostInfo))
    {
        *ptr = ptrBackup;

        if(!parseIPv4Address(ptr, aHostInfo))
        {
            *ptr = ptrBackup;

            return parseDomainName(ptr,aHostInfo);
        }
    }

    return true;
}

bool URL::parseDomainName(char **ptr, std::string& aAddress)
{
    aAddress.clear();

    for(;;)
    {
        char ch = **ptr;
        if(!isNonReserved(ch) && !isSubDelims(ch))
        {
            break;
        }
        else
        {
            aAddress += ch;
            (*ptr)++;
        }
    }

    return true;
}

bool URL::parseIPv4Address(char **ptr, std::string& aAddress)
{
    aAddress.clear();

    char *ptrBackup = *ptr;
    std::string tmp;
    tmp.resize(32);

    bool res = parseOctet(ptr, tmp);
    if(!res)
    {
        *ptr = ptrBackup;
        return false;
    }

    for(int i = 0; i < 3; i++)
    {
        char ch = *((*ptr)++);
        if(ch != '.')
        {
            *ptr = ptrBackup;
            return false;
        }

        tmp += '.';

        std::string tmp1;
        res = parseOctet(ptr, tmp1);
        if(!res)
        {
            *ptr = ptrBackup;
            return false;
        }

        tmp += tmp1;
    }

    aAddress = tmp;

    return true;
}

bool URL::parseIPv6Address(char **ptr, std::string& aAddress)
{
    aAddress.clear();

    char *ptrBackup = *ptr;

    char ch = **ptr;
    if(ch == '[')
    {
        aAddress += '[';
        (*ptr)++;
    }
    else
        return false;

    std::string tmp;

    int leftHexColons = 0;

    for(;;)
    {
        if(!parseH16(ptr, tmp))
            break;

        if(leftHexColons != 7)
        {
            char ch = **ptr;
            if(ch != ':')
            {
                *ptr = ptrBackup;
                return false;
            }

            tmp += ':';
            (*ptr)++;
        }

        aAddress += tmp;
        leftHexColons++;
    }

    ch = **ptr;
    if(ch != ']')
    {
        *ptr = ptrBackup;
        return false;
    }

    aAddress += ']';
    (*ptr)++;

    return true;
}

bool URL::parsePath(char **ptr, std::string& aPath)
{
    aPath.clear();

    char *ptrBackup = *ptr;
    if(*((*ptr)++) != '/')
    {
        *ptr = ptrBackup;
        return false;
    }

    aPath += '/';

    while(true)
    {
        char ch = **ptr;
        if(isdigit(ch) || isalpha(ch) || isNonReserved(ch) || isSubDelims(ch)
            || ch == ':' || ch == '@' || ch == '/')
        {
            aPath += ch;
            (*ptr)++;
        }
        else
            break;
    }

    if(aPath.empty())
    {
        *ptr = ptrBackup;
        return false;
    }

    return true;
}

bool URL::parseQuery(char **ptr, std::string& aQuery)
{
    aQuery.clear();

    char ch = **ptr;
    if(ch != '?')
        return false;

    (*ptr)++;

    for(;;)
    {
        ch  = *((*ptr)++);

        if(isdigit(ch) || isalpha(ch) || isNonReserved(ch) || isSubDelims(ch)
            || ch == ':' || ch == '@' || ch == '/')
        {
            aQuery += ch;
        }
        else
        {
            char *ptrBackup = *ptr;

            if(ch == '/' || ch == '?')
                aQuery += ch;
            else
            {
                *ptr = ptrBackup;
                break;
            }
        }
    }

    if(aQuery.empty())
        return false;

    return true;
}

bool URL::parseFragment(char **ptr, std::string& aFragment)
{
    aFragment.clear();

    char ch = **ptr;
    if(ch != '#')
        return false;

    (*ptr)++;

    for(;;)
    {
        char *ptrBackup = *ptr;
        ch = *((*ptr)++);

        if(isdigit(ch) || isalpha(ch) || isNonReserved(ch) || isSubDelims(ch)
            || ch == ':' || ch == '@' || ch =='/')
        {
            aFragment += ch;
        }
        else
        {
            if(ch == '/' || ch == '?' || ch == '#')
                aFragment += ch;
            else
            {
                *ptr = ptrBackup;
                break;
            }
        }
    }

    if(aFragment.empty())
        return false;

    return true;
}

bool URL::parseH16(char **ptr, std::string& aH16)
{
    aH16.clear();

    char ch = **ptr;
    if(!isHexDigit(ch))
        return false;

    aH16 += ch;

    (*ptr)++;

    for(int i = 0; i < 3; i++)
    {
        char ch = **ptr;
        if(!isHexDigit(ch))
            break;

        aH16 += ch;
        (*ptr)++;
    }

    if(aH16.empty())
        return false;

    return true;
}

bool URL::parseOctet(char **ptr, std::string& aOct)
{
    aOct.clear();

    for(int i = 0; i < 3; i++)
    {
        char ch = **ptr;
        if(!isdigit(ch))
        {
            if(ch == '.' && atoi(aOct.c_str()) < 255)
                return true;

            return false;
        }

        aOct += ch;
        (*ptr)++;

        if(i == 0 && ch == '0')
            return true;
    }

    if(atoi(aOct.c_str()) > 255)
        return false;

    return true;
}

bool URL::parsePortInfo(char **ptr, unsigned& aPort)
{
    aPort = 0;
    std::string num;

    for(;;)
    {
        char *ptrBackup = *ptr;
        char ch = *((*ptr)++);

        if(!isdigit(ch))
        {
            *ptr = ptrBackup;
            break;
        }

        num += ch;
    }

    if(num.empty())
        return false;

    aPort = atoi(num.c_str());

    return true;
}


std::string URL::scheme() const
{
    return m_scheme;
}

std::string URL::user() const
{
    return m_user;
}

std::string URL::password() const
{
    return m_password;
}

std::string URL::host() const
{
    return m_host;
}

unsigned URL::port() const
{
    return m_port;
}

std::string URL::path() const
{
    return m_path;
}

std::string URL::query() const
{
    return m_query;
}

std::string URL::fragment() const
{
    return m_fragment;
}


