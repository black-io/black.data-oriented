#pragma once


namespace Black
{
inline namespace DataOriented
{
inline namespace Composition
{
namespace Internal
{
	// List of destruction functions for collection of composition parts.
	template< typename THost, typename... TParts >
	struct PartDestructorList
	{
		static constexpr const Black::GlobalFunctionPointer<void, void*> FUNCTIONS[ sizeof...( TParts ) ] = {
			&PartConstructionPolicy<TParts, THost>::Destruct...
		};
	};

	template< typename THost, typename... TParts >
	constexpr const Black::GlobalFunctionPointer<void, void*> PartDestructorList<THost, TParts...>::FUNCTIONS[ sizeof...( TParts ) ];


	// Deduction branch. Simplify the parts deduction from collection.
	template< typename THost, typename... TParts >
	struct PartDestructorList<THost, Black::TypesCollection<TParts...>> : PartDestructorList<THost, TParts...>
	{

	};
}
}
}
}
