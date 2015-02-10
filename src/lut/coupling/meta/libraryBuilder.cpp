#include "lut/stable.hpp"
#include "lut/coupling/meta/libraryBuilder.hpp"
#include "lut/coupling/meta/impl/libraryBuilder.hpp"
#include "lut/coupling/meta/library.hpp"
#include "lut/coupling/meta/impl/library.hpp"

namespace lut { namespace coupling { namespace meta
{
    using namespace himpl;

    std::string CommitError::toString() const
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

    LibraryBuilder::LibraryBuilder()
        : FaceLayout<impl::LibraryBuilder>()
    {
    }

    LibraryBuilder::~LibraryBuilder()
    {
    }

    void LibraryBuilder::merge(const Library &lib)
    {
        return impl().merge(face2Impl(lib));
    }

    Primitive *LibraryBuilder::addPrimitive()
    {
        return impl2Face<Primitive>(impl().addPrimitive());
    }

    List *LibraryBuilder::addList()
    {
        return impl2Face<List>(impl().addList());
    }

    Set *LibraryBuilder::addSet()
    {
        return impl2Face<Set>(impl().addSet());
    }

    Map *LibraryBuilder::addMap()
    {
        return impl2Face<Map>(impl().addMap());
    }

    Ptr *LibraryBuilder::addPtr()
    {
        return impl2Face<Ptr>(impl().addPtr());
    }

    Array *LibraryBuilder::addArray()
    {
        return impl2Face<Array>(impl().addArray());
    }

    Scope *LibraryBuilder::addScope(Scope *parent, const std::string &name)
    {
        return impl2Face<Scope>(impl().addScope(face2Impl(parent), name));
    }

    Alias *LibraryBuilder::addAlias(Scope *parent, const std::string &name)
    {
        return impl2Face<Alias>(impl().addAlias(face2Impl(parent), name));
    }

    Struct *LibraryBuilder::addStruct(Scope *parent, const std::string &name)
    {
        return impl2Face<Struct>(impl().addStruct(face2Impl(parent), name));
    }

    Variant *LibraryBuilder::addVariant(Scope *parent, const std::string &name)
    {
        return impl2Face<Variant>(impl().addVariant(face2Impl(parent), name));
    }

    Enum *LibraryBuilder::addEnum(Scope *parent, const std::string &name)
    {
        return impl2Face<Enum>(impl().addEnum(face2Impl(parent), name));
    }

    Iface *LibraryBuilder::addIface(Scope *parent, const std::string &name)
    {
        return impl2Face<Iface>(impl().addIface(face2Impl(parent), name));
    }

    Method *LibraryBuilder::addMethod(Iface *parent, const std::string &name)
    {
        return impl2Face<Method>(impl().addMethod(face2Impl(parent), name));
    }

    Attribute *LibraryBuilder::addAttribute(Struct *parent, const std::string &name)
    {
        return impl2Face<Attribute>(impl().addAttribute(face2Impl(parent), name));
    }

    Attribute *LibraryBuilder::addAttribute(Variant *parent, const std::string &name)
    {
        return impl2Face<Attribute>(impl().addAttribute(face2Impl(parent), name));
    }

    Attribute *LibraryBuilder::addAttribute(Method *parent, const std::string &name)
    {
        return impl2Face<Attribute>(impl().addAttribute(face2Impl(parent), name));
    }

    EnumValue *LibraryBuilder::addEnumValue(Enum *parent, const std::string &name)
    {
        return impl2Face<EnumValue>(impl().addEnumValue(face2Impl(parent), name));
    }

    void LibraryBuilder::addBase(Variant *target, Variant *base)
    {
        return impl().addBase(face2Impl(target), face2Impl(base));
    }

    void LibraryBuilder::addBase(Struct *target, Struct *base)
    {
        return impl().addBase(face2Impl(target), face2Impl(base));
    }

    void LibraryBuilder::addBase(Enum *target, Enum *base)
    {
        return impl().addBase(face2Impl(target), face2Impl(base));
    }

    void LibraryBuilder::addBase(Iface *target, Iface *base)
    {
        return impl().addBase(face2Impl(target), face2Impl(base));
    }

    void LibraryBuilder::setType(Attribute *target, Type *type)
    {
        return impl().setType(face2Impl(target), face2Impl(type));
    }

    void LibraryBuilder::setType(Alias *target, Type *type)
    {
        return impl().setType(face2Impl(target), face2Impl(type));
    }

    void LibraryBuilder::setType(Method *target, Type *type)
    {
        return impl().setType(face2Impl(target), face2Impl(type));
    }

    void LibraryBuilder::setElementType(Set *target, Type *type)
    {
        return impl().setElementType(face2Impl(target), face2Impl(type));
    }

    void LibraryBuilder::setElementType(List *target, Type *type)
    {
        return impl().setElementType(face2Impl(target), face2Impl(type));
    }

    void LibraryBuilder::setElementType(Array *target, Type *type)
    {
        return impl().setElementType(face2Impl(target), face2Impl(type));
    }

    void LibraryBuilder::setElementType(Ptr *target, Type *type)
    {
        return impl().setElementType(face2Impl(target), face2Impl(type));
    }

    void LibraryBuilder::setElementType(Map *target, Type *keyType, Type *valueType)
    {
        return impl().setElementType(face2Impl(target), face2Impl(keyType), face2Impl(valueType));
    }

    void LibraryBuilder::setMethodDirection(Method *target, CallDirection direction)
    {
        return impl().setMethodDirection(face2Impl(target), direction);
    }

    void LibraryBuilder::setMethodNowait(Method *target, bool nowait)
    {
        return impl().setMethodNowait(face2Impl(target), nowait);
    }

    void LibraryBuilder::setArraySize(Array *target, std::uint32_t size)
    {
        return impl().setArraySize(face2Impl(target), size);
    }

    bool LibraryBuilder::commitChanges(std::vector<CommitError> &errors)
    {
        return impl().commitChanges(errors);
    }

    void LibraryBuilder::rollbackChanges()
    {
        return impl().rollbackChanges();
    }

}}}
