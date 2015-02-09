#include "lut/stable.hpp"
#include "lut/coupling/meta/impl/library.hpp"

namespace lut { namespace coupling { namespace meta { namespace impl
{
    Library::Library()
    {
    }

    Library::~Library()
    {
    }

    LoadResult Library::load(const std::string &file)
    {
        return LoadResult::malformedFile;
    }

    Primitive *Library::addPrimitive()
    {
        assert(0);
    }

    List *Library::addList()
    {
        assert(0);
    }

    Set *Library::addSet()
    {
        assert(0);
    }

    Map *Library::addMap()
    {
        assert(0);
    }

    Ptr *Library::addPtr()
    {
        assert(0);
    }

    Array *Library::addArray()
    {
        assert(0);
    }

    Scope *Library::addScope(Scope *parent, const std::string &name)
    {
        assert(0);
    }

    Alias *Library::addAlias(Scope *parent, const std::string &name)
    {
        assert(0);
    }

    Struct *Library::addStruct(Scope *parent, const std::string &name)
    {
        assert(0);
    }

    Variant *Library::addVariant(Scope *parent, const std::string &name)
    {
        assert(0);
    }

    Enum *Library::addEnum(Scope *parent, const std::string &name)
    {
        assert(0);
    }

    Iface *Library::addIface(Scope *parent, const std::string &name)
    {
        assert(0);
    }

    Method *Library::addMethod(Iface *parent, const std::string &name)
    {
        assert(0);
    }

    Attribute *Library::addAttribute(Struct *parent, const std::string &name)
    {
        assert(0);
    }

    Attribute *Library::addAttribute(Variant *parent, const std::string &name)
    {
        assert(0);
    }

    Attribute *Library::addAttribute(Method *parent, const std::string &name)
    {
        assert(0);
    }

    EnumValue *Library::addEnumValue(Enum *parent, const std::string &name)
    {
        assert(0);
    }

    void Library::addBase(Variant *target, Variant *base)
    {
        assert(0);
    }

    void Library::addBase(Struct *target, Struct *base)
    {
        assert(0);
    }

    void Library::addBase(Enum *target, Enum *base)
    {
        assert(0);
    }

    void Library::addBase(Iface *target, Iface *base)
    {
        assert(0);
    }

    void Library::setType(Attribute *target, Type *type)
    {
        assert(0);
    }

    void Library::setType(Alias *target, Type *type)
    {
        assert(0);
    }

    void Library::setType(Method *target, Type *type)
    {
        assert(0);
    }

    void Library::setElementType(Set *target, Type *type)
    {
        assert(0);
    }

    void Library::setElementType(List *target, Type *type)
    {
        assert(0);
    }

    void Library::setElementType(Array *target, Type *type)
    {
        assert(0);
    }

    void Library::setElementType(Ptr *target, Type *type)
    {
        assert(0);
    }

    void Library::setElementType(Map *target, Type *keyType, Type *valueType)
    {
        assert(0);
    }

    void Library::setMethodDirection(Method *target, CallDirection direction)
    {
        assert(0);
    }

    void Library::setMethodNowait(Method *target, bool nowait)
    {
        assert(0);
    }

    void Library::setArraySize(Array *target, std::uint32_t size)
    {
        assert(0);
    }

    bool Library::commitChanges(std::vector<LibraryCommitError> &errors)
    {
        assert(0);
    }

    void Library::rollbackChanges()
    {
        assert(0);
    }

}}}}
