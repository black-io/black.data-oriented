#pragma once


namespace Black
{
inline namespace DataOriented
{
inline namespace Global
{
inline namespace Types
{
	// Trivial numeric collection.
	template< typename TNumberType, TNumberType... NUMBERS >
	struct NumericCollection final
	{
		static_assert( std::is_integral_v<TNumberType>, "The type of numbers should be integral." );


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
		static inline constexpr TNumberType LENGTH		= sizeof...( NUMBERS );

		// Collection representation.
		static inline constexpr TNumberType ITEMS[]		= { NUMBERS... };

		// Minimum of collection.
		static inline constexpr TNumberType MINIMUM		= std::min( { NUMBERS... } );

		// Maximum of collection.
		static inline constexpr TNumberType MAXIMUM		= std::max( { NUMBERS... } );
	};
}
}
}
}
