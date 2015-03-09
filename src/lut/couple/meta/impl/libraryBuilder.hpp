#pragma once

#include "lut/himpl/implLayout.hpp"
#include "lut/couple/meta/libraryBuilder.hpp"
#include "lut/couple/meta/impl/libraryContent.hpp"

namespace lut { namespace couple { namespace meta { namespace impl
{
    class LibraryBuilder
        : public himpl::ImplLayout<LibraryBuilder>
    {
    public:
        LibraryBuilder();
        ~LibraryBuilder();

    public:
        void merge(const Library &lib);

    public:// changes
        Primitive   *addPrimitive();
        List        *addList();
        Set         *addSet();
        Map         *addMap();
        Ptr         *addPtr();
        Array       *addArray();

        Scope       *addScope(Scope *parent, const std::string &name);
        Alias       *addAlias(Scope *parent, const std::string &name);
        Struct      *addStruct(Scope *parent, const std::string &name);
        Variant     *addVariant(Scope *parent, const std::string &name);
        Enum        *addEnum(Scope *parent, const std::string &name);
        Iface       *addIface(Scope *parent, const std::string &name);

        Method      *addMethod(Iface *parent, const std::string &name);

        Attribute   *addAttribute(Struct *parent, const std::string &name);
        Attribute   *addAttribute(Variant *parent, const std::string &name);
        Attribute   *addAttribute(Method *parent, const std::string &name);
        EnumValue   *addEnumValue(Enum *parent, const std::string &name);

        void addBase(Variant *target, Variant *base);
        void addBase(Struct *target, Struct *base);
        void addBase(Enum *target, Enum *base);
        void addBase(Iface *target, Iface *base);

        void setType(Attribute *target, Type *type);
        void setType(Alias *target, Type *type);
        void setType(Method *target, Type *type);

        void setElementType(Set *target, Type *type);
        void setElementType(List *target, Type *type);
        void setElementType(Array *target, Type *type);
        void setElementType(Ptr *target, Type *type);

        void setElementType(Map *target, Type *keyType, Type *valueType);

        void setMethodDirection(Method *target, CallDirection direction);
        void setMethodNowait(Method *target, bool nowait);

        void setArraySize(Array *target, std::uint32_t size);

    public:// commit
        bool commitChanges(Library &lib, std::vector<meta::CommitError> &errors);
        void rollbackChanges();

    private:
        LibraryContent _lc;
    };

}}}}
