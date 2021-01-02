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
	struct PartConstructionPolicy final
	{
		// Construct the component using the given memory.
		static inline TComponent* Construct( void* memory, const size_t host_offset )	{ return new( memory ) TComponent{}; };

		// Destruct the component in memory.
		static inline void Destruct( void* memory )
		{
			EXPECTS_DEBUG( memory != nullptr );
			reinterpret_cast<TComponent*>( memory )->~TComponent();
		};
	};

	// Exclusive component allocator to allocate smart components.
	template< typename TComponent, typename THost >
	struct PartConstructionPolicy<TComponent, THost, Black::EnableIf<Black::IS_BASE_OF<Black::HostRelatedFeature<THost>, TComponent>>> final
	{
		// Construct the component using the given memory.
		static inline TComponent* Construct( void* memory, const size_t host_offset )	{ return new( memory ) TComponent{ host_offset }; };

		// Destruct the component in memory.
		static inline void Destruct( void* memory )
		{
			EXPECTS_DEBUG( memory != nullptr );
			reinterpret_cast<TComponent*>( memory )->~TComponent();
		};
	};
}
}
}
}
