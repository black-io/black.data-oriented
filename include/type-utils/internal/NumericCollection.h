#pragma once


namespace Black
{
inline namespace DataOriented
{
inline namespace TypeUtils
{
namespace Internal
{
	// Trivial numeric collection.
	template< typename TNumberType, TNumberType... NUMBERS >
	struct NumericCollection;

	// Helper to merge the tail into numeric collection.
	template< typename TNumberType, typename TTailNumbers, TNumberType... NUMBERS >
	struct NumericCollectionTailMerger;

	// Helper to merge the tail into numeric collection.
	template< typename TNumberType, TNumberType... NUMBERS, TNumberType... TAIL_NUMBERS >
	struct NumericCollectionTailMerger<TNumberType, NumericCollection<TNumberType, TAIL_NUMBERS...>, NUMBERS...> final
	{
		// Result of merging.
		using Result = NumericCollection<TNumberType, NUMBERS..., TAIL_NUMBERS...>;
	};

	// Helper to detect the minimum number of collection.
	// @TODO: Use `std::min` from C++14.
	template< typename TNumberType, TNumberType... NUMBERS >
	struct NumericCollectionMinimum;

	// Deduction branch. Reduce the range of selection.
	template< typename TNumberType, TNumberType LEFT, TNumberType RIGHT, TNumberType... REST >
	struct NumericCollectionMinimum<TNumberType, LEFT, RIGHT, REST...> final
	{
		// Result of selection.
		static constexpr const TNumberType RESULT = NumericCollectionMinimum<
			TNumberType,
			LEFT,
			NumericCollectionMinimum<TNumberType, RIGHT, REST...>::RESULT
		>::RESULT;
	};

	// Terminal branch. Binary selection.
	template< typename TNumberType, TNumberType LEFT, TNumberType RIGHT >
	struct NumericCollectionMinimum<TNumberType, LEFT, RIGHT> final
	{
		// Result of selection.
		static constexpr const TNumberType RESULT = ( LEFT < RIGHT )? LEFT : RIGHT;
	};

	// Terminal branch. Single number minimum.
	template< typename TNumberType, TNumberType NUMBER >
	struct NumericCollectionMinimum<TNumberType, NUMBER> final
	{
		// Result of selection.
		static constexpr const TNumberType RESULT = NUMBER;
	};

	// Helper to detect the maximum number of collection.
	// @TODO: Use `std::max` from C++14.
	template< typename TNumberType, TNumberType... NUMBERS >
	struct NumericCollectionMaximum;

	// Deduction branch. Reduce the range of selection.
	template< typename TNumberType, TNumberType LEFT, TNumberType RIGHT, TNumberType... REST >
	struct NumericCollectionMaximum<TNumberType, LEFT, RIGHT, REST...> final
	{
		// Result of selection.
		static constexpr const TNumberType RESULT = NumericCollectionMaximum<
			TNumberType,
			LEFT,
			NumericCollectionMaximum<TNumberType, RIGHT, REST...>::RESULT
		>::RESULT;
	};

	// Terminal branch. Binary selection.
	template< typename TNumberType, TNumberType LEFT, TNumberType RIGHT >
	struct NumericCollectionMaximum<TNumberType, LEFT, RIGHT> final
	{
		// Result of selection.
		static constexpr const TNumberType RESULT = ( LEFT > RIGHT )? LEFT : RIGHT;
	};

	// Terminal branch. Single number minimum.
	template< typename TNumberType, TNumberType NUMBER >
	struct NumericCollectionMaximum<TNumberType, NUMBER> final
	{
		// Result of selection.
		static constexpr const TNumberType RESULT = NUMBER;
	};
}
}
}
}