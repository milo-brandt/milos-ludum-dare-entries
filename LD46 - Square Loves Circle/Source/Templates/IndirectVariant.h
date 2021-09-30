#ifndef IndirectVariant_h
#define IndirectVariant_h

#include <array>
#include <memory>
#include <utility>
#include "TemplateUtility.h"

template<class... Types>
class IndirectVariant{ //Holds one of the given types or is empty.
    /* Relevant templates */
    static constexpr std::size_t variantCount = sizeof...(Types);
    template<std::size_t index, typename = std::enable_if_t<(index < variantCount)> >
    using NthType = NthParameter<index, Types...>;
    template<class Type>
    static constexpr bool appearsOnce = PackIncludesUnique<Type, Types...>;
    template<class Type, class = std::enable_if_t<appearsOnce<Type> > >
    static constexpr std::size_t indexOf = PackIncludesUniqueIndex<Type, Types...>;
    struct __Data{
        std::size_t index;
        __Data(std::size_t index):index(index){} //Force this to be specified.
    protected:
        ~__Data() = default; //Ensure deletions are from subclasses.
    };
    template<std::size_t _index>
    struct __DataSpecific : __Data{
        static constexpr auto index = _index;
        NthType<index> data;
        template<class... Args>
        __DataSpecific(Args&&... args):__Data(index),data(std::forward<Args>(args)...){}
    };
    /*
     Tables for deletions and constructions
     */
    template<class Ret, class Fn>
    static Ret internalVisit(__Data* data, Fn&& fn){
        static constexpr auto CallTable = generateStaticArray<Ret(*)(__Data*, Fn&&), variantCount>([](auto arg) constexpr{
            constexpr std::size_t index = decltype(arg)::value;
            return [](__Data* data, Fn&& fn) -> Ret{
                return std::invoke(fn, (__DataSpecific<index>*)(data));
            };
        });
        return CallTable[data->index](data, std::forward<Fn>(fn));
    }
    static void deleter(__Data* data){
        internalVisit<void>(data, [](auto* ptr){ delete ptr; });
    }
    using DataPtr = std::unique_ptr<__Data, StaticFunction<&deleter> >;
    static DataPtr copy(DataPtr const& input){
        if(!input) return nullptr;
        else return internalVisit<DataPtr>(input.get(), [](auto* data) -> DataPtr{
            return DataPtr(new std::remove_reference_t<decltype(*data)>(data->data));
        });
    }
    DataPtr data;
public:
    IndirectVariant(){}
    IndirectVariant(std::nullptr_t){}
    template<std::size_t index, class... Args, typename = std::enable_if_t<(index < sizeof...(Types))  && std::is_constructible_v<NthType<index>, Args&&...> > >
    IndirectVariant(std::in_place_index_t<index>, Args&&... args):data(new __DataSpecific<index>(std::forward<Args>(args)...)){}
    template<class Type, class... Args, typename = std::enable_if_t<appearsOnce<Type> && std::is_constructible_v<Type, Args&&...> > >
    IndirectVariant(std::in_place_type_t<Type>, Args&&... args):IndirectVariant(std::in_place_index<indexOf<Type> >, std::forward<Args>(args)...){}
    template<class Type, typename = std::enable_if_t<appearsOnce<Type> > >
    IndirectVariant(Type const& value):IndirectVariant(std::in_place_type<Type>, value){}
    template<class Type, typename = std::enable_if_t<appearsOnce<Type> > >
    IndirectVariant(Type&& value):IndirectVariant(std::in_place_type<Type>, std::move(value)){}
    IndirectVariant(IndirectVariant&& other) = default;
    IndirectVariant(IndirectVariant const& other):data(copy(other.data)){}
    IndirectVariant& operator=(std::nullptr_t){ data = nullptr; return *this; }
    IndirectVariant& operator=(IndirectVariant&&) = default;
    IndirectVariant& operator=(IndirectVariant const& other){ data = copy(other.data); return *this; }
    template<class Type, typename = std::enable_if_t<appearsOnce<Type> > >
    IndirectVariant& operator=(Type const& other){
        data.reset(new __DataSpecific<indexOf<Type> >(other));
        return *this;
    }
    template<class Type, typename = std::enable_if_t<appearsOnce<Type> > >
    IndirectVariant& operator=(Type&& other){
        data.reset(new __DataSpecific<indexOf<Type> >(std::move(other)));
        return *this;
    }
    
