#include "lut/stable.hpp"
#include "lut/coupling/meta/impl/libraryBuilder.hpp"

#include "lut/coupling/meta/impl/scope.hpp"

namespace lut { namespace coupling { namespace meta { namespace impl
{
    LibraryBuilder::LibraryBuilder()
    {
    }

    LibraryBuilder::~LibraryBuilder()
    {
        rollbackChanges();
    }

    void LibraryBuilder::merge(const Library &lib)
    {
        assert(0);
    }

    Primitive *LibraryBuilder::addPrimitive()
    {
        return create<Primitive>();
    }

    List *LibraryBuilder::addList()
    {
        return create<List>();
    }

    Set *LibraryBuilder::addSet()
    {
        return create<Set>();
    }

    Map *LibraryBuilder::addMap()
    {
        return create<Map>();
    }

    Ptr *LibraryBuilder::addPtr()
    {
        return create<Ptr>();
    }

    Array *LibraryBuilder::addArray()
    {
        return create<Array>();
    }

    Scope *LibraryBuilder::addScope(Scope *parent, const std::string &name)
    {
        auto res = create<Scope>();
        res->setName(name);

        if(parent)
        {
            checkPresense(parent);
            parent->add(res);
        }

        return res;
    }

    Alias *LibraryBuilder::addAlias(Scope *parent, const std::string &name)
    {
        auto res = create<Alias>();
        res->setName(name);

        checkPresense(parent);
        parent->add(res);

        return res;
    }

    Struct *LibraryBuilder::addStruct(Scope *parent, const std::string &name)
    {
        auto res = create<Struct>();
        res->setName(name);

        checkPresense(parent);
        parent->add(static_cast<Type *>(res));
        parent->add(static_cast<Scope *>(res));

        return res;
    }

    Variant *LibraryBuilder::addVariant(Scope *parent, const std::string &name)
    {
        auto res = create<Variant>();
        res->setName(name);

        checkPresense(parent);
        parent->add(static_cast<Type *>(res));
        parent->add(static_cast<Scope *>(res));

        return res;
    }

    Enum *LibraryBuilder::addEnum(Scope *parent, const std::string &name)
    {
        auto res = create<Enum>();
        res->setName(name);

        checkPresense(parent);
        parent->add(res);

        return res;
    }

    Iface *LibraryBuilder::addIface(Scope *parent, const std::string &name)
    {
        auto res = create<Iface>();
        res->setName(name);

        checkPresense(parent);
        parent->add(static_cast<Type *>(res));
        parent->add(static_cast<Scope *>(res));

        return res;
    }

    Method *LibraryBuilder::addMethod(Iface *parent, const std::string &name)
    {
        auto res = create<Method>();
        res->setName(name);

        checkPresense(parent);
        parent->add(res);

        return res;
    }

    Attribute *LibraryBuilder::addAttribute(Struct *parent, const std::string &name)
    {
        auto res = create<Attribute>();
        res->setName(name);

        checkPresense(parent);
        parent->add(res);

        return res;
    }

    Attribute *LibraryBuilder::addAttribute(Variant *parent, const std::string &name)
    {
        auto res = create<Attribute>();
        res->setName(name);

        checkPresense(parent);
        parent->add(res);

        return res;
    }

    Attribute *LibraryBuilder::addAttribute(Method *parent, const std::string &name)
    {
        auto res = create<Attribute>();
        res->setName(name);

        checkPresense(parent);
        parent->add(res);

        return res;
    }

    EnumValue *LibraryBuilder::addEnumValue(Enum *parent, const std::string &name)
    {
        auto res = create<EnumValue>();
        res->setName(name);

        checkPresense(parent);
        parent->add(res);

        return res;
    }

    void LibraryBuilder::addBase(Variant *target, Variant *base)
    {
        checkPresense(target);
        target->addBase(base);
    }

    void LibraryBuilder::addBase(Struct *target, Struct *base)
    {
        checkPresense(target);
        target->addBase(base);
    }

    void LibraryBuilder::addBase(Enum *target, Enum *base)
    {
        checkPresense(target);
        target->addBase(base);
    }

    void LibraryBuilder::addBase(Iface *target, Iface *base)
    {
        checkPresense(target);
        target->addBase(base);
    }

    void LibraryBuilder::setType(Attribute *target, Type *type)
    {
        checkPresense(target);
        target->setValueType(type);
    }

    void LibraryBuilder::setType(Alias *target, Type *type)
    {
        checkPresense(target);
        target->setTarget(type);
    }

    void LibraryBuilder::setType(Method *target, Type *type)
    {
        checkPresense(target);
        target->setResultType(type);
    }

    void LibraryBuilder::setElementType(Set *target, Type *type)
    {
        checkPresense(target);
        target->setElementType(type);
    }

