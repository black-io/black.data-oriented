#pragma once


namespace Black
{
inline namespace DataOriented
{
inline namespace Composition
{
namespace Internal
{
	// Internal numeric collection for sizes and offsets.
	template< size_t... NUMBERS >
	using NumericCollection = Black::NumericCollection<size_t, NUMBERS...>;

	// Component indexing helper. Used to associate the component type with ordinal index in list.
	template< typename TComponent, typename TList, size_t BASE_INDEX >
	struct ComponentIndexHelper;

	// Deduction branch. Looking for the component in list.
	template< typename TComponent, size_t BASE_INDEX, typename TCurrent, typename... TRest >
	struct ComponentIndexHelper<TComponent, TypesCollection<TCurrent, TRest...>, BASE_INDEX> final
	{
		using NextHelper = typename ComponentIndexHelper<TComponent, typename TypesCollection<TRest...>, BASE_INDEX + 1>;

		static constexpr const size_t Find()	{ return NextHelper::Find(); };
	};

	// Terminal branch. The component is not found.
	template< typename TComponent, size_t BASE_INDEX >
	struct ComponentIndexHelper<TComponent, TypesCollection<>, BASE_INDEX> final
	{
		static constexpr const size_t Find()	{ return Black::UNDEFINED_INDEX; };
	};

	// Terminal branch. The component was found.
	template< typename TComponent, size_t BASE_INDEX, typename... TRest >
	struct ComponentIndexHelper<TComponent, TypesCollection<TComponent, TRest...>, BASE_INDEX> final
	{
		static constexpr const size_t Find()	{ return BASE_INDEX; };
	};

	// Helps to unpack any inner packed collections into single top-ranked collection.
	template< typename TBasicCollection, typename... TRawComponents >
	struct CollectionUnpackHelper;

	// Deduction branch. Just check the type and pack it directly.
	template< typename... TRefinedComponents, typename TRawHead, typename... TRawRest >
	struct CollectionUnpackHelper<TypesCollection<TRefinedComponents...>, TRawHead, TRawRest...>
		: CollectionUnpackHelper<TypesCollection<TRefinedComponents..., TRawHead>, TRawRest...>
	{
		static_assert( !Black::IS_CONST<TRawHead>,		"The type of component should not be constant." );
		static_assert( !Black::IS_REFERENCE<TRawHead>,	"The type of component should not be reference." );
		static_assert( !Black::IS_POINTER<TRawHead>,	"The type of component should not be pointer." );
	};

	// Unpacking branch. Unpack the collection into top-ranked one.
	template< typename... TRefinedComponents, typename... TInnerComponents, typename... TRawRest >
	struct CollectionUnpackHelper<TypesCollection<TRefinedComponents...>, TypesUnion<TInnerComponents...>, TRawRest...>
		: CollectionUnpackHelper<TypesCollection<TRefinedComponents...>, TInnerComponents..., TRawRest...>
	{

	};

	// Unpacking branch. Unpack the collection into top-ranked one.
	template< typename... TRefinedComponents, typename... TInnerComponents, typename... TRawRest >
	struct CollectionUnpackHelper<TypesCollection<TRefinedComponents...>, TypesCollection<TInnerComponents...>, TRawRest...>
		: CollectionUnpackHelper<TypesCollection<TRefinedComponents...>, TInnerComponents..., TRawRest...>
	{

	};

	// Terminal branch. The top-ranked collection is ready.
	template< typename... TRefinedComponents, typename TRawComponent >
	struct CollectionUnpackHelper<TypesCollection<TRefinedComponents...>, TRawComponent>
	{
		static_assert( !Black::IS_CONST<TRawComponent>,		"The type of component should not be constant." );
		static_assert( !Black::IS_REFERENCE<TRawComponent>,	"The type of component should not be reference." );
		static_assert( !Black::IS_POINTER<TRawComponent>,	"The type of component should not be pointer." );

		using Components = TypesCollection<TRefinedComponents..., TRawComponent>;
	};
}
}
}
}
