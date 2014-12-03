#include "lut/stable.hpp"
#include "lut/io/endpoint.hpp"
#include <boost/spirit/include/qi.hpp>
#include <string>

namespace lut { namespace io
{

    namespace ip4
    {
        bool Address::parse(const char *&str, const char *end)
        {
            const char *begin = str;
            namespace qi = boost::spirit::qi;
            namespace ascii = boost::spirit::ascii;

            qi::uint_parser<unsigned, 10, 1, 3> uint3d;
            unsigned int v0, v1, v2, v3;

            if(!qi::parse(begin, end,
                          uint3d[([&](unsigned v){v0=v;})] >> '.' >>
                          uint3d[([&](unsigned v){v1=v;})] >> '.' >>
                          uint3d[([&](unsigned v){v2=v;})] >> '.' >>
                          uint3d[([&](unsigned v){v3=v;})]))
            {
                return false;
            }

            if(0xff <= v0 || 0xff <= v1 || 0xff <= v2 || 0xff <= v3)
            {
                return false;
            }

            _value._chars[3] = v0;
            _value._chars[2] = v1;
            _value._chars[1] = v2;
            _value._chars[0] = v3;

            str = begin;
            return true;
        }

        std::string Address::asString() const
        {
            char buf[16];
            int len = std::sprintf(buf, "%hhu.%hhu.%hhu.%hhu", _value._chars[3], _value._chars[2], _value._chars[1], _value._chars[0]);
            return std::string(buf, len);
        }
    }

    namespace ip6
    {
        bool Address::parse(const char *&str, const char *end)
        {
            const char *begin = str;
            namespace qi = boost::spirit::qi;
            namespace ascii = boost::spirit::ascii;

            qi::uint_parser<unsigned, 16, 4, 4> uhex4d;
            unsigned int v0, v1, v2, v3, v4, v5, v6, v7;

            if(!qi::parse(begin, end,
                          '[' >>
                          (
                              (uhex4d[([&](unsigned v){v0=v;})] >> ':') >>
                              (uhex4d[([&](unsigned v){v1=v;})] >> ':') >>
                              (uhex4d[([&](unsigned v){v2=v;})] >> ':') >>
                              (uhex4d[([&](unsigned v){v3=v;})] >> ':') >>
                              (uhex4d[([&](unsigned v){v4=v;})] >> ':') >>
                              (uhex4d[([&](unsigned v){v5=v;})] >> ':') >>
                              (uhex4d[([&](unsigned v){v6=v;})] >> ':') >>
                              (uhex4d[([&](unsigned v){v7=v;})])
                          ) >> ']'))
            {
                return false;
            }


            _value[0] = v0;
            _value[1] = v1;
            _value[2] = v2;
            _value[3] = v3;
            _value[4] = v4;
            _value[5] = v5;
            _value[6] = v6;
            _value[7] = v7;

            str = begin;
            return true;
        }

        std::string Address::asString() const
        {
            char buf[40];
            int len = std::sprintf(buf, "[%hx:%hx:%hx:%hx:%hx:%hx:%hx:%hx]", _value[0], _value[1], _value[2], _value[3], _value[4], _value[5], _value[6], _value[7]);
            return std::string(buf, len);
        }

    }

    namespace str
    {
        Address::Address()
            : _size{0}
        {
        }

        Address::Address(const Address &from)
            : _size{from._size}
        {
            if(_size <= sizeof(_data._internal)-1)
            {
                memcpy(_data._internal, from._data._internal, _size+1);
            }
            else
            {
                _data._external = new char[_size+1];
                memcpy(_data._external, from._data._external, _size+1);
            }
        }

        Address::Address(Address &&from)
            : _size{from._size}
        {
            if(from._size >= sizeof(_data._internal))
            {
                _data._external = new char[_size+1];
                memcpy(_data._external, from._data._external, _size+1);
                delete from._data._external;
            }
            else
            {
                memcpy(&_data, &from._data, sizeof(from._data));
            }
            from._size = 0;
        }

