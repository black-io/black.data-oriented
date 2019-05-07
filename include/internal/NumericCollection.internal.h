#pragma once


namespace Black
{
inline namespace DataOriented
{
namespace Internal
{
	// Trivial numeric collection.
	template< size_t... NUMBERS >
	struct NumericCollection;

	// Helper to merge the tail into numeric collection.
	template< typename TTailNumbers, size_t... NUMBERS >
	struct NumericCollectionTailMerger;

	// Helper to merge the tail into numeric collection.
	template< size_t... NUMBERS, size_t... TAIL_NUMBERS >
	struct NumericCollectionTailMerger<NumericCollection<TAIL_NUMBERS...>, NUMBERS...> final
	{
		// Result of merging.
		using Result = NumericCollection<NUMBERS..., TAIL_NUMBERS...>;
	};

	// Helper to detect the minimum number of collection.
	// @TODO: Use `std::min` from C++14.
	template< size_t... NUMBERS >
	struct NumericCollectionMinimum;

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

	// Helper to detect the maximum number of collection.
	// @TODO: Use `std::max` from C++14.
	template< size_t... NUMBERS >
	struct NumericCollectionMaximum;

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
