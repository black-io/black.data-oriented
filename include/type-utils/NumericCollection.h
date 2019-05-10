#pragma once


namespace Black
{
inline namespace DataOriented
{
inline namespace TypeUtils
{
	// Trivial numeric collection.
	template< typename TNumberType, TNumberType... NUMBERS >
	struct NumericCollection final
	{
		static_assert( Black::IS_INTEGER<TNumberType>, "The type of numbers should be integral." );

		// Add portion to each number.
		template< TNumberType PORTION >
		using IncreaseEach			= NumericCollection<TNumberType, ( NUMBERS + PORTION )...>;

		// Subtract portion from each number.
		template< TNumberType PORTION >
		using DecreaseEach			= NumericCollection<TNumberType, ( NUMBERS - PORTION )...>;

		// Merge the numbers into the head of collection.
		template< TNumberType... OTHER_NUMBERS >
		using MergeHeadNumbers		= NumericCollection<TNumberType, OTHER_NUMBERS..., NUMBERS...>;

		// Merge the numbers into the tail of collection.
		template< TNumberType... OTHER_NUMBERS >
		using MergeTailNumbers		= NumericCollection<TNumberType, NUMBERS..., OTHER_NUMBERS...>;

		// Merge the numbers into the head of collection.
		template< typename TOtherNumbers >
		using MergeHeadCollection	= typename TOtherNumbers::template MergeTailNumbers<NUMBERS...>;

		// Merge the given number collection into the tail of collection.
		template< typename TOtherNumbers >
		using MergeTailCollection	= typename TOtherNumbers::template MergeHeadNumbers<NUMBERS...>;


		// Length of collection.
		static constexpr const TNumberType LENGTH	= sizeof...( NUMBERS );

		// Collection representation.
		static constexpr const TNumberType ITEMS[]	= { NUMBERS... };

		// Minimum of collection.
		static constexpr const TNumberType MINIMUM	= Internal::NumericCollectionMinimum<TNumberType, NUMBERS...>::RESULT;

		// Maximum of collection.
		static constexpr const TNumberType MAXIMUM	= Internal::NumericCollectionMaximum<TNumberType, NUMBERS...>::RESULT;
	};
}
}
}
