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
        return emplace<Primitive>();
    }

    List *LibraryBuilder::addList()
    {
        return emplace<List>();
    }

    Set *LibraryBuilder::addSet()
    {
        return emplace<Set>();
    }

    Map *LibraryBuilder::addMap()
    {
        return emplace<Map>();
    }

    Ptr *LibraryBuilder::addPtr()
    {
        return emplace<Ptr>();
    }

    Array *LibraryBuilder::addArray()
    {
        return emplace<Array>();
    }

    Scope *LibraryBuilder::addScope(Scope *parent, const std::string &name)
    {
        auto res = emplace<Scope>();
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
        auto res = emplace<Alias>();
        res->setName(name);

        checkPresense(parent);
        parent->add(res);

        return res;
    }

    Struct *LibraryBuilder::addStruct(Scope *parent, const std::string &name)
    {
        auto res = emplace<Struct>();
        res->setName(name);

        checkPresense(parent);
        parent->add(res);

        return res;
    }

    Variant *LibraryBuilder::addVariant(Scope *parent, const std::string &name)
    {
        auto res = emplace<Variant>();
        res->setName(name);

        checkPresense(parent);
        parent->add(res);

        return res;
    }

    Enum *LibraryBuilder::addEnum(Scope *parent, const std::string &name)
    {
        auto res = emplace<Enum>();
        res->setName(name);

        checkPresense(parent);
        parent->add(res);

        return res;
    }

    Iface *LibraryBuilder::addIface(Scope *parent, const std::string &name)
    {
        auto res = emplace<Iface>();
        res->setName(name);

        checkPresense(parent);
        parent->add(res);

        return res;
    }

    Method *LibraryBuilder::addMethod(Iface *parent, const std::string &name)
    {
        auto res = emplace<Method>();
        res->setName(name);

        checkPresense(parent);
        parent->add(res);

        return res;
    }

    Attribute *LibraryBuilder::addAttribute(Struct *parent, const std::string &name)
    {
        auto res = emplace<Attribute>();
        res->setName(name);

        checkPresense(parent);
        parent->add(res);

        return res;
    }

    Attribute *LibraryBuilder::addAttribute(Variant *parent, const std::string &name)
    {
        auto res = emplace<Attribute>();
        res->setName(name);

        checkPresense(parent);
        parent->add(res);

        return res;
    }

    Attribute *LibraryBuilder::addAttribute(Method *parent, const std::string &name)
    {
        auto res = emplace<Attribute>();
        res->setName(name);

        checkPresense(parent);
        parent->add(res);

        return res;
    }

    EnumValue *LibraryBuilder::addEnumValue(Enum *parent, const std::string &name)
    {
        auto res = emplace<EnumValue>();
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
        resetHolder<Scope>();
        resetHolder<Alias>();
        resetHolder<Struct>();
        resetHolder<Variant>();
        resetHolder<Enum>();
        resetHolder<Iface>();
        resetHolder<Method>();
        resetHolder<Attribute>();
        resetHolder<EnumValue>();
    }

    template <class T>
    LibraryBuilder::Holder<T> &LibraryBuilder::holder()
    {
        assert(0);
        abort();

        static Holder<T> stub;
        return stub;
    }

    template <>
    LibraryBuilder::Holder<Scope> &LibraryBuilder::holder<Scope>()
    {
        return _holderScope;
    }

    template <>
    LibraryBuilder::Holder<Alias> &LibraryBuilder::holder<Alias>()
    {
        return _holderAlias;
    }

    template <>
    LibraryBuilder::Holder<Struct> &LibraryBuilder::holder<Struct>()
    {
        return _holderStruct;
    }

    template <>
    LibraryBuilder::Holder<Variant> &LibraryBuilder::holder<Variant>()
    {
        return _holderVariant;
    }

    template <>
    LibraryBuilder::Holder<Enum> &LibraryBuilder::holder<Enum>()
    {
        return _holderEnum;
    }

    template <>
    LibraryBuilder::Holder<Iface> &LibraryBuilder::holder<Iface>()
    {
        return _holderIface;
    }

    template <>
    LibraryBuilder::Holder<Method> &LibraryBuilder::holder<Method>()
    {
        return _holderMethod;
    }

    template <>
    LibraryBuilder::Holder<Attribute> &LibraryBuilder::holder<Attribute>()
    {
        return _holderAttribute;
    }

    template <>
    LibraryBuilder::Holder<EnumValue> &LibraryBuilder::holder<EnumValue>()
    {
        return _holderEnumValue;
    }

    template <class T>
    void LibraryBuilder::resetHolder()
    {
        for(auto v : holder<T>())
        {
            delete v;
        }
        holder<T>().clear();
    }

    template <class T>
    T *LibraryBuilder::emplace()
    {
        T *res = new T;
        holder<T>().emplace(res);
        return res;
    }

    template <class T>
    void LibraryBuilder::checkPresense(T *p)
    {
        if(holder<T>().end() == holder<T>().find(p))
        {
            assert(!"requested object is not presented in builder");
            abort();
        }
    }


}}}}
