#pragma once


namespace Black
{
inline namespace DataOriented
{
inline namespace Composition
{
namespace Internal
{
	// Associate the list of components with indices.
	template< typename... TComponents >
	struct ComponentList
	{
		// Trivial list of components.
		using Components = typename TypesCollection<TComponents...>;


		// Length of component list.
		static constexpr const size_t LENGTH = sizeof...( TComponents );


		// Get the length of component list.
		static constexpr const size_t GetLength()	{ return LENGTH; };

		// Get the index of component. `Black::UNDEFINED_INDEX` will be returned if `TComponent` not in list of components.
		template< typename TComponent >
		static constexpr const size_t GetIndex()	{ return typename ComponentIndexHelper<TComponent, Components, 0>::Find(); };
	};

	// Deduction branch to simplify the type construction from component collection.
	template< typename... TComponents >
	struct ComponentList<TypesCollection<TComponents...>> : public ComponentList<TComponents...>
	{

	};
}
}
}
}
