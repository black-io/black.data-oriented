#pragma once


namespace Black
{
inline namespace DataOriented
{
inline namespace Compositions
{
	template< typename THost, typename... TComponents >
	Composition<THost, TComponents...>::Composition()
	{
		static_assert( std::is_base_of_v<Composition<THost, TComponents...>, THost>, "The host type should be derived from composition mediator." );
	}

	template< typename THost, typename... TComponents >
	Composition<THost, TComponents...>::~Composition()
	{
		DestructAllComponents();
	}

	template< typename THost, typename... TComponents >
	template< typename TComponent >
	inline TComponent& Composition<THost, TComponents...>::ConstructComponent()
	{
		constexpr size_t component_index = GetComponentIndex<TComponent>();
		void* const component_memory = AllocationMediator::GetMemory( component_index );
		CRET( AllocationMediator::IsAllocated( component_index ), *reinterpret_cast<TComponent*>( component_memory ) );

		AllocationMediator::SetAllocated( component_index );
		return *ConstructionPolicy<TComponent>::Construct( component_memory, GetHostOffset() + AllocationMediator::GetOffset( component_index ) );
	}

	template< typename THost, typename... TComponents >
	template< typename TComponent >
	inline void Composition<THost, TComponents...>::DestructComponent()
	{
		constexpr size_t component_index = GetComponentIndex<TComponent>();

		CRET( !AllocationMediator::IsAllocated( component_index ) );
		void* const component_memory = AllocationMediator::GetMemory( component_index );

		ConstructionPolicy<TComponent>::Destruct( component_memory );
		AllocationMediator::SetFree( component_index );
	}

	template< typename THost, typename... TComponents >
	inline void Composition<THost, TComponents...>::DestructAllComponents()
	{
		using DestructorList = Internal::PartDestructorList<THost, Parts>;

		for( size_t component_index = 0; component_index < Parts::LENGTH; ++component_index )
		{
			CCON( !AllocationMediator::IsAllocated( component_index ) );

			void* const component_memory = AllocationMediator::GetMemory( component_index );
			DestructorList::FUNCTIONS[ component_index ]( component_memory );
			AllocationMediator::SetFree( component_index );
		}
	}

	template< typename THost, typename... TComponents >
	template< typename TComponent >
	inline TComponent& Composition<THost, TComponents...>::GetComponent()
	{
		TComponent* const component = QueryComponent<TComponent>();
		EXPECTS( component != nullptr );

		return *component;
	}

	template< typename THost, typename... TComponents >
	template< typename TComponent >
	inline const TComponent& Composition<THost, TComponents...>::GetComponent() const
	{
		const TComponent* const component = QueryComponent<TComponent>();
		EXPECTS( component != nullptr );

		return *component;
	}

	template< typename THost, typename... TComponents >
	template< typename TComponent >
	inline TComponent* const Composition<THost, TComponents...>::QueryComponent()
	{
		constexpr size_t component_index = GetComponentIndex<TComponent>();

		CRET( !AllocationMediator::IsAllocated( component_index ), nullptr );
		return reinterpret_cast<TComponent*>( AllocationMediator::GetMemory( component_index ) );
	}

	template< typename THost, typename... TComponents >
	template< typename TComponent >
	inline const TComponent* const Composition<THost, TComponents...>::QueryComponent() const
	{
		constexpr size_t component_index = GetComponentIndex<TComponent>();

		CRET( !AllocationMediator::IsAllocated( component_index ), nullptr );
		return reinterpret_cast<const TComponent*>( AllocationMediator::GetMemory( component_index ) );
	}

	template< typename THost, typename... TComponents >
	template< typename TComponent >
	inline const bool Composition<THost, TComponents...>::CanConstructComponent() const
	{
		using Indices = typename PartIntersectionIndices::template TypeAt<GetComponentIndex<TComponent>()>;
		return std::none_of(
			std::cbegin( Indices::ITEMS ),
			std::cend( Indices::ITEMS ),
			[this]( const size_t component_index ) -> bool
			{
				return AllocationMediator::IsAllocated( component_index );
			}
		);
	}

	template< typename THost, typename... TComponents >
	template< typename TComponent >
	inline const bool Composition<THost, TComponents...>::HasComponent() const
	{
		return AllocationMediator::IsAllocated( GetComponentIndex<TComponent>() );
	}

	template< typename THost, typename... TComponents >
	template< typename... TQueriedComponents >
	inline const bool Composition<THost, TComponents...>::HasAnyComponent() const
	{
		const size_t component_indices[] = { GetComponentIndex<TQueriedComponents>()... };

		return std::any_of(
			std::cbegin( component_indices ),
			std::cend( component_indices ),
			[this]( const size_t component_index ) -> bool
			{
				return AllocationMediator::IsAllocated( component_index );
			}
		);
	}

	template< typename THost, typename... TComponents >
	template< typename... TQueriedComponents >
	inline const bool Composition<THost, TComponents...>::HasAllComponents() const
	{
		const size_t component_indices[] = { GetComponentIndex<TQueriedComponents>()... };

		return std::all_of(
			std::cbegin( component_indices ),
			std::cend( component_indices ),
			[this]( const size_t component_index ) -> bool
			{
				return AllocationMediator::IsAllocated( component_index );
			}
		);
	}

	template< typename THost, typename... TComponents >
	inline const size_t Composition<THost, TComponents...>::GetHostOffset() const
	{
		return (std::ptrdiff_t)AllocationMediator::GetMemory( 0 ) - (std::ptrdiff_t)static_cast<const THost*>( this );
	}
}
}
}
