#include "lut/stable.hpp"
#include "lut/coupling/meta/library.hpp"

namespace lut { namespace coupling { namespace meta
{
    LoadResult Library::load(const std::string &file)
    {
        //TODO serialize to internal binary form
        return LoadResult::malformedFile;
    }

    Primitive *Library::addPrimitive()
    {
        return nullptr;
    }

    List *Library::addList()
    {
        return nullptr;
    }

    Set *Library::addSet()
    {
        return nullptr;
    }

    Map *Library::addMap()
    {
        return nullptr;
    }

    Ptr *Library::addPtr()
    {
        return nullptr;
    }

    Array *Library::addArray()
    {
        return nullptr;
    }

    Scope *Library::addScope(Scope *parent, const std::string &name)
    {
        return nullptr;
    }

    Alias *Library::addAlias(Scope *parent, const std::string &name)
    {
        return nullptr;
    }

    Struct *Library::addStruct(Scope *parent, const std::string &name)
    {
        return nullptr;
    }

    Variant *Library::addVariant(Scope *parent, const std::string &name)
    {
        return nullptr;
    }

    Enum *Library::addEnum(Scope *parent, const std::string &name)
    {
        return nullptr;
    }

    Iface *Library::addIface(Scope *parent, const std::string &name)
    {
        return nullptr;
    }

    Method *Library::addMethod(Iface *parent, const std::string &name)
    {
        return nullptr;
    }

    Attribute *Library::addAttribute(Struct *parent, const std::string &name)
    {
        return nullptr;
    }

    Attribute *Library::addAttribute(Variant *parent, const std::string &name)
    {
        return nullptr;
    }

    Attribute *Library::addAttribute(Method *parent, const std::string &name)
    {
        return nullptr;
    }

    EnumValue *Library::addEnumValue(Enum *parent, const std::string &name)
    {
        return nullptr;
    }

    void Library::addBase(Variant *target, Variant *base)
    {
    }

    void Library::addBase(Struct *target, Struct *base)
    {
    }

    void Library::addBase(Enum *target, Enum *base)
    {
    }

    void Library::addBase(Iface *target, Iface *base)
    {
    }

    void Library::setType(Attribute *target, Type *type)
    {
    }

    void Library::setType(Alias *target, Type *type)
    {
    }

    void Library::setType(Method *target, Type *type)
    {
    }

    void Library::setElementType(Set *target, Type *type)
    {
    }

    void Library::setElementType(List *target, Type *type)
    {
    }

    void Library::setElementType(Array *target, Type *type)
    {
    }

    void Library::setElementType(Ptr *target, Type *type)
    {
    }

    void Library::setElementType(Map *target, Type *keyType, Type *valueType)
    {
    }

    void Library::setMethodDirection(Method *target, CallDirection direction)
    {
    }

    void Library::setMethodNowait(Method *target, bool nowait)
    {
    }

    void Library::setArraySize(Array *target, std::uint32_t size)
    {
    }


    bool Library::resolve()
    {
        assert(0);
    }

}}}
