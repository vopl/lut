#include "lut/stable.hpp"
#include "lut/coupling/meta/library.hpp"
#include "lut/coupling/meta/impl/library.hpp"

namespace lut { namespace coupling { namespace meta
{
    using namespace himpl;

    std::string LibraryCommitError::toString() const
    {
        if(!_ec)
        {
            return std::string();
        }

        if(_comment.empty())
        {
            return _ec.message();
        }

        return _ec.message() + ": " + _comment;
    }

    Library::Library()
        : FaceLayout<impl::Library>()
    {
    }

    Library::~Library()
    {
    }

    LoadResult Library::load(const std::string &file)
    {
        return impl().load(file);
    }

    Primitive *Library::addPrimitive()
    {
        return impl2Face<Primitive>(impl().addPrimitive());
    }

    List *Library::addList()
    {
        return impl2Face<List>(impl().addList());
    }

    Set *Library::addSet()
    {
        return impl2Face<Set>(impl().addSet());
    }

    Map *Library::addMap()
    {
        return impl2Face<Map>(impl().addMap());
    }

    Ptr *Library::addPtr()
    {
        return impl2Face<Ptr>(impl().addPtr());
    }

    Array *Library::addArray()
    {
        return impl2Face<Array>(impl().addArray());
    }

    Scope *Library::addScope(Scope *parent, const std::string &name)
    {
        return impl2Face<Scope>(impl().addScope(face2Impl(parent), name));
    }

    Alias *Library::addAlias(Scope *parent, const std::string &name)
    {
        return impl2Face<Alias>(impl().addAlias(face2Impl(parent), name));
    }

    Struct *Library::addStruct(Scope *parent, const std::string &name)
    {
        return impl2Face<Struct>(impl().addStruct(face2Impl(parent), name));
    }

    Variant *Library::addVariant(Scope *parent, const std::string &name)
    {
        return impl2Face<Variant>(impl().addVariant(face2Impl(parent), name));
    }

    Enum *Library::addEnum(Scope *parent, const std::string &name)
    {
        return impl2Face<Enum>(impl().addEnum(face2Impl(parent), name));
    }

    Iface *Library::addIface(Scope *parent, const std::string &name)
    {
        return impl2Face<Iface>(impl().addIface(face2Impl(parent), name));
    }

    Method *Library::addMethod(Iface *parent, const std::string &name)
    {
        return impl2Face<Method>(impl().addMethod(face2Impl(parent), name));
    }

    Attribute *Library::addAttribute(Struct *parent, const std::string &name)
    {
        return impl2Face<Attribute>(impl().addAttribute(face2Impl(parent), name));
    }

    Attribute *Library::addAttribute(Variant *parent, const std::string &name)
    {
        return impl2Face<Attribute>(impl().addAttribute(face2Impl(parent), name));
    }

    Attribute *Library::addAttribute(Method *parent, const std::string &name)
    {
        return impl2Face<Attribute>(impl().addAttribute(face2Impl(parent), name));
    }

    EnumValue *Library::addEnumValue(Enum *parent, const std::string &name)
    {
        return impl2Face<EnumValue>(impl().addEnumValue(face2Impl(parent), name));
    }

    void Library::addBase(Variant *target, Variant *base)
    {
        return impl().addBase(face2Impl(target), face2Impl(base));
    }

    void Library::addBase(Struct *target, Struct *base)
    {
        return impl().addBase(face2Impl(target), face2Impl(base));
    }

    void Library::addBase(Enum *target, Enum *base)
    {
        return impl().addBase(face2Impl(target), face2Impl(base));
    }

    void Library::addBase(Iface *target, Iface *base)
    {
        return impl().addBase(face2Impl(target), face2Impl(base));
    }

    void Library::setType(Attribute *target, Type *type)
    {
        return impl().setType(face2Impl(target), face2Impl(type));
    }

    void Library::setType(Alias *target, Type *type)
    {
        return impl().setType(face2Impl(target), face2Impl(type));
    }

    void Library::setType(Method *target, Type *type)
    {
        return impl().setType(face2Impl(target), face2Impl(type));
    }

    void Library::setElementType(Set *target, Type *type)
    {
        return impl().setElementType(face2Impl(target), face2Impl(type));
    }

    void Library::setElementType(List *target, Type *type)
    {
        return impl().setElementType(face2Impl(target), face2Impl(type));
    }

    void Library::setElementType(Array *target, Type *type)
    {
        return impl().setElementType(face2Impl(target), face2Impl(type));
    }

    void Library::setElementType(Ptr *target, Type *type)
    {
        return impl().setElementType(face2Impl(target), face2Impl(type));
    }

    void Library::setElementType(Map *target, Type *keyType, Type *valueType)
    {
        return impl().setElementType(face2Impl(target), face2Impl(keyType), face2Impl(valueType));
    }

    void Library::setMethodDirection(Method *target, CallDirection direction)
    {
        return impl().setMethodDirection(face2Impl(target), direction);
    }

    void Library::setMethodNowait(Method *target, bool nowait)
    {
        return impl().setMethodNowait(face2Impl(target), nowait);
    }

    void Library::setArraySize(Array *target, std::uint32_t size)
    {
        return impl().setArraySize(face2Impl(target), size);
    }

    bool Library::commitChanges(std::vector<LibraryCommitError> &errors)
    {
        return impl().commitChanges(errors);
    }

    void Library::rollbackChanges()
    {
        return impl().rollbackChanges();
    }

}}}
