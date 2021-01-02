#pragma once


namespace Black
{
inline namespace DataOriented
{
inline namespace Compositions
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
		static_assert( !std::is_void_v<TType>,			"Type should not be void." );
		static_assert( !std::is_const_v<TType>,			"Type should not be constant." );
		static_assert( !std::is_reference_v<TType>,		"Type should not be reference." );
		static_assert( !std::is_pointer_v<TType>,		"Type should not be pointer." );


		// Size of type.
		static inline constexpr const size_t SIZE		= sizeof( TType );

		// Alignment of type.
		static inline constexpr const size_t ALIGNMENT	= alignof( TType );
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

	template< typename TCollection, typename... TTypes >
	struct PartIntersectionHelper;

	// Deduction branch. Keep merging the indices.
	template< typename TCollection, typename THead, typename... TRest >
	struct PartIntersectionHelper<TCollection, THead, TRest...>
	{
		// Indexed projection for current type.
		using HeadIndices	= typename PartIntersectionHelper<TCollection, THead>::Indices;

		// Merged collection of indexed projections.
		using Indices		= typename HeadIndices::template MergeTailCollection<typename PartIntersectionHelper<TCollection, TRest...>::Indices>;
	};

	// Terminal branch. Deducting the indexed projection for collection of types.
	template< typename TCollection, typename... TInnerTypes >
	struct PartIntersectionHelper<TCollection, Black::TypesCollection<TInnerTypes...>> : PartIntersectionHelper<TCollection, TInnerTypes...>
	{

	};

	// Terminal branch. Deducting the indexed projection for union of types.
	template< typename TCollection, typename... TInnerTypes >
	struct PartIntersectionHelper<TCollection, Black::TypesUnion<TInnerTypes...>>
	{
		// Unfolded union to get the projection of all collected types.
		using UnfoldedUnion	= typename Black::TypesUnion<TInnerTypes...>::UnfoldedCollection;

		// Indexed projection for single type, it consists of indices for all collected types.
		using LocalIndices	= typename UnfoldedUnion::template IndexedProjection<TCollection>;

		// Each of collected type in union represent the same indexed projection.
		using Indices		= typename Black::TypesCollection<LocalIndices>::template Repeat<UnfoldedUnion::LENGTH>;
	};

	// Terminal branch. The last type found for indexing.
	template< typename TCollection, typename TLast >
	struct PartIntersectionHelper<TCollection, TLast>
	{
		// Collection of projected index.
		using Indices = Black::TypesCollection<typename Black::TypesCollection<TLast>::template IndexedProjection<TCollection>>;
	};
}
}
}
}
