#pragma once


namespace Black
{
inline namespace DataOriented
{
inline namespace Composition
{
	template< typename THost, typename... TAllowedParts >
	CompositionMediator<THost, TAllowedParts...>::CompositionMediator()
	{
		static_assert( Black::IS_BASE_OF<CompositionMediator<THost, TAllowedParts...>, THost>, "The host type should be derived from composition mediator." );
	}

	template< typename THost, typename... TAllowedParts >
	CompositionMediator<THost, TAllowedParts...>::~CompositionMediator()
	{
		DestructAllParts();
	}

	template< typename THost, typename... TAllowedParts >
	template< typename TPart >
	inline TPart& CompositionMediator<THost, TAllowedParts...>::ConstructPart()
	{
		const size_t part_index = GetPartIndex<TPart>();
		void* part_memory = AllocationMediator::GetMemory( part_index );
		CRET( AllocationMediator::IsAllocated( part_index ), *reinterpret_cast<TPart*>( part_memory ) );

		AllocationMediator::SetAllocated( part_index );
		return *ConstructionPolicy<TPart>::Construct( part_memory, GetHostOffset() + AllocationMediator::GetOffset( part_index ) );
	}

	template< typename THost, typename... TAllowedParts >
	template< typename TPart >
	inline void CompositionMediator<THost, TAllowedParts...>::DestructPart()
	{
		const size_t part_index = GetPartIndex<TPart>();

		CRET( !AllocationMediator::IsAllocated( part_index ) );
		void* part_memory = AllocationMediator::GetMemory( part_index );

		ConstructionPolicy<TPart>::Destruct( part_memory );
		AllocationMediator::SetFree( part_index );
	}

	template< typename THost, typename... TAllowedParts >
	inline void CompositionMediator<THost, TAllowedParts...>::DestructAllParts()
	{
		using DestructorList = Internal::PartDestructorList<THost, Parts>;

		for( size_t part_index = 0; part_index < Parts::LENGTH; ++part_index )
		{
			CCON( !AllocationMediator::IsAllocated( part_index ) );

			DestructorList::FUNCTIONS[ part_index ]( AllocationMediator::GetMemory( part_index ) );
			AllocationMediator::SetFree( part_index );
		}
	}

	template< typename THost, typename... TAllowedParts >
	template< typename TPart >
	inline TPart& CompositionMediator<THost, TAllowedParts...>::GetPart()
	{
		TPart* part = QueryPart<TPart>();
		EXPECTS( part != nullptr );

		return *part;
	}

	template< typename THost, typename... TAllowedParts >
	template< typename TPart >
	inline const TPart& CompositionMediator<THost, TAllowedParts...>::GetPart() const
	{
		const TPart* part = QueryPart<TPart>();
		EXPECTS( part != nullptr );

		return *part;
	}

	template< typename THost, typename... TAllowedParts >
	template< typename TPart >
	inline TPart* CompositionMediator<THost, TAllowedParts...>::QueryPart()
	{
		const size_t part_index = GetPartIndex<TPart>();

		CRET( !AllocationMediator::IsAllocated( part_index ), nullptr );
		return reinterpret_cast<TPart*>( AllocationMediator::GetMemory( part_index ) );
	}

	template< typename THost, typename... TAllowedParts >
	template< typename TPart >
	inline const TPart* CompositionMediator<THost, TAllowedParts...>::QueryPart() const
	{
		const size_t part_index = GetPartIndex<TPart>();

		CRET( !AllocationMediator::IsAllocated( part_index ), nullptr );
		return reinterpret_cast<const TPart*>( AllocationMediator::GetMemory( part_index ) );
	}

	template< typename THost, typename... TAllowedParts >
	template< typename TPart >
	inline const bool CompositionMediator<THost, TAllowedParts...>::CanConstructPart() const
	{
		return true;
	}

	template< typename THost, typename... TAllowedParts >
	template< typename TPart >
	inline const bool CompositionMediator<THost, TAllowedParts...>::HasPart() const
	{
		return AllocationMediator::IsAllocated( GetPartIndex<TPart>() );
	}

	template< typename THost, typename... TAllowedParts >
	template< typename... TParts >
	inline const bool CompositionMediator<THost, TAllowedParts...>::HasAnyPart() const
	{
		const size_t part_indices[] = { GetPartIndex<TParts>()... };

		return std::any_of(
			std::begin( part_indices ),
			std::end( part_indices ),
			[this]( const size_t part_index ) -> bool
			{
				return AllocationMediator::IsAllocated( part_index );
			}
		);
	}

	template< typename THost, typename... TAllowedParts >
	template< typename... TParts >
	inline const bool CompositionMediator<THost, TAllowedParts...>::HasAllParts() const
	{
		const size_t part_indices[] = { GetPartIndex<TParts>()... };

		return std::all_of(
			std::begin( part_indices ),
			std::end( part_indices ),
			[this]( const size_t part_index ) -> bool
			{
				return AllocationMediator::IsAllocated( part_index );
			}
		);
	}

	template< typename THost, typename... TAllowedParts >
	inline const size_t CompositionMediator<THost, TAllowedParts...>::GetHostOffset() const
	{
		return (std::ptrdiff_t)AllocationMediator::GetMemory( 0 ) - (std::ptrdiff_t)static_cast<const THost*>( this );
	}
}
}
}
