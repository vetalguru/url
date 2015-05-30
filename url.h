#ifndef URL_H
#define URL_H

#include <string>

class URL
{
public:
    URL();
    URL(const std::wstring &aUrl);
    ~URL();

    std::wstring scheme() const;
    void setScheme(const std::wstring& aScheme);

    std::wstring user() const;
    void setUser(const std::wstring& aUser);

    std::wstring password() const;
    void setPassword(const std::wstring& aPassword);

    std::wstring host() const;
    void setHost(const std::wstring& aHost);

    unsigned port() const;
    void setPort(const unsigned aPort);

    std::wstring path() const;
    void setPath(const std::wstring& aPath);

    std::wstring query() const;
    void setQuery(const std::wstring& aQuery);

    std::wstring fragment() const;
    void setFragment(const std::wstring& aFragment);

    bool setURL(const std::wstring& aUrl);
    std::wstring getURLString();


    URL& operator = (const URL& aURL);
    bool operator == (const URL& aURL);

private:
    bool parseUrl(const std::wstring& aUrl);

    bool isNonReserved(const wchar_t aSymbol);
    bool isDelims(const wchar_t aSymbol);
    bool isSubDelims(const wchar_t aSymbol);
    bool isHexDigit(const wchar_t aDigit);

    bool parseScheme(wchar_t **ptr, std::wstring& aScheme);

    bool parseAuthorityInfo(wchar_t **ptr, std::wstring& aUserInfo, std::wstring &aHostInfo, unsigned &aPort);

    bool parseUserInfo(wchar_t **ptr, std::wstring& aUserInfo);
    bool parseHostInfo(wchar_t **ptr, std::wstring& aHostInfo);

    bool parseDomainName(wchar_t **ptr, std::wstring& aAddress);
    bool parseIPv4Address(wchar_t **ptr, std::wstring& aAddress);
    bool parseIPv6Address(wchar_t **ptr, std::wstring& aAddress);

    bool parsePortInfo(wchar_t **ptr, unsigned &aPort);

    bool parsePath(wchar_t **ptr, std::wstring& aPath);

    bool parseQuery(wchar_t **ptr, std::wstring& aQuery);

    bool parseFragment(wchar_t **ptr, std::wstring& aFragment);


    bool parseH16(wchar_t **ptr, std::wstring& aH16);
    bool parseOctet(wchar_t **ptr, std::wstring& aOct);

private:
    std::wstring m_scheme;
    std::wstring m_user;
    std::wstring m_password;
    std::wstring m_host;
    unsigned     m_port;
    std::wstring m_path;
    std::wstring m_query;
    std::wstring m_fragment;

};

#endif // URL_H