        Address::Address(const char *from, const char *end)
            : _size{static_cast<std::uint16_t>(end ? (end-from) : strlen(from))}
        {
            assert(65535 > strlen(from));

            if(_size <= sizeof(_data._internal)-1)
            {
                memcpy(_data._internal, from, _size+1);
            }
            else
            {
                _data._external = new char[_size+1];
                memcpy(_data._external, from, _size);
                _data._external[_size] = 0;
            }
        }

        Address::Address(const std::string &from)
            : _size{static_cast<std::uint16_t>(from.size())}
        {
            assert(65535 > from.size());

            if(_size <= sizeof(_data._internal)-1)
            {
                memcpy(_data._internal, from.c_str(), _size+1);
            }
            else
            {
                _data._external = new char[_size+1];
                memcpy(_data._external, from.c_str(), _size+1);
            }
        }

        Address::~Address()
        {
            if(_size >= sizeof(_data._internal))
            {
                delete _data._external;
            }
            _size = 0;
        }

        Address &Address::operator=(const Address &from)
        {
            if(_size >= sizeof(_data._internal))
            {
                delete _data._external;
            }

            _size = from._size;

            if(_size <= sizeof(_data._internal)-1)
            {
                memcpy(_data._internal, from._data._internal, _size+1);
            }
            else
            {
                _data._external = new char[_size+1];
                memcpy(_data._external, from._data._external, _size+1);
            }

            return *this;
        }

        Address &Address::operator=(Address &&from)
        {
            _size = from._size;

            if(_size <= sizeof(_data._internal)-1)
            {
                memcpy(_data._internal, from._data._internal, _size+1);
            }
            else
            {
                _data._external = from._data._external;
            }

            from._size = 0;

            return *this;
        }

        std::size_t Address::size() const
        {
            return _size;
        }

        const char *Address::csz() const
        {
            if(_size <= sizeof(_data._internal)-1)
            {
                return _data._internal;
            }

            return _data._external;
        }

        std::string Address::string() const
        {
            return std::string(csz(), size());
        }
    }


    Endpoint::Endpoint()
        : _scope{Scope::null}
    {
    }

    Endpoint::Endpoint(const Endpoint &from)
        : _scope{from._scope}
        , _port{from._port}
    {
        switch(_scope)
        {
        default:
            break;
        case Scope::ipc:
        case Scope::domainName:
        case Scope::ip4_domainName:
        case Scope::ip6_domainName:
            {
                new(&addressStr()) str::Address(from.addressStr());
            }
            break;
        case Scope::ip4:
            {
                new(&addressIp4()) ip4::Address(from.addressIp4());
            }
            break;
        case Scope::ip6:
            {
                new(&addressIp6()) ip6::Address(from.addressIp6());
            }
            break;
        }
    }

    Endpoint::Endpoint(Endpoint &&from)
        : _scope{from._scope}
        , _port{from._port}
    {
        switch(_scope)
        {
        default:
            break;
        case Scope::ipc:
        case Scope::domainName:
        case Scope::ip4_domainName:
        case Scope::ip6_domainName:
            {
                new(&addressStr()) str::Address(std::move(from.addressStr()));
            }
            break;
        case Scope::ip4:
            {
                new(&addressIp4()) ip4::Address(std::move(from.addressIp4()));
            }
            break;
        case Scope::ip6:
            {
                new(&addressIp6()) ip6::Address(std::move(from.addressIp6()));
            }
            break;
        }
    }

