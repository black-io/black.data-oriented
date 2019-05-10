#pragma once


namespace Black
{
inline namespace DataOriented
{
inline namespace Composition
{
namespace Internal
{
	// Mapping information about components.
	template< typename... TComponents >
	struct ComponentMapping
	{
		// Ordered collection of components.
		using Components		= typename Black::TypesCollection<TComponents...>::UnfoldedCollection;

		// Ordered list of component offsets.
		using OffsetCollection	= typename TypesCollectionOffsetBuilder<0, TComponents...>::Offsets;


		// Total number of mapped components.
		static constexpr const size_t COMPONENTS_COUNT	= Components::LENGTH;

		// Total size of memory to map the components.
		static constexpr const size_t SIZE				= TypesCollectionStats<TComponents...>::SIZE;

		// Alignment of component mapping.
		static constexpr const size_t ALIGNMENT			= TypesCollectionStats<TComponents...>::ALIGNMENT;
	};
}
}
}
}
