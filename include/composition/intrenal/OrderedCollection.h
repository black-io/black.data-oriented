#pragma once


namespace Black
{
inline namespace DataOriented
{
inline namespace Composition
{
namespace Internal
{
	// Ordered collection of components is an collection resulted of unpacking all inner collections of input collection.
	template< typename... TComponents >
	struct OrderedCollection
	{
		// Resulted ordered collection.
		using Components = typename CollectionUnpackHelper<Black::TypesEmptyCollection, TComponents...>::Components;
	};

	// Deduction branch to simplify the construction from component collection.
	template< typename... TComponents >
	struct OrderedCollection<Black::TypesCollection<TComponents...>> final : OrderedCollection<TComponents...>
	{

	};
}
}
}
}
