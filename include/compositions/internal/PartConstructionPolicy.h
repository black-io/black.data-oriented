#pragma once


namespace Black
{
inline namespace DataOriented
{
inline namespace Compositions
{
namespace Internal
{
	// Trivial component allocator.
	template< typename TComponent, typename THost, typename = void >
	struct PartConstructionPolicy final
	{
		// Construct the component using the given memory.
		static inline TComponent* Construct( void* memory, const size_t )
		{
			EXPECTS_DEBUG( memory != nullptr );
			return new( memory ) TComponent{};
		};

		// Destruct the component in memory.
		static inline void Destruct( void* memory )
		{
			EXPECTS_DEBUG( memory != nullptr );
			reinterpret_cast<TComponent*>( memory )->~TComponent();
		};
	};

	// Exclusive component allocator to allocate smart components.
	template< typename TComponent, typename THost >
	struct PartConstructionPolicy<TComponent, THost, std::enable_if_t<std::is_base_of_v<Black::HostRelatedComponent<THost>, TComponent>>> final
	{
		// Construct the component using the given memory.
		static inline TComponent* Construct( void* memory, const size_t host_offset )
		{
			EXPECTS_DEBUG( memory != nullptr );
			return new( memory ) TComponent{ host_offset };
		};

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
