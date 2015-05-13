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
    void setScheme(const std::string& aScheme);

    std::string user() const;
    void setUser(const std::string& aUser);

    std::string password() const;
    void setPassword(const std::string& aPassword);

    std::string host() const;
    void setHost(const std::string& aHost);

    unsigned port() const;
    void setPort(const unsigned aPort);

    std::string path() const;
    void setPath(const std::string& aPath);

    std::string query() const;
    void setQuery(const std::string& aQuery);

    std::string fragment() const;
    void setFragment(const std::string& aFragment);

    bool setURL(const std::string& aUrl);
    std::string getURLString();


    URL& operator = (const URL& aURL);
    bool operator == (const URL& aURL);

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
