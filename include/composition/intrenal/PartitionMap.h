#pragma once


namespace Black
{
inline namespace DataOriented
{
inline namespace Composition
{
namespace Internal
{
	// Mapping information for collection of composition parts.
	template< typename... TParts >
	struct PartitionMap
	{
		// Unfolded collection of mapped parts.
		using Parts				= typename Black::TypesCollection<TParts...>::UnfoldedCollection;

		// Collection of valid part offsets.
		using OffsetCollection	= typename TypesCollectionOffsetBuilder<0, TParts...>::Offsets;


		// Total number of mapped parts.
		static constexpr const size_t COMPONENTS_COUNT	= Parts::LENGTH;

		// Total size of memory, which the partition may occupy.
		static constexpr const size_t SIZE				= TypesCollectionStats<TParts...>::SIZE;

		// Alignment of the partition.
		static constexpr const size_t ALIGNMENT			= TypesCollectionStats<TParts...>::ALIGNMENT;
	};
}
}
}
}
