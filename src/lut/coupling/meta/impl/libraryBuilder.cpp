#include "lut/stable.hpp"
#include "lut/coupling/meta/impl/libraryBuilder.hpp"
#include "lut/coupling/meta/impl/library.hpp"

#include <type_traits>

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
        return _lc.create<Primitive>();
    }

    List *LibraryBuilder::addList()
    {
        return _lc.create<List>();
    }

    Set *LibraryBuilder::addSet()
    {
        return _lc.create<Set>();
    }

    Map *LibraryBuilder::addMap()
    {
        return _lc.create<Map>();
    }

    Ptr *LibraryBuilder::addPtr()
    {
        return _lc.create<Ptr>();
    }

    Array *LibraryBuilder::addArray()
    {
        return _lc.create<Array>();
    }

    Scope *LibraryBuilder::addScope(Scope *parent, const std::string &name)
    {
        auto res = _lc.create<Scope>();
        res->setName(name);

        if(parent)
        {
            _lc.checkPresense(parent);
            parent->add(res);
        }

        return res;
    }

    Alias *LibraryBuilder::addAlias(Scope *parent, const std::string &name)
    {
        auto res = _lc.create<Alias>();
        res->setName(name);

        _lc.checkPresense(parent);
        parent->add(res);

        return res;
    }

    Struct *LibraryBuilder::addStruct(Scope *parent, const std::string &name)
    {
        auto res = _lc.create<Struct>();
        res->setName(name);

        _lc.checkPresense(parent);
        parent->add(static_cast<Type *>(res));
        parent->add(static_cast<Scope *>(res));

        return res;
    }

    Variant *LibraryBuilder::addVariant(Scope *parent, const std::string &name)
    {
        auto res = _lc.create<Variant>();
        res->setName(name);

        _lc.checkPresense(parent);
        parent->add(static_cast<Type *>(res));
        parent->add(static_cast<Scope *>(res));

        return res;
    }

    Enum *LibraryBuilder::addEnum(Scope *parent, const std::string &name)
    {
        auto res = _lc.create<Enum>();
        res->setName(name);

        _lc.checkPresense(parent);
        parent->add(res);

        return res;
    }

    Iface *LibraryBuilder::addIface(Scope *parent, const std::string &name)
    {
        auto res = _lc.create<Iface>();
        res->setName(name);

        _lc.checkPresense(parent);
        parent->add(static_cast<Type *>(res));
        parent->add(static_cast<Scope *>(res));

        return res;
    }

    Method *LibraryBuilder::addMethod(Iface *parent, const std::string &name)
    {
        auto res = _lc.create<Method>();
        res->setName(name);

        _lc.checkPresense(parent);
        parent->add(res);

        return res;
    }

    Attribute *LibraryBuilder::addAttribute(Struct *parent, const std::string &name)
    {
        auto res = _lc.create<Attribute>();
        res->setName(name);

        _lc.checkPresense(parent);
        parent->add(res);

        return res;
    }

    Attribute *LibraryBuilder::addAttribute(Variant *parent, const std::string &name)
    {
        auto res = _lc.create<Attribute>();
        res->setName(name);

        _lc.checkPresense(parent);
        parent->add(res);

        return res;
    }

    Attribute *LibraryBuilder::addAttribute(Method *parent, const std::string &name)
    {
        auto res = _lc.create<Attribute>();
        res->setName(name);

        _lc.checkPresense(parent);
        parent->add(res);

        return res;
    }

    EnumValue *LibraryBuilder::addEnumValue(Enum *parent, const std::string &name)
    {
        auto res = _lc.create<EnumValue>();
        res->setName(name);

        _lc.checkPresense(parent);
        parent->add(res);

        return res;
    }

    void LibraryBuilder::addBase(Variant *target, Variant *base)
    {
        _lc.checkPresense(target);
        _lc.checkPresense(base);
        target->addBase(base);
    }

    void LibraryBuilder::addBase(Struct *target, Struct *base)
    {
        _lc.checkPresense(target);
        _lc.checkPresense(base);
        target->addBase(base);
    }

    void LibraryBuilder::addBase(Enum *target, Enum *base)
    {
        _lc.checkPresense(target);
        _lc.checkPresense(base);
        target->addBase(base);
    }

    void LibraryBuilder::addBase(Iface *target, Iface *base)
    {
        _lc.checkPresense(target);
        _lc.checkPresense(base);
        target->addBase(base);
    }

    void LibraryBuilder::setType(Attribute *target, Type *type)
    {
        _lc.checkPresense(target);
        _lc.checkPresense(type);
        target->setValueType(type);
    }

    void LibraryBuilder::setType(Alias *target, Type *type)
    {
        _lc.checkPresense(target);
        _lc.checkPresense(type);
        target->setTarget(type);
    }

    void LibraryBuilder::setType(Method *target, Type *type)
    {
        _lc.checkPresense(target);
        _lc.checkPresense(type);
        target->setResultType(type);
    }

    void LibraryBuilder::setElementType(Set *target, Type *type)
    {
        _lc.checkPresense(target);
        _lc.checkPresense(type);
        target->setElementType(type);
    }

    void LibraryBuilder::setElementType(List *target, Type *type)
    {
        _lc.checkPresense(target);
        _lc.checkPresense(type);
        target->setElementType(type);
    }

    void LibraryBuilder::setElementType(Array *target, Type *type)
    {
        _lc.checkPresense(target);
        _lc.checkPresense(type);
        target->setElementType(type);
    }

    void LibraryBuilder::setElementType(Ptr *target, Type *type)
    {
        _lc.checkPresense(target);
        _lc.checkPresense(type);
        target->setElementType(type);
    }

    void LibraryBuilder::setElementType(Map *target, Type *keyType, Type *valueType)
    {
        _lc.checkPresense(target);
        _lc.checkPresense(keyType);
        _lc.checkPresense(valueType);
        target->setElementType1(keyType);
        target->setElementType2(valueType);
    }

    void LibraryBuilder::setMethodDirection(Method *target, CallDirection direction)
    {
        _lc.checkPresense(target);
        target->setDirection(direction);
    }

    void LibraryBuilder::setMethodNowait(Method *target, bool nowait)
    {
        _lc.checkPresense(target);
        target->setNowait(nowait);
    }

    void LibraryBuilder::setArraySize(Array *target, std::uint32_t size)
    {
        _lc.checkPresense(target);
        target->setSize(size);
    }

    bool LibraryBuilder::commitChanges(Library &lib, std::vector<CommitError> &errors)
    {
        //TODO: check errors

        lib = std::move(_lc);
        _lc.clear();
        return true;
    }

    void LibraryBuilder::rollbackChanges()
    {
        _lc.clear();
    }

}}}}
