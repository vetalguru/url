#include "url.h"

#include <cctype>
#include <stdio.h>
#include <cstdlib>

URL::URL()
    : m_port(0)
{
}

URL::URL(const std::wstring &aUrl)
    : m_port(0)
{
    parseUrl(aUrl);
}

URL::~URL()
{
}

bool URL::isNonReserved(const wchar_t aSymbol)
{
    if(isalpha(aSymbol) || isdigit(aSymbol))
        return true;

    switch(aSymbol)
    {
        case L'-': case L'.': case L'_' : case L'~':
        return true;
    }

    return false;
}

bool URL::isDelims(const wchar_t aSymbol)
{
    switch(aSymbol)
    {
        case L':': case L'/': case L'?': case L'#':
        case L'[': case L']': case L'@':
            return true;
    }

    return false;
}

bool URL::isSubDelims(const wchar_t aSymbol)
{
    switch(aSymbol)
    {
        case L'!': case L'$': case L'&': case L'\\':
        case L'(': case L')': case L'*': case L'+':
        case L',': case L';': case L'=':
            {
                return true;
            }
    }

    return false;
}

bool URL::isHexDigit(const wchar_t aDigit)
{
    if(isdigit(aDigit) || (aDigit >= L'a' && aDigit <= L'f') || (aDigit >= L'A' && aDigit <= L'F'))
        return true;

    return false;
}

