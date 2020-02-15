#pragma once


namespace Black
{
inline namespace DataOriented
{
inline namespace Composition
{
	template< typename THost, typename... TAllowedFeatures >
	FeatureComposition<THost, TAllowedFeatures...>::FeatureComposition()
	{
		static_assert( Black::IS_BASE_OF<FeatureComposition<THost, TAllowedFeatures...>, THost>, "The host type should be derived from composition mediator." );
	}

	template< typename THost, typename... TAllowedFeatures >
	FeatureComposition<THost, TAllowedFeatures...>::~FeatureComposition()
	{
		DestructAllFeatures();
	}

	template< typename THost, typename... TAllowedFeatures >
	template< typename TFeature >
	inline TFeature& FeatureComposition<THost, TAllowedFeatures...>::ConstructFeature()
	{
		constexpr size_t feature_index = GetPartIndex<TFeature>();
		void* const feature_memory = AllocationMediator::GetMemory( feature_index );
		CRET( AllocationMediator::IsAllocated( feature_index ), *reinterpret_cast<TFeature*>( feature_memory ) );

		AllocationMediator::SetAllocated( feature_index );
		return *ConstructionPolicy<TFeature>::Construct( feature_memory, GetHostOffset() + AllocationMediator::GetOffset( feature_index ) );
	}

	template< typename THost, typename... TAllowedFeatures >
	template< typename TFeature >
	inline void FeatureComposition<THost, TAllowedFeatures...>::DestructFeature()
	{
		constexpr size_t feature_index = GetPartIndex<TFeature>();

		CRET( !AllocationMediator::IsAllocated( feature_index ) );
		void* const feature_memory = AllocationMediator::GetMemory( feature_index );

		ConstructionPolicy<TFeature>::Destruct( feature_memory );
		AllocationMediator::SetFree( feature_index );
	}

	template< typename THost, typename... TAllowedFeatures >
	inline void FeatureComposition<THost, TAllowedFeatures...>::DestructAllFeatures()
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

	template< typename THost, typename... TAllowedFeatures >
	template< typename TFeature >
	inline TFeature& FeatureComposition<THost, TAllowedFeatures...>::GetFeature()
	{
		TFeature* const feature = QueryFeature<TFeature>();
		EXPECTS( feature != nullptr );

		return *feature;
	}

	template< typename THost, typename... TAllowedFeatures >
	template< typename TFeature >
	inline const TFeature& FeatureComposition<THost, TAllowedFeatures...>::GetFeature() const
	{
		const TFeature* const feature = QueryFeature<TFeature>();
		EXPECTS( feature != nullptr );

		return *feature;
	}

	template< typename THost, typename... TAllowedFeatures >
	template< typename TFeature >
	inline TFeature* const FeatureComposition<THost, TAllowedFeatures...>::QueryFeature()
	{
		constexpr size_t feature_index = GetPartIndex<TFeature>();

		CRET( !AllocationMediator::IsAllocated( feature_index ), nullptr );
		return reinterpret_cast<TFeature*>( AllocationMediator::GetMemory( feature_index ) );
	}

	template< typename THost, typename... TAllowedFeatures >
	template< typename TFeature >
	inline const TFeature* const FeatureComposition<THost, TAllowedFeatures...>::QueryFeature() const
	{
		constexpr size_t feature_index = GetPartIndex<TFeature>();

		CRET( !AllocationMediator::IsAllocated( feature_index ), nullptr );
		return reinterpret_cast<const TFeature*>( AllocationMediator::GetMemory( feature_index ) );
	}

	template< typename THost, typename... TAllowedFeatures >
	template< typename TFeature >
	inline const bool FeatureComposition<THost, TAllowedFeatures...>::CanConstructFeature() const
	{
		using Indices = typename PartIntersectionIndices::template TypeAt<GetPartIndex<TFeature>()>;
		return std::none_of(
			std::cbegin( Indices::ITEMS ),
			std::cend( Indices::ITEMS ),
			[this]( const size_t feature_index ) -> bool
			{
				return AllocationMediator::IsAllocated( feature_index );
			}
		);
	}

	template< typename THost, typename... TAllowedFeatures >
	template< typename TFeature >
	inline const bool FeatureComposition<THost, TAllowedFeatures...>::HasFeature() const
	{
		return AllocationMediator::IsAllocated( GetPartIndex<TFeature>() );
	}

	template< typename THost, typename... TAllowedFeatures >
	template< typename... TParts >
	inline const bool FeatureComposition<THost, TAllowedFeatures...>::HasAnyFeature() const
	{
		const size_t feature_indices[] = { GetPartIndex<TParts>()... };

		return std::any_of(
			std::cbegin( feature_indices ),
			std::cend( feature_indices ),
			[this]( const size_t feature_index ) -> bool
			{
				return AllocationMediator::IsAllocated( feature_index );
			}
		);
	}

	template< typename THost, typename... TAllowedFeatures >
	template< typename... TParts >
	inline const bool FeatureComposition<THost, TAllowedFeatures...>::HasAllFeatures() const
	{
		const size_t feature_indices[] = { GetPartIndex<TParts>()... };

		return std::all_of(
			std::cbegin( feature_indices ),
			std::cend( feature_indices ),
			[this]( const size_t feature_index ) -> bool
			{
				return AllocationMediator::IsAllocated( feature_index );
			}
		);
	}

	template< typename THost, typename... TAllowedFeatures >
	inline const size_t FeatureComposition<THost, TAllowedFeatures...>::GetHostOffset() const
	{
		return (std::ptrdiff_t)AllocationMediator::GetMemory( 0 ) - (std::ptrdiff_t)static_cast<const THost*>( this );
	}
}
}
}