    template<class Visitor, class Ret = std::common_type_t<std::invoke_result_t<Visitor, Types&>... > >
    Ret visitUpon(Visitor&& visitor)&{
        assert(data);
        return internalVisit<Ret>(data.get(), [&visitor](auto* data) -> Ret{
            return std::invoke(std::forward<Visitor>(visitor), data->data);
        });
    }
    template<class Visitor, class Ret = std::common_type_t<std::invoke_result_t<Visitor, Types const&>... > >
    Ret visitUpon(Visitor&& visitor) const&{
        assert(data);
        return internalVisit<Ret>(data.get(), [&visitor](auto const* data) -> Ret{
            return std::invoke(std::forward<Visitor>(visitor), data->data);
        });
    }
    template<class Visitor, class Ret = std::common_type_t<std::invoke_result_t<Visitor, Types&>... > >
    Ret visitUpon(Visitor&& visitor)&&{
        assert(data);
        return internalVisit<Ret>(data.get(), [&visitor](auto* data) -> Ret{
            return std::invoke(std::forward<Visitor>(visitor), std::move(data->data));
        });
    }
    bool hasValue() const{
        return data != nullptr;
    }
    explicit operator bool() const{ return hasValue(); }
    template<class Type, typename = std::enable_if_t<appearsOnce<Type> > >
    bool holdsAlternative() const{
        assert(data);
        return data->index == indexOf<Type>;
    }
    std::size_t index() const{
        assert(data); //Precondition
        return data->index;
    }
    template<std::size_t index>
    NthType<index>& get(){
        assert(data); //Precondition
        assert(data->index == index); //Precondition
        return ((__DataSpecific<index>*)data.get())->data;
    }
    template<std::size_t index>
    NthType<index> const& get() const{
        assert(data); //Precondition
        assert(data->index == index); //Precondition
        return ((__DataSpecific<index> const*)data.get())->data;
    }
    template<class Type, typename = std::enable_if_t<appearsOnce<Type> > >
    Type& get(){
        return get<indexOf<Type> >();
    }
    template<class Type, typename = std::enable_if_t<appearsOnce<Type> > >
    Type const& get() const{
        return get<indexOf<Type> >();
    }
};
template<class... Types>
class WeakVariant;
template<class... Types>
class SharedVariant{ //Holds one of the given types or is empty.
    /* Relevant templates */
    static constexpr std::size_t variantCount = sizeof...(Types);
    template<std::size_t index, typename = std::enable_if_t<(index < variantCount)> >
    using NthType = NthParameter<index, Types...>;
    template<class Type>
    static constexpr bool appearsOnce = PackIncludesUnique<Type, Types...>;
    template<class Type, class = std::enable_if_t<appearsOnce<Type> > >
    static constexpr std::size_t indexOf = PackIncludesUniqueIndex<Type, Types...>;
    struct __Data{
        std::size_t index;
        __Data(std::size_t index):index(index){} //Force this to be specified.
    protected:
        ~__Data() = default; //Ensure deletions are from subclasses.
    };
    template<std::size_t _index>
    struct __DataSpecific : __Data{
        static constexpr auto index = _index;
        NthType<index> data;
        template<class... Args>
        __DataSpecific(Args&&... args):__Data(index),data(std::forward<Args>(args)...){}
    };
    /*
     Tables for deletions and constructions
     */
    template<class Ret, class Fn>
    static Ret internalVisit(__Data* data, Fn&& fn){
        static constexpr auto CallTable = generateStaticArray<Ret(*)(__Data*, Fn&&), variantCount>([](auto arg) constexpr{
            constexpr std::size_t index = decltype(arg)::value;
            return [](__Data* data, Fn&& fn) -> Ret{
                return std::invoke(fn, (__DataSpecific<index>*)(data));
            };
        });
        return CallTable[data->index](data, std::forward<Fn>(fn));
    }
    static void deleter(__Data* data){
        internalVisit<void>(data, [](auto* ptr){ delete ptr; });
    }
    using DataPtr = std::shared_ptr<__Data>;
    static DataPtr copy(DataPtr const& input){
        if(!input) return nullptr;
        else return internalVisit<DataPtr>(input.get(), [](auto* data) -> DataPtr{
            return DataPtr(new std::remove_reference_t<decltype(*data)>(data->data), &deleter);
        });
    }
    DataPtr data;
    friend class WeakVariant<Types...>;
public:
    SharedVariant(){}
    SharedVariant(std::nullptr_t){}
    template<std::size_t index, class... Args, typename = std::enable_if_t<(index < sizeof...(Types))  && std::is_constructible_v<NthType<index>, Args&&...> > >
    SharedVariant(std::in_place_index_t<index>, Args&&... args):data(new __DataSpecific<index>(std::forward<Args>(args)...)){}
    template<class Type, class... Args, typename = std::enable_if_t<appearsOnce<Type> && std::is_constructible_v<Type, Args&&...> > >
    SharedVariant(std::in_place_type_t<Type>, Args&&... args):SharedVariant(std::in_place_index<indexOf<Type> >, std::forward<Args>(args)...){}
    template<class Type, typename = std::enable_if_t<appearsOnce<Type> > >
    SharedVariant(Type const& value):SharedVariant(std::in_place_type<Type>, value){}
    template<class Type, typename = std::enable_if_t<appearsOnce<Type> > >
    SharedVariant(Type&& value):SharedVariant(std::in_place_type<Type>, std::move(value)){}
    SharedVariant(SharedVariant&& other) = default;
    SharedVariant(SharedVariant const& other) = default;
    SharedVariant& operator=(std::nullptr_t){ data = nullptr; return *this; }
    SharedVariant& operator=(SharedVariant&&) = default;
    SharedVariant& operator=(SharedVariant const& other) = default;
    template<class Type, typename = std::enable_if_t<appearsOnce<Type> > >
    SharedVariant& operator=(Type const& other){
        data.reset(new __DataSpecific<indexOf<Type> >(other));
        return *this;
    }
    template<class Type, typename = std::enable_if_t<appearsOnce<Type> > >
    SharedVariant& operator=(Type&& other){
        data.reset(new __DataSpecific<indexOf<Type> >(std::move(other)));
        return *this;
    }
    
