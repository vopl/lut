#pragma once

#include <string>

namespace lut { namespace io
{
    namespace ip4
    {
        struct Address
        {
            union
            {
                unsigned char _chars[4];
                std::uint32_t _uint32;
            } _value;

            bool parse(const char *&str, const char *end);
            std::string asString() const;
        };
    }

    namespace ip6
    {
        struct Address
        {
            std::uint16_t _value[8];

            bool parse(const char *&str, const char *end);
            std::string asString() const;
        };
    }

    namespace str
    {
        class Address
        {
        public:
            Address();
            Address(const Address &from);
            Address(Address &&from);

            Address(const char *from, const char *end = nullptr);
            Address(const std::string &from);

            ~Address();

            Address &operator=(const Address &from);
            Address &operator=(Address &&from);

            std::size_t size() const;
            const char *csz() const;

            std::string string() const;

        private:
            std::uint16_t _size;

            union
            {
                char *_external;
                char _internal[14];
            } _data;
        };
    }

    enum class Scope : std::uint8_t
    {
        null,

        ipc,

        domainName,

        ip4_domainName,
        ip4,

        ip6_domainName,
        ip6,
    };

    class Endpoint
    {

    public:
        Endpoint();

        Endpoint(const Endpoint &from);
        Endpoint(Endpoint &&from);

        Endpoint(const char *str, const char *end);
        Endpoint(const char *str);
        Endpoint(const std::string &str);

        Endpoint(const ip4::Address &address, std::uint16_t port);
        Endpoint(const ip6::Address &address, std::uint16_t port);
        Endpoint(const str::Address &address, std::uint16_t port);

        ~Endpoint();

        Endpoint &operator=(const Endpoint &from);
        Endpoint &operator=(Endpoint &&from);


        Scope scope() const;

        const ip4::Address &addressIp4() const;
        const ip6::Address &addressIp6() const;
        const str::Address &addressStr() const;
        std::uint16_t port() const;

        std::string asString() const;

    private:
        ip4::Address &addressIp4();
        ip6::Address &addressIp6();
        str::Address &addressStr();

    private:
        union
        {
            std::aligned_storage<sizeof(ip4::Address), alignof(ip4::Address)>::type _ip4Area;
            std::aligned_storage<sizeof(ip6::Address), alignof(ip6::Address)>::type _ip6Area;
            std::aligned_storage<sizeof(str::Address), alignof(str::Address)>::type _strArea;
        } _address;

        Scope _scope;

        std::uint16_t _port;
    };
}}
