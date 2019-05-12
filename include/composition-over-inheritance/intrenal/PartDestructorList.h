#pragma once


namespace Black
{
inline namespace DataOriented
{
inline namespace CompositionOverInheritance
{
namespace Internal
{
	// List of destruction functions for collection of composition parts.
	template< typename THost, typename... TParts >
	struct PartDestructorList
	{
		static constexpr const Black::GlobalFunctionPointer<void, void*> FUNCTIONS[] = {
			&PartConstructionPolicy<TParts, THost>::Destruct...
		};
	};

	// Deduction branch. Simplify the parts deduction from collection.
	template< typename THost, typename... TParts >
	struct PartDestructorList<THost, Black::TypesCollection<TParts...>> : PartDestructorList<THost, TParts...>
	{

	};
}
}
}
}