    void LibraryBuilder::setElementType(List *target, Type *type)
    {
        checkPresense(target);
        target->setElementType(type);
    }

    void LibraryBuilder::setElementType(Array *target, Type *type)
    {
        checkPresense(target);
        target->setElementType(type);
    }

    void LibraryBuilder::setElementType(Ptr *target, Type *type)
    {
        checkPresense(target);
        target->setElementType(type);
    }

    void LibraryBuilder::setElementType(Map *target, Type *keyType, Type *valueType)
    {
        checkPresense(target);
        target->setElementType1(keyType);
        target->setElementType2(valueType);
    }

    void LibraryBuilder::setMethodDirection(Method *target, CallDirection direction)
    {
        checkPresense(target);
        target->setDirection(direction);
    }

    void LibraryBuilder::setMethodNowait(Method *target, bool nowait)
    {
        checkPresense(target);
        target->setNowait(nowait);
    }

    void LibraryBuilder::setArraySize(Array *target, std::uint32_t size)
    {
        checkPresense(target);
        target->setSize(size);
    }

    bool LibraryBuilder::commitChanges(std::vector<CommitError> &errors)
    {
        assert(0);
    }

    void LibraryBuilder::rollbackChanges()
    {
        _lists.clear();
        _sets.clear();
        _maps.clear();
        _ptrs.clear();
        _arrays.clear();
        _primitives.clear();
        _scopes.clear();
        _aliases.clear();
        _structs.clear();
        _variants.clear();
        _enums.clear();
        _ifaces.clear();
        _methods.clear();
        _attributes.clear();
        _enumValues.clear();

        _holders.clear();
    }

    template <>
    LibraryBuilder::Items<List> &LibraryBuilder::items<List>()
    {
        return _lists;
    }

    template <>
    LibraryBuilder::Items<Set> &LibraryBuilder::items<Set>()
    {
        return _sets;
    }

    template <>
    LibraryBuilder::Items<Map> &LibraryBuilder::items<Map>()
    {
        return _maps;
    }

    template <>
    LibraryBuilder::Items<Ptr> &LibraryBuilder::items<Ptr>()
    {
        return _ptrs;
    }

    template <>
    LibraryBuilder::Items<Array> &LibraryBuilder::items<Array>()
    {
        return _arrays;
    }

    template <>
    LibraryBuilder::Items<Primitive> &LibraryBuilder::items<Primitive>()
    {
        return _primitives;
    }

    template <>
    LibraryBuilder::Items<Scope> &LibraryBuilder::items<Scope>()
    {
        return _scopes;
    }

    template <>
    LibraryBuilder::Items<Alias> &LibraryBuilder::items<Alias>()
    {
        return _aliases;
    }

    template <>
    LibraryBuilder::Items<Struct> &LibraryBuilder::items<Struct>()
    {
        return _structs;
    }

    template <>
    LibraryBuilder::Items<Variant> &LibraryBuilder::items<Variant>()
    {
        return _variants;
    }

    template <>
    LibraryBuilder::Items<Enum> &LibraryBuilder::items<Enum>()
    {
        return _enums;
    }

    template <>
    LibraryBuilder::Items<Iface> &LibraryBuilder::items<Iface>()
    {
        return _ifaces;
    }

    template <>
    LibraryBuilder::Items<Method> &LibraryBuilder::items<Method>()
    {
        return _methods;
    }

    template <>
    LibraryBuilder::Items<Attribute> &LibraryBuilder::items<Attribute>()
    {
        return _attributes;
    }

    template <>
    LibraryBuilder::Items<EnumValue> &LibraryBuilder::items<EnumValue>()
    {
        return _enumValues;
    }

    template <class T>
    T *LibraryBuilder::create()
    {
        T *res = new T;
        _holders.emplace(_holders.end(), res, [](void *p){delete static_cast<T *>(p);});

        locate(res);

        return res;
    }

    template <class T>
    void LibraryBuilder::locate(T *p)
    {
        items<T>().emplace(p);
    }

    template <>
    void LibraryBuilder::locate<Struct>(Struct *p)
    {
        items<Struct>().emplace(p);
        items<Scope>().emplace(p);
    }

    template <>
    void LibraryBuilder::locate<Variant>(Variant *p)
    {
        items<Variant>().emplace(p);
        items<Scope>().emplace(p);
    }

    template <>
    void LibraryBuilder::locate<Iface>(Iface *p)
    {
        items<Iface>().emplace(p);
        items<Scope>().emplace(p);
    }

    template <class T>
    void LibraryBuilder::checkPresense(T *p)
    {
        if(items<T>().end() == items<T>().find(p))
        {
            assert(!"requested object is not presented in builder");
            abort();
        }
    }


}}}}
