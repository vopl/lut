#pragma once

#include "lut/coupling/meta/impl/libraryContent.hpp"

#include "lut/coupling/meta/impl/alias.hpp"
#include "lut/coupling/meta/impl/array.hpp"
#include "lut/coupling/meta/impl/enum.hpp"
#include "lut/coupling/meta/impl/iface.hpp"
#include "lut/coupling/meta/impl/list.hpp"
#include "lut/coupling/meta/impl/map.hpp"
#include "lut/coupling/meta/impl/ptr.hpp"
#include "lut/coupling/meta/impl/primitive.hpp"
#include "lut/coupling/meta/impl/scope.hpp"
#include "lut/coupling/meta/impl/set.hpp"
#include "lut/coupling/meta/impl/struct.hpp"
#include "lut/coupling/meta/impl/variant.hpp"


#include <vector>
#include <set>
#include <memory>

namespace lut { namespace coupling { namespace meta { namespace impl
{
    class LibraryContent
    {
        LibraryContent(const LibraryContent &) = delete;
        void operator=(const LibraryContent &) = delete;

    public:
        LibraryContent();
        LibraryContent(LibraryContent &&from);
        ~LibraryContent();

        LibraryContent &operator=(LibraryContent &&from);

    public:
        template <class T>
        using Items = std::set<T *>;

        template <class T>
        T *create();

        template <class T>
        void locate(T *p);

        template <class T>
        Items<T> &items();

        template <class T>
        void checkPresense(T *p);

        void clear();

    private:
        using Holder = std::unique_ptr<void, void(*)(void*)>;
        std::vector<Holder> _holders;

        Items<Type>         _types;
        Items<List>         _lists;
        Items<Set>          _sets;
        Items<Map>          _maps;
        Items<Ptr>          _ptrs;
        Items<Array>        _arrays;
        Items<Primitive>    _primitives;
        Items<Scope>        _scopes;
        Items<Alias>        _aliases;
        Items<Struct>       _structs;
        Items<Variant>      _variants;
        Items<Enum>         _enums;
        Items<Iface>        _ifaces;
        Items<Method>       _methods;
        Items<Attribute>    _attributes;
        Items<EnumValue>    _enumValues;

    };

    template <class T>
    T *LibraryContent::create()
    {
        T *res = new T;
        _holders.emplace(_holders.end(), res, [](void *p){delete static_cast<T *>(p);});

        locate(res);

        return res;
    }

    namespace
    {
        template <class T, class TItems>
        typename std::enable_if<std::is_convertible<T*, typename TItems::value_type>::value>::type locateConvertible(T *p, TItems &items)
        {
            items.emplace(p);
        }

        template <class T, class TItems>
        typename std::enable_if<!std::is_convertible<T*, typename TItems::value_type>::value>::type locateConvertible(T *, TItems &)
        {
        }
    }

    template <class T>
    void LibraryContent::locate(T *p)
    {
        locateConvertible(p, items<Type>());
        locateConvertible(p, items<List>());
        locateConvertible(p, items<Set>());
        locateConvertible(p, items<Map>());
        locateConvertible(p, items<Ptr>());
        locateConvertible(p, items<Array>());
        locateConvertible(p, items<Primitive>());
        locateConvertible(p, items<Scope>());
        locateConvertible(p, items<Alias>());
        locateConvertible(p, items<Struct>());
        locateConvertible(p, items<Variant>());
        locateConvertible(p, items<Enum>());
        locateConvertible(p, items<Iface>());
        locateConvertible(p, items<Method>());
        locateConvertible(p, items<Attribute>());
        locateConvertible(p, items<EnumValue>());
    }

    template <class T>
    void LibraryContent::checkPresense(T *p)
    {
        if(items<T>().end() == items<T>().find(p))
        {
            assert(!"requested object is not presented in builder");
            abort();
        }
    }

}}}}
