#pragma once


namespace Black
{
inline namespace DataOriented
{
inline namespace Composition
{
namespace Internal
{
	// Helper to merge the tail into numeric collection.
	template< typename TTailNumbers, size_t... NUMBERS >
	struct NumericCollectionTailMerger;

	// Helper to detect the minimum number of collection.
	// @TODO: Use `std::min` from C++14.
	template< size_t... NUMBERS >
	struct NumericCollectionMinimum;

	// Helper to detect the maximum number of collection.
	// @TODO: Use `std::max` from C++14.
	template< size_t... NUMBERS >
	struct NumericCollectionMaximum;


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


	// Helper to merge the tail into numeric collection.
	template< size_t... NUMBERS, size_t... TAIL_NUMBERS >
	struct NumericCollectionTailMerger<NumericCollection<TAIL_NUMBERS...>, NUMBERS...> final
	{
		// Result of merging.
		using Result = NumericCollection<NUMBERS..., TAIL_NUMBERS...>;
	};

	// Deduction branch. Reduce the range of selection.
	template< size_t LEFT, size_t RIGHT, size_t... REST >
	struct NumericCollectionMinimum<LEFT, RIGHT, REST...> final
	{
		// Result of selection.
		static constexpr const size_t RESULT = NumericCollectionMinimum<LEFT, NumericCollectionMinimum<RIGHT, REST...>::RESULT>::RESULT;
	};

	// Terminal branch. Binary selection.
	template< size_t LEFT, size_t RIGHT >
	struct NumericCollectionMinimum<LEFT, RIGHT> final
	{
		// Result of selection.
		static constexpr const size_t RESULT = ( LEFT < RIGHT )? LEFT : RIGHT;
	};

	// Terminal branch. Single number minimum.
	template< size_t NUMBER >
	struct NumericCollectionMinimum<NUMBER> final
	{
		// Result of selection.
		static constexpr const size_t RESULT = NUMBER;
	};

	// Deduction branch. Reduce the range of selection.
	template< size_t LEFT, size_t RIGHT, size_t... REST >
	struct NumericCollectionMaximum<LEFT, RIGHT, REST...> final
	{
		// Result of selection.
		static constexpr const size_t RESULT = NumericCollectionMaximum<LEFT, NumericCollectionMaximum<RIGHT, REST...>::RESULT>::RESULT;
	};

	// Terminal branch. Binary selection.
	template< size_t LEFT, size_t RIGHT >
	struct NumericCollectionMaximum<LEFT, RIGHT> final
	{
		// Result of selection.
		static constexpr const size_t RESULT = ( LEFT > RIGHT )? LEFT : RIGHT;
	};

	// Terminal branch. Single number minimum.
	template< size_t NUMBER >
	struct NumericCollectionMaximum<NUMBER> final
	{
		// Result of selection.
		static constexpr const size_t RESULT = NUMBER;
	};
}
}
}
}
