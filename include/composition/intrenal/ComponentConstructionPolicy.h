#pragma once


namespace Black
{
inline namespace DataOriented
{
inline namespace Composition
{
namespace Internal
{
	// Trivial component allocator.
	template< typename TComponent, typename THost, typename = void >
	struct ComponentConstructionPolicy final
	{
		// Construct the component using the given memory.
		static inline TComponent* Construct( void* memory, const size_t host_offset )	{ return new( memory ) TComponent{}; };

		// Destruct the component in memory.
		static inline void Destruct( TComponent* component )							{ EXPECTS_DEBUG( component != nullptr ); component->~TComponent(); };
	};

	// Exclusive component allocator to allocate smart components.
	template< typename TComponent, typename THost >
	struct ComponentConstructionPolicy<TComponent, THost, Black::EnableIf<Black::IS_BASE_OF<Black::CompositionPart<THost>, TComponent>>> final
	{
		// Construct the component using the given memory.
		static inline TComponent* Construct( void* memory, const size_t host_offset )	{ return new( memory ) TComponent{ host_offset }; };

		// Destruct the component in memory.
		static inline void Destruct( TComponent* component )							{ EXPECTS_DEBUG( component != nullptr ); component->~TComponent(); };
	};
}
}
}
}