    Endpoint::Endpoint(const char *str, const char *end)
    {
        namespace qi = boost::spirit::qi;
        namespace ascii = boost::spirit::ascii;


        //scope
        /*
         * ip
         * ip4
         * ip6
         * ipc
         */

        bool allow_ip4 = false;
        bool allow_ip6 = false;
        bool allow_ipc = false;

        auto scope =
                (
                    qi::lit("ip") >>
                    (
                        qi::lit("c://")[([&](){allow_ipc=true;})] |
                        qi::lit("6://")[([&](){allow_ip6=true;})] |
                        qi::lit("4://")[([&](){allow_ip4=true;})] |
                        qi::lit("://") [([&](){allow_ip6=allow_ip4=true;})]
                    )
                );

        if(!qi::parse(str, end, scope))
        {
            allow_ip4 = true;
            allow_ip6 = true;
            allow_ipc = true;
        }



        ip4::Address ip4Address;
        bool isIp4Address = allow_ip4 && ip4Address.parse(str, end);


        ip6::Address ip6Address;
        bool isIp6Address = allow_ip6 && !isIp4Address && ip6Address.parse(str, end);

        const char *strAddressBegin;
        const char *strAddressEnd;
        if(!qi::parse(str, end,
                      qi::raw[+ (qi::char_ - ':')][([&](boost::iterator_range<const char *>range){strAddressBegin = range.begin(); strAddressEnd = range.end();})]))
        {
            strAddressBegin = end;
            strAddressEnd = end;
        }

        unsigned port;
        bool has_port = qi::parse(str, end,
                                  ':' >> qi::uint_[([&](unsigned v){port=v;})]);


        if(isIp6Address)
        {
            _scope = Scope::ip6;

            new(&addressIp6()) ip6::Address(ip6Address);
        }
        else if(isIp4Address)
        {
            _scope = Scope::ip4;

            new(&addressIp4()) ip4::Address(ip4Address);
        }
        else if(allow_ip6 && allow_ip4)
        {
            _scope = Scope::domainName;

            new(&addressStr()) str::Address(strAddressBegin, strAddressEnd);
        }
        else if(allow_ip6)
        {
            _scope = Scope::ip6_domainName;

            new(&addressStr()) str::Address(strAddressBegin, strAddressEnd);
        }
        else if(allow_ip4)
        {
            _scope = Scope::ip4_domainName;

            new(&addressStr()) str::Address(strAddressBegin, strAddressEnd);
        }
        else if(allow_ipc)
        {
            _scope = Scope::ipc;

            new(&addressStr()) str::Address(strAddressBegin, strAddressEnd);
        }
        else
        {
            _scope = Scope::null;
        }

        if(has_port)
        {
            _port = port;
        }
        else
        {
            _port = 0;
        }
    }

    Endpoint::Endpoint(const char *str)
        : Endpoint{str, str+strlen(str)}
    {
    }

    Endpoint::Endpoint(const std::string &str)
        : Endpoint{str.c_str(), str.c_str()+str.size()}
    {
    }

    Endpoint::Endpoint(const ip4::Address &address, std::uint16_t port)
        : _scope(Scope::ip4)
        , _port(port)
    {
        new(&addressIp4()) ip4::Address(address);
    }

    Endpoint::Endpoint(const ip6::Address &address, std::uint16_t port)
        : _scope(Scope::ip6)
        , _port(port)
    {
        new(&addressIp6()) ip6::Address(address);
    }

    Endpoint::Endpoint(const str::Address &address, uint16_t port)
        : _scope(Scope::domainName)
        , _port(port)
    {
        new(&addressStr()) str::Address(address);
    }

    Endpoint::~Endpoint()
    {
        switch(_scope)
        {
        default:
            break;

        case Scope::ip6:
            addressIp6().~Address();
            break;

        case Scope::ip4:
            addressIp4().~Address();
            break;

        case Scope::ipc:
        case Scope::domainName:
        case Scope::ip4_domainName:
        case Scope::ip6_domainName:
            addressStr().~Address();
            break;
        }
    }

