#pragma once


namespace Black
{
inline namespace DataOriented
{
inline namespace Composition
{
namespace Internal
{
	// Compile-time representation for memory chunk information.
	template< size_t CHUNK_SIZE, size_t CHUNK_ALIGNMENT, size_t CHUNK_OFFSET >
	struct MemoryChunkInfo final
	{
		// Specified size of chunk.
		static constexpr const size_t SIZE		= CHUNK_SIZE;

		// Specified alignment of chunk.
		static constexpr const size_t ALIGNMENT	= CHUNK_ALIGNMENT;

		// Specified internal offset of chunk.
		static constexpr const size_t OFFSET	= CHUNK_OFFSET;
	};

	// Component indexing helper. Used to associate the component type with ordinal index in list.
	template< typename TComponent, typename TList, size_t BASE_INDEX >
	struct ComponentIndexHelper;

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

	// Deduction branch. Looking for the component in list.
	template< typename TComponent, size_t BASE_INDEX, typename TCurrent, typename... TRest >
	struct ComponentIndexHelper<TComponent, TypesCollection<TCurrent, TRest...>, BASE_INDEX> final
	{
		using NextHelper = typename ComponentIndexHelper<TComponent, typename TypesCollection<TRest...>, BASE_INDEX + 1>;

		static constexpr const size_t Find()	{ return NextHelper::Find(); };
	};

	// Helps to unpack any inner packed collections into single top-ranked collection.
	template< typename TBasicCollection, typename... TRawComponents >
	struct CollectionUnpackHelper;

	// Deduction branch. Just check the type and pack it directly.
	template< typename... TRefinedComponents, typename TRawHead, typename... TRawRest >
	struct CollectionUnpackHelper<TypesCollection<TRefinedComponents...>, TRawHead, TRawRest...>
		: CollectionUnpackHelper<TypesCollection<TRefinedComponents..., TRawHead>, TRawRest...>
	{
		static_assert( !Black::IS_CONST<TRawHead>, "The type of component should not be constant." );
		static_assert( !Black::IS_REFERENCE<TRawHead>, "The type of component should not be reference." );
		static_assert( !Black::IS_POINTER<TRawHead>, "The type of component should not be pointer." );
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
		using Components = TypesCollection<TRefinedComponents..., TRawComponent>;
	};
}
}
}
}
