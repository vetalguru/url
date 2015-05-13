#ifndef URL_H
#define URL_H

#include <string>

class URL
{
public:
    URL();
    URL(const std::string &aUrl);
    ~URL();

    std::string scheme() const;
    std::string user() const;
    std::string password() const;
    std::string host() const;
    unsigned    port() const;
    std::string path() const;
    std::string query() const;
    std::string fragment() const;


    URL& operator = (const URL& aURL);

private:
    bool parseUrl(const std::string& aUrl);

    bool isNonReserved(const char aSymbol);
    bool isDelims(const char aSymbol);
    bool isSubDelims(const char aSymbol);
    bool isHexDigit(const char aDigit);

    bool parseScheme(char **ptr, std::string& aScheme);

    bool parseAuthorityInfo(char **ptr, std::string& aUserInfo, std::string &aHostInfo, unsigned &aPort);

    bool parseUserInfo(char **ptr, std::string& aUserInfo);
    bool parseHostInfo(char **ptr, std::string& aHostInfo);

    bool parseDomainName(char **ptr, std::string& aAddress);
    bool parseIPv4Address(char **ptr, std::string& aAddress);
    bool parseIPv6Address(char **ptr, std::string& aAddress);

    bool parsePortInfo(char **ptr, unsigned &aPort);

    bool parsePath(char **ptr, std::string& aPath);

    bool parseQuery(char **ptr, std::string& aQuery);

    bool parseFragment(char **ptr, std::string& aFragment);


    bool parseH16(char **ptr, std::string& aH16);
    bool parseOctet(char **ptr, std::string& aOct);

private:
    std::string m_scheme;
    std::string m_user;
    std::string m_password;
    std::string m_host;
    unsigned    m_port;
    std::string m_path;
    std::string m_query;
    std::string m_fragment;

};

#endif // URL_H
