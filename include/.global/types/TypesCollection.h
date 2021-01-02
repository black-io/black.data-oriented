#pragma once


namespace Black
{
inline namespace DataOriented
{
inline namespace TypeUtils
{
	// Trivial collection of types.
	template< typename... TTypes >
	struct TypesCollection final
	{
		// Collection, where all inner collections are unfolded.
		using UnfoldedCollection	= typename Internal::TypesCollectionUnfoldHelper<TypesCollection<TTypes...>>::Collection;

		// Merge some types into head of collection.
		template< typename... TOtherTypes >
		using MergeHeadTypes		= TypesCollection<TOtherTypes..., TTypes...>;

		// Merge some types into tail of collection.
		template< typename... TOtherTypes >
		using MergeTailTypes		= TypesCollection<TTypes..., TOtherTypes...>;

		// Merge some types into head of collection.
		template< typename TCollection >
		using MergeHeadCollection	= typename TCollection::template MergeTailTypes<TTypes...>;

		// Merge some types into tail of collection.
		template< typename TCollection >
		using MergeTailCollection	= typename TCollection::template MergeHeadTypes<TTypes...>;

		// Produce the collection by repeating the types with given number of times.
		template< size_t REPEAT_COUNT >
		using Repeat				= typename Internal::TypesCollectionRepeatHelper<REPEAT_COUNT - 1, TTypes...>::Collection;

		// Get the indices of types in other collection.
		template< typename TOtherCollection >
		using IndexedProjection		= NumericCollection<size_t, TOtherCollection::template INDEX_OF<TTypes>...>;

		// Get the type with given index.
		template< size_t INDEX >
		using TypeAt				= typename Internal::TypesCollectionAccessHelper<INDEX, TTypes...>::Result;


		// Length of collection.
		static constexpr const size_t LENGTH	= sizeof...( TTypes );

		// Ordinal index of type. `UNDEFINED_INDEX` in case of type does not exist in collection.
		template< typename TType >
		static constexpr const size_t INDEX_OF	= Internal::TypesCollectionIndexHelper<0, TType, TypesCollection<TTypes...>>::RESULT;

		// Whether the type exist in collection.
		template< typename TType >
		static constexpr const bool IS_EXIST	= INDEX_OF<TType> != Black::UNDEFINED_INDEX;
	};

	// Empty trivial types collection.
	using TypesEmptyCollection = TypesCollection<>;
}
}
}
