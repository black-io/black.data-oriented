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
	struct ComponentAllocator final
	{
		// Construct the component using the given memory.
		inline TComponent* Construct( void* memory, const size_t host_offset ) const	{ return new( memory ) TComponent{}; };

		// Destruct the component in memory.
		inline void Destruct( TComponent* component ) const								{ EXPECTS_DEBUG( component != nullptr ); component->~TComponent(); };
	};

	// Exclusive component allocator to allocate smart components.
	template< typename TComponent, typename THost >
	struct ComponentAllocator<TComponent, THost, Black::EnableIf<Black::IS_BASE_OF<Black::CompositionComponent<THost>, TComponent>>> final
	{
		// Construct the component using the given memory.
		inline TComponent* Construct( void* memory, const size_t host_offset ) const	{ return new( memory ) TComponent{ host_offset }; };

		// Destruct the component in memory.
		inline void Destruct( TComponent* component ) const								{ EXPECTS_DEBUG( component != nullptr ); component->~TComponent(); };
	};
}
}
}
}
