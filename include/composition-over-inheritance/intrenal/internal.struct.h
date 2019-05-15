#pragma once


namespace Black
{
inline namespace DataOriented
{
inline namespace CompositionOverInheritance
{
namespace Internal
{
	// Internal numeric collection for sizes and offsets.
	template< size_t... NUMBERS >
	using NumericCollection = Black::NumericCollection<size_t, NUMBERS...>;

	// Forward declaration for TypesCollection statistics.
	template< typename... TTypes >
	struct TypesCollectionStats;

	// Forward declaration for TypesUnion statistics.
	template< typename... TTypes >
	struct TypesUnionStats;

	// Statistics for regular type.
	template< typename TType >
	struct TypeStats final
	{
		static_assert( !Black::IS_VOID<TType>,			"Type should not be void." );
		static_assert( !Black::IS_CONST<TType>,			"Type should not be constant." );
		static_assert( !Black::IS_REFERENCE<TType>,		"Type should not be reference." );
		static_assert( !Black::IS_POINTER<TType>,		"Type should not be pointer." );


		// Size of type.
		static constexpr const size_t SIZE		= sizeof( TType );

		// Alignment of type.
		static constexpr const size_t ALIGNMENT	= alignof( TType );
	};

	// Deduction branch. Collection complex stats.
	template< typename... TTypes >
	struct TypeStats<Black::TypesCollection<TTypes...>> final : TypesCollectionStats<TTypes...>
	{

	};

	// Deduction branch. Union complex stats.
	template< typename... TTypes >
	struct TypeStats<Black::TypesUnion<TTypes...>> final : TypesUnionStats<TTypes...>
	{

	};

	// Type offset statistics.
	template< typename TType, size_t BASE_OFFSET >
	struct TypeOffset final
	{
		// Offset for entity of given type.
		static constexpr const size_t OFFSET		= Black::GetAlignedSize( BASE_OFFSET, TypeStats<TType>::ALIGNMENT );

		// Offset for next entity.
		static constexpr const size_t NEXT_OFFSET	= OFFSET + TypeStats<TType>::SIZE;
	};

	// Helper to build the size of type collections.
	template< size_t BASE_OFFSET, typename... TTypes >
	struct CollectionSizeHelper;

	// Deduction branch. Keep detecting the size.
	template< size_t BASE_OFFSET, typename THead, typename... TRest >
	struct CollectionSizeHelper<BASE_OFFSET, THead, TRest...> : CollectionSizeHelper<TypeOffset<THead, BASE_OFFSET>::NEXT_OFFSET, TRest...>
	{

	};

	// Terminal branch. The last type in collection reached.
	template< size_t BASE_OFFSET, typename THead >
	struct CollectionSizeHelper<BASE_OFFSET, THead>
	{
		// Calculated size of collection.
		static constexpr const size_t RESULT = TypeOffset<THead, BASE_OFFSET>::NEXT_OFFSET;
	};

	// Statistics for collection of types.
	template< typename... TTypes >
	struct TypesCollectionStats
	{
		// Total size of collection.
		static constexpr const size_t SIZE		= CollectionSizeHelper<0, TTypes...>::RESULT;

		// Alignment of collection.
		static constexpr const size_t ALIGNMENT	= NumericCollection<TypeStats<TTypes>::ALIGNMENT...>::MAXIMUM;
	};

	// Statistics for union of types.
	template< typename... TTypes >
	struct TypesUnionStats
	{
		// Size of union.
		static constexpr const size_t SIZE		= NumericCollection<TypeStats<TTypes>::SIZE...>::MAXIMUM;

		// Alignment of union.
		static constexpr const size_t ALIGNMENT	= NumericCollection<TypeStats<TTypes>::ALIGNMENT...>::MAXIMUM;
	};

	// Helper to merge the numeric collections.
	template< typename... TCollections >
	struct NumericCollectionMergeHelper;

	// Deduction branch. Keep merging the collections.
	template< size_t... NUMBERS, typename... TRest >
	struct NumericCollectionMergeHelper<NumericCollection<NUMBERS...>, TRest...>
	{
		// Result of merging.
		using Collection = typename NumericCollection<NUMBERS...>::template MergeTailCollection<typename NumericCollectionMergeHelper<TRest...>::Collection>;
	};

	// Terminal branch. The last collection found.
	template< size_t... NUMBERS >
	struct NumericCollectionMergeHelper<NumericCollection<NUMBERS...>>
	{
		// Merged numbers.
		using Collection = NumericCollection<NUMBERS...>;
	};

	template< typename TCollection, size_t BASE_INDEX, typename... TTypes >
	struct PartIntersectionHelper;

	template< typename TCollection, size_t BASE_INDEX, typename THead, typename... TRest >
	struct PartIntersectionHelper<TCollection, BASE_INDEX, THead, TRest...>
	{

	};

	template< typename TCollection, size_t BASE_INDEX, typename... TInnerTypes, typename... TRest >
	struct PartIntersectionHelper<TCollection, BASE_INDEX, Black::TypesCollection<TInnerTypes...>, TRest...>
	{

	};

	template< typename TCollection, size_t BASE_INDEX, typename... TInnerTypes, typename... TRest >
	struct PartIntersectionHelper<TCollection, BASE_INDEX, Black::TypesUnion<TInnerTypes...>, TRest...>
	{

	};

	template< typename TCollection, size_t BASE_INDEX, typename... TInnerTypes >
	struct PartIntersectionHelper<TCollection, BASE_INDEX, Black::TypesCollection<TInnerTypes...>>
	{

	};

	template< typename TCollection, size_t BASE_INDEX, typename... TInnerTypes >
	struct PartIntersectionHelper<TCollection, BASE_INDEX, Black::TypesUnion<TInnerTypes...>>
	{

	};

	template< typename TCollection, size_t BASE_INDEX, typename TLast >
	struct PartIntersectionHelper<TCollection, BASE_INDEX, TLast>
	{
		using LocalIndices	= typename Black::TypesCollection<TLast>::template IndexedProjection<TCollection>;

		using Indices		= Black::TypesCollection<typename LocalIndices::IncreaseEach<BASE_INDEX>>;
	};
}
}
}
}
