#pragma once


namespace Black
{
inline namespace DataOriented
{
inline namespace Compositions
{
namespace Internal
{
	// List of destruction functions for collection of composition parts.
	template< typename THost, typename... TParts >
	struct PartDestructorList
	{
		using DestructionFunction = void (*)( void* );

		static inline constexpr const DestructionFunction FUNCTIONS[] = {
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
