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
		static_assert( Black::IS_BASE_OF<Composition<THost, TComponents...>, THost>, "The host type should be derived from composition mediator." );
	}

	template< typename THost, typename... TComponents >
	Composition<THost, TComponents...>::~Composition()
	{
		DestructAllFeatures();
	}

	template< typename THost, typename... TComponents >
	template< typename TComponent >
	inline TComponent& Composition<THost, TComponents...>::ConstructComponent()
	{
		constexpr size_t feature_index = GetComponentIndex<TComponent>();
		void* const feature_memory = AllocationMediator::GetMemory( feature_index );
		CRET( AllocationMediator::IsAllocated( feature_index ), *reinterpret_cast<TComponent*>( feature_memory ) );

		AllocationMediator::SetAllocated( feature_index );
		return *ConstructionPolicy<TComponent>::Construct( feature_memory, GetHostOffset() + AllocationMediator::GetOffset( feature_index ) );
	}

	template< typename THost, typename... TComponents >
	template< typename TComponent >
	inline void Composition<THost, TComponents...>::DestructComponent()
	{
		constexpr size_t feature_index = GetComponentIndex<TComponent>();

		CRET( !AllocationMediator::IsAllocated( feature_index ) );
		void* const feature_memory = AllocationMediator::GetMemory( feature_index );

		ConstructionPolicy<TComponent>::Destruct( feature_memory );
		AllocationMediator::SetFree( feature_index );
	}

	template< typename THost, typename... TComponents >
	inline void Composition<THost, TComponents...>::DestructAllComponents()
	{
		using DestructorList = Internal::PartDestructorList<THost, Parts>;

		for( size_t feature_index = 0; feature_index < Parts::LENGTH; ++feature_index )
		{
			CCON( !AllocationMediator::IsAllocated( feature_index ) );

			void* const feature_memory = AllocationMediator::GetMemory( feature_index );
			DestructorList::FUNCTIONS[ feature_index ]( feature_memory );
			AllocationMediator::SetFree( feature_index );
		}
	}

	template< typename THost, typename... TComponents >
	template< typename TComponent >
	inline TComponent& Composition<THost, TComponents...>::GetComponent()
	{
		TComponent* const feature = QueryFeature<TComponent>();
		EXPECTS( feature != nullptr );

		return *feature;
	}

	template< typename THost, typename... TComponents >
	template< typename TComponent >
	inline const TComponent& Composition<THost, TComponents...>::GetComponent() const
	{
		const TComponent* const feature = QueryFeature<TComponent>();
		EXPECTS( feature != nullptr );

		return *feature;
	}

	template< typename THost, typename... TComponents >
	template< typename TComponent >
	inline TComponent* const Composition<THost, TComponents...>::QueryComponent()
	{
		constexpr size_t feature_index = GetComponentIndex<TComponent>();

		CRET( !AllocationMediator::IsAllocated( feature_index ), nullptr );
		return reinterpret_cast<TComponent*>( AllocationMediator::GetMemory( feature_index ) );
	}

	template< typename THost, typename... TComponents >
	template< typename TComponent >
	inline const TComponent* const Composition<THost, TComponents...>::QueryComponent() const
	{
		constexpr size_t feature_index = GetComponentIndex<TComponent>();

		CRET( !AllocationMediator::IsAllocated( feature_index ), nullptr );
		return reinterpret_cast<const TComponent*>( AllocationMediator::GetMemory( feature_index ) );
	}

	template< typename THost, typename... TComponents >
	template< typename TComponent >
	inline const bool Composition<THost, TComponents...>::CanConstructComponent() const
	{
		using Indices = typename PartIntersectionIndices::template TypeAt<GetComponentIndex<TComponent>()>;
		return std::none_of(
			std::cbegin( Indices::ITEMS ),
			std::cend( Indices::ITEMS ),
			[this]( const size_t feature_index ) -> bool
			{
				return AllocationMediator::IsAllocated( feature_index );
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
		const size_t feature_indices[] = { GetComponentIndex<TQueriedComponents>()... };

		return std::any_of(
			std::cbegin( feature_indices ),
			std::cend( feature_indices ),
			[this]( const size_t feature_index ) -> bool
			{
				return AllocationMediator::IsAllocated( feature_index );
			}
		);
	}

	template< typename THost, typename... TComponents >
	template< typename... TQueriedComponents >
	inline const bool Composition<THost, TComponents...>::HasAllComponents() const
	{
		const size_t feature_indices[] = { GetComponentIndex<TQueriedComponents>()... };

		return std::all_of(
			std::cbegin( feature_indices ),
			std::cend( feature_indices ),
			[this]( const size_t feature_index ) -> bool
			{
				return AllocationMediator::IsAllocated( feature_index );
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