bool URL::parseUrl(const std::wstring& aUrl)
{
    if(aUrl.empty())
        return false;

    wchar_t *pptr = (wchar_t*)aUrl.c_str();
    wchar_t **ptr = &pptr;

    // option
    parseScheme(ptr, m_scheme);

    // //<user>:<password>@<host>:<port>/<url-path>
    std::wstring userInfo;
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

bool URL::parseScheme(wchar_t **ptr, std::wstring& aScheme)
{
    wchar_t *ptrBackup = *ptr;
    bool first = true;
    for(;;)
    {
        wchar_t ch = **ptr;
        if (isalpha(ch))
            aScheme += ch;
        else if (!first && (isdigit(ch) || ch == L'+' || ch == L'-' || ch == L'.'))
            aScheme += ch;
        else
            break;

        ++(*ptr);
        first = false;
    }

    wchar_t ch = *((*ptr)++);
    if(ch != L':')
    {
        *ptr = ptrBackup;
        aScheme.clear();
        return false;
    }

    return true;
}

bool URL::parseAuthorityInfo(wchar_t **ptr, std::wstring& aUserInfo, std::wstring& aHostInfo, unsigned& aPort)
{
    if(*((*ptr)++) == L'/' && *((*ptr)++) == L'/')
    {
        wchar_t *ptrBackup = *ptr;

        if(parseUserInfo(ptr, aUserInfo))
        {
            if(*((*ptr)++) != L'@')
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

        wchar_t *ptrBackup2 = *ptr;
        if(*((*ptr)++) != L':')
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

bool URL::parseUserInfo(wchar_t **ptr, std::wstring& aUserInfo)
{
    for(;;)
    {
        wchar_t ch = **ptr;
        if(isalpha(ch) || isdigit(ch) || ch == L'-' || ch == L'.' || ch == L'_' || ch == L'~' || isSubDelims(ch) || ch == L':')
        {
            aUserInfo += ch;
            (*ptr)++;
        }
        else
            break;
    }

    return true;
}

bool URL::parseHostInfo(wchar_t **ptr, std::wstring& aHostInfo)
{
    aHostInfo.clear();

    wchar_t *ptrBackup = *ptr;
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

bool URL::parseDomainName(wchar_t **ptr, std::wstring& aAddress)
{
    aAddress.clear();

    for(;;)
    {
        wchar_t ch = **ptr;
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

bool URL::parseIPv4Address(wchar_t **ptr, std::wstring& aAddress)
{
    aAddress.clear();

    wchar_t *ptrBackup = *ptr;
    std::wstring tmp;
    tmp.resize(32);

    bool res = parseOctet(ptr, tmp);
    if(!res)
    {
        *ptr = ptrBackup;
        return false;
    }

    for(int i = 0; i < 3; i++)
    {
        wchar_t ch = *((*ptr)++);
        if(ch != L'.')
        {
            *ptr = ptrBackup;
            return false;
        }

        tmp += L'.';

        std::wstring tmp1;
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

bool URL::parseIPv6Address(wchar_t **ptr, std::wstring& aAddress)
{
    aAddress.clear();

    wchar_t *ptrBackup = *ptr;

    wchar_t ch = **ptr;
    if(ch == L'[')
    {
        aAddress += L'[';
        (*ptr)++;
    }
    else
        return false;

    std::wstring tmp;

    int leftHexColons = 0;

    for(;;)
    {
        if(!parseH16(ptr, tmp))
            break;

        if(leftHexColons != 7)
        {
            wchar_t ch = **ptr;
            if(ch != L':')
            {
                *ptr = ptrBackup;
                return false;
            }

            tmp += L':';
            (*ptr)++;
        }

        aAddress += tmp;
        leftHexColons++;
    }

    ch = **ptr;
    if(ch != L']')
    {
        *ptr = ptrBackup;
        return false;
    }

    aAddress += L']';
    (*ptr)++;

    return true;
}

bool URL::parsePath(wchar_t **ptr, std::wstring& aPath)
{
    aPath.clear();

    wchar_t *ptrBackup = *ptr;
    if(*((*ptr)++) != L'/')
    {
        *ptr = ptrBackup;
        return false;
    }

    aPath += L'/';

    while(true)
    {
        wchar_t ch = **ptr;
        if(isdigit(ch) || isalpha(ch) || isNonReserved(ch) || isSubDelims(ch)
            || ch == L':' || ch == L'@' || ch == L'/')
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

bool URL::parseQuery(wchar_t **ptr, std::wstring& aQuery)
{
    aQuery.clear();

    wchar_t ch = **ptr;
    if(ch != L'?')
        return false;

    (*ptr)++;

    for(;;)
    {
        ch  = *((*ptr)++);

        if(isdigit(ch) || isalpha(ch) || isNonReserved(ch) || isSubDelims(ch)
            || ch == L':' || ch == L'@' || ch == L'/')
        {
            aQuery += ch;
        }
        else
        {
            wchar_t *ptrBackup = *ptr;

            if(ch == L'/' || ch == L'?')
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

bool URL::parseFragment(wchar_t **ptr, std::wstring& aFragment)
{
    aFragment.clear();

    wchar_t ch = **ptr;
    if(ch != L'#')
        return false;

    (*ptr)++;

    for(;;)
    {
        wchar_t *ptrBackup = *ptr;
        ch = *((*ptr)++);

        if(isdigit(ch) || isalpha(ch) || isNonReserved(ch) || isSubDelims(ch)
            || ch == L':' || ch == L'@' || ch == L'/')
        {
            aFragment += ch;
        }
        else
        {
            if(ch == L'/' || ch == L'?' || ch == L'#')
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

bool URL::parseH16(wchar_t **ptr, std::wstring& aH16)
{
    aH16.clear();

    wchar_t ch = **ptr;
    if(!isHexDigit(ch))
        return false;

    aH16 += ch;

    (*ptr)++;

    for(int i = 0; i < 3; i++)
    {
        wchar_t ch = **ptr;
        if(!isHexDigit(ch))
            break;

        aH16 += ch;
        (*ptr)++;
    }

    if(aH16.empty())
        return false;

    return true;
}

bool URL::parseOctet(wchar_t **ptr, std::wstring& aOct)
{
    aOct.clear();

    for(int i = 0; i < 3; i++)
    {
        wchar_t ch = **ptr;
        if(!isdigit(ch))
        {
            if(ch == L'.' && atoi(std::string(aOct.begin(), aOct.end()).c_str()) < 255)
                return true;

            return false;
        }

        aOct += ch;
        (*ptr)++;

        if(i == 0 && ch == L'0')
            return true;
    }

    if(atoi(std::string(aOct.begin(), aOct.end()).c_str()) > 255)
        return false;

    return true;
}

bool URL::parsePortInfo(wchar_t **ptr, unsigned& aPort)
{
    aPort = 0;
    std::wstring num;

    for(;;)
    {
        wchar_t *ptrBackup = *ptr;
        wchar_t ch = *((*ptr)++);

        if(!isdigit(ch))
        {
            *ptr = ptrBackup;
            break;
        }

        num += ch;
    }

    if(num.empty())
        return false;

    aPort = atoi(std::string(num.begin(), num.end()).c_str());

    return true;
}


std::wstring URL::scheme() const
{
    return m_scheme;
}

void URL::setScheme(const std::wstring& aScheme)
{
    m_scheme == aScheme;
}

std::wstring URL::user() const
{
    return m_user;
}

void URL::setUser(const std::wstring& aUser)
{
    m_user = aUser;
}

std::wstring URL::password() const
{
    return m_password;
}

void URL::setPassword(const std::wstring& aPassword)
{
    m_password = aPassword;
}

std::wstring URL::host() const
{
    return m_host;
}

void URL::setHost(const std::wstring& aHost)
{
    m_host = aHost;
}

unsigned URL::port() const
{
    return m_port;
}

void URL::setPort(const unsigned aPort)
{
    m_port = aPort;
}

std::wstring URL::path() const
{
    return m_path;
}

void URL::setPath(const std::wstring& aPath)
{
    m_path = aPath;
}

std::wstring URL::query() const
{
    return m_query;
}

void URL::setQuery(const std::wstring& aQuery)
{
    m_query = aQuery;
}

std::wstring URL::fragment() const
{
    return m_fragment;
}

void URL::setFragment(const std::wstring& aFragment)
{
    m_fragment = aFragment;
}

bool URL::setURL(const std::wstring& aUrl)
{
    return parseUrl(aUrl);
}

std::wstring URL::getURLString()
{
    std::wstring result;

    if(!m_scheme.empty())
    {
        result += m_scheme;
        result += L"://";
    }

    if(!m_user.empty())
    {
        result += m_user;

        if(!m_password.empty())
        {
            result += L':';
            result += m_password;
        }

        result += L'@';
    }

    if(!m_host.empty())
    {
        result += m_host;

        if(m_port)
        {
            result += L':';
            char buf[20];
            sprintf(buf, "%d", m_port);
            std::string port(buf);
            result += std::wstring(port.begin(), port.end());
        }
    }

    if(!m_path.empty())
    {
        result += L'/';
        result += m_path;
    }

    if(!m_query.empty())
    {
        result += L'?';
        result += m_query;
    }

    if(!m_fragment.empty())
    {
        result += L'#';
        result += m_fragment;
    }

    return result;
}

URL& URL::operator = (const URL& aURL)
{
    m_scheme   = aURL.scheme();
    m_user     = aURL.user();
    m_password = aURL.password();
    m_host     = aURL.host();
    m_port     = aURL.port();
    m_path     = aURL.path();
    m_query    = aURL.query();
    m_fragment = aURL.fragment();

    return *this;
}

bool URL::operator == (const URL& aURL)
{
    if( m_scheme   == aURL.scheme()   &&
        m_user     == aURL.user()     &&
        m_password == aURL.password() &&
        m_host     == aURL.host()     &&
        m_port     == aURL.port()     &&
        m_path     == aURL.path()     &&
        m_query    == aURL.query()    &&
        m_fragment == aURL.fragment())
        return true;

    return false;
}

