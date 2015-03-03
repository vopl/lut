#pragma once

#include "lut/coupling/meta/alias.hpp"
#include "lut/coupling/meta/array.hpp"
#include "lut/coupling/meta/enum.hpp"
#include "lut/coupling/meta/iface.hpp"
#include "lut/coupling/meta/list.hpp"
#include "lut/coupling/meta/map.hpp"
#include "lut/coupling/meta/ptr.hpp"
#include "lut/coupling/meta/primitive.hpp"
#include "lut/coupling/meta/scope.hpp"
#include "lut/coupling/meta/set.hpp"
#include "lut/coupling/meta/struct.hpp"
#include "lut/coupling/meta/variant.hpp"

#include <string>
#include <system_error>

namespace lut { namespace coupling { namespace meta
{
    struct CommitError
    {
        std::error_code _ec;
        std::string     _comment;

        std::string toString() const;
    };


    class LibraryBuilder
        : public himpl::FaceLayout<impl::LibraryBuilder>
    {
    public:
        LibraryBuilder();
        ~LibraryBuilder();

    public:// changes
        //LoadResult load(const std::string &file);
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
        bool commitChanges(Library &lib, std::vector<CommitError> &errors);
        void rollbackChanges();

    };
}}}