    Endpoint &Endpoint::operator=(const Endpoint &from)
    {
        switch(_scope)
        {
        default:
            break;

        case Scope::ip6:
            addressIp6().~Address();
            break;

        case Scope::ip4:
            addressIp4().~Address();
            break;

        case Scope::ipc:
        case Scope::domainName:
        case Scope::ip4_domainName:
        case Scope::ip6_domainName:
            addressStr().~Address();
            break;
        }

        _scope = from._scope;
        _port = from._port;

        switch(_scope)
        {
        default:
            break;
        case Scope::ipc:
        case Scope::domainName:
        case Scope::ip4_domainName:
        case Scope::ip6_domainName:
            {
                new(&addressStr()) str::Address(from.addressStr());
            }
            break;
        case Scope::ip4:
            {
                new(&addressIp4()) ip4::Address(from.addressIp4());
            }
            break;
        case Scope::ip6:
            {
                new(&addressIp6()) ip6::Address(from.addressIp6());
            }
            break;
        }

        return *this;
    }

    Endpoint &Endpoint::operator=(Endpoint &&from)
    {
        std::swap(_address, from._address);
        std::swap(_scope, from._scope);
        std::swap(_port, from._port);

        return *this;
    }

    Scope Endpoint::scope() const
    {
        return _scope;
    }

    const ip4::Address &Endpoint::addressIp4() const
    {
        assert(Scope::ip4 == _scope);
        if(Scope::ip4 != _scope)
        {
            abort();
        }

        return *(ip4::Address *)&_address;
    }

    const ip6::Address &Endpoint::addressIp6() const
    {
        assert(Scope::ip6 == _scope);
        if(Scope::ip6 != _scope)
        {
            abort();
        }

        return *(ip6::Address *)&_address;
    }

    const str::Address &Endpoint::addressStr() const
    {
        assert(
                    Scope::ipc == _scope ||
                    Scope::domainName == _scope ||
                    Scope::ip4_domainName == _scope ||
                    Scope::ip6_domainName == _scope);

        if(Scope::ipc != _scope &&
           Scope::domainName != _scope &&
           Scope::ip4_domainName != _scope &&
           Scope::ip6_domainName != _scope)
        {
            abort();
        }

        return *(str::Address *)&_address;
    }

    std::uint16_t Endpoint::port() const
    {
        return _port;
    }

    std::string Endpoint::asString() const
    {
        std::string res;

        switch(_scope)
        {
        case Scope::null:
            return res;

        case Scope::ipc:
            res.append("ipc://");
            res.append(addressStr().csz(), addressStr().size());
            return res;

        case Scope::domainName:
            res.append("ip://");
            res.append(addressStr().csz(), addressStr().size());
            res.append(":");
            res += std::to_string(_port);
            return res;

        case Scope::ip4_domainName:
            res.append("ip4://");
            res.append(addressStr().csz(), addressStr().size());
            res.append(":");
            res += std::to_string(_port);
            return res;

        case Scope::ip4:
            res.append("ip4://");
            res += addressIp4().asString();
            res.append(":");
            res += std::to_string(_port);
            return res;

        case Scope::ip6_domainName:
            res.append("ip6://");
            res.append(addressStr().csz(), addressStr().size());
            res.append(":");
            res += std::to_string(_port);
            return res;

        case Scope::ip6:
            res.append("ip6://");
            res += addressIp6().asString();
            res.append(":");
            res += std::to_string(_port);
            return res;
        }

        return res;
    }

    ip4::Address &Endpoint::addressIp4()
    {
        assert(Scope::ip4 == _scope);
        if(Scope::ip4 != _scope)
        {
            abort();
        }

        return *(ip4::Address *)&_address;
    }

    ip6::Address &Endpoint::addressIp6()
    {
        assert(Scope::ip6 == _scope);
        if(Scope::ip6 != _scope)
        {
            abort();
        }

        return *(ip6::Address *)&_address;
    }

    str::Address &Endpoint::addressStr()
    {
        assert(
                    Scope::ipc == _scope ||
                    Scope::domainName == _scope ||
                    Scope::ip4_domainName == _scope ||
                    Scope::ip6_domainName == _scope);

        if(Scope::ipc != _scope &&
           Scope::domainName != _scope &&
           Scope::ip4_domainName != _scope &&
           Scope::ip6_domainName != _scope)
        {
            abort();
        }

        return *(str::Address *)&_address;
    }

}}
