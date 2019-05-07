#pragma once


namespace Black
{
inline namespace DataOriented
{
namespace Internal
{
	// Trivial numeric collection.
	template< size_t... NUMBERS >
	struct NumericCollection final
	{
		// Add portion to each number.
		template< size_t PORTION >
		using IncreaseEach			= NumericCollection<( NUMBERS + PORTION )...>;

		// Subtract portion from each number.
		template< size_t PORTION >
		using DecreaseEach			= NumericCollection<( NUMBERS - PORTION )...>;

		// Merge the numbers into the tail of collection.
		template< size_t... OTHER_NUMBERS >
		using MergeTailNumbers		= NumericCollection<NUMBERS..., OTHER_NUMBERS...>;

		// Merge the given number collection into the tail of collection.
		template< typename TOtherNumbers >
		using MergeTailCollection	= typename NumericCollectionTailMerger<TOtherNumbers, NUMBERS...>::Result;

		// Merge the numbers into the head of collection.
		template< size_t... OTHER_NUMBERS >
		using MergeHeadNumbers		= NumericCollection<OTHER_NUMBERS..., NUMBERS...>;

		// Merge the numbers into the head of collection.
		template< typename TOtherNumbers >
		using MergeHeadCollection	= typename TOtherNumbers::template MergeTailNumbers<NUMBERS...>;


		// Length of collection.
		static constexpr const size_t LENGTH	= sizeof...( NUMBERS );

		// Collection representation.
		static constexpr const size_t ITEMS[]	= { NUMBERS... };

		// Minimum of collection.
		static constexpr const size_t MINIMUM	= NumericCollectionMinimum<NUMBERS...>::RESULT;

		// Maximum of collection.
		static constexpr const size_t MAXIMUM	= NumericCollectionMaximum<NUMBERS...>::RESULT;
	};
}
}
}