    template<class Visitor, class Ret = std::common_type_t<std::invoke_result_t<Visitor, Types&>... > >
    Ret visitUpon(Visitor&& visitor)&{
        assert(data);
        return internalVisit<Ret>(data.get(), [&visitor](auto* data) -> Ret{
            return std::invoke(std::forward<Visitor>(visitor), data->data);
        });
    }
    template<class Visitor, class Ret = std::common_type_t<std::invoke_result_t<Visitor, Types const&>... > >
    Ret visitUpon(Visitor&& visitor) const&{
        assert(data);
        return internalVisit<Ret>(data.get(), [&visitor](auto const* data) -> Ret{
            return std::invoke(std::forward<Visitor>(visitor), data->data);
        });
    }
    template<class Visitor, class Ret = std::common_type_t<std::invoke_result_t<Visitor, Types&>... > >
    Ret visitUpon(Visitor&& visitor)&&{
        assert(data);
        return internalVisit<Ret>(data.get(), [&visitor](auto* data) -> Ret{
            return std::invoke(std::forward<Visitor>(visitor), std::move(data->data));
        });
    }
    bool hasValue() const{
        return data != nullptr;
    }
    explicit operator bool() const{ return hasValue(); }
    template<class Type, typename = std::enable_if_t<appearsOnce<Type> > >
    bool holdsAlternative() const{
        assert(data);
        return data->index == indexOf<Type>;
    }
    std::size_t index() const{
        assert(data); //Precondition
        return data->index;
    }
    template<std::size_t index>
    NthType<index>& get() const{
        assert(data); //Precondition
        assert(data->index == index); //Precondition
        return ((__DataSpecific<index>*)data.get())->data;
    }
    template<class Type, typename = std::enable_if_t<appearsOnce<Type> > >
    Type& get() const{
        return get<indexOf<Type> >();
    }
    SharedVariant clone() const{
        SharedVariant ret;
        ret.data = copy(data);
        return ret;
    }
};
template<class... Types>
class WeakVariant{
    std::weak_ptr<typename SharedVariant<Types...>::__Data> data;
public:
    WeakVariant(){}
    WeakVariant(std::nullptr_t){}
    WeakVariant(SharedVariant<Types...> const& var):data(var.data){}
    SharedVariant<Types...> lock() const{
        SharedVariant<Types...> ret;
        ret.data = data.lock();
        return ret;
    }
};
template<class... Types>
WeakVariant(SharedVariant<Types...> const&) -> WeakVariant<Types...>;

#endif
