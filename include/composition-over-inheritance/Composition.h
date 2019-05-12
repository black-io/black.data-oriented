#pragma once


namespace Black
{
inline namespace DataOriented
{
inline namespace Composition
{
	// Composition storage implementation.
	template< typename THost, typename... TAllowedParts >
	class CompositionMediator : private Black::NonTransferable, private Internal::PartitionMediator<Internal::PartitionMap<TAllowedParts...>>
	{
	// Construction and assignment.
	public:
		CompositionMediator();
		~CompositionMediator();

	// Public interface.
	public:
		// Assign the new component. Returns the actual copy of component if one already exists.
		template< typename TPart >
		inline TPart& ConstructPart();

		// Delete the existing component. Does nothing if component already deleted.
		template< typename TPart >
		inline void DestructPart();

		// Delete all allocated components.
		inline void DestructAllParts();


		// Get the allocated component. Expects that component already assigned.
		template< typename TPart >
		inline TPart& GetPart();

		// Get the allocated component. Expects that component already assigned.
		template< typename TPart >
		inline const TPart& GetPart() const;

		// Query the component. `nullptr` will be returned if component not assigned.
		template< typename TPart >
		inline TPart* QueryPart();

		// Query the component. `nullptr` will be returned if component not assigned.
		template< typename TPart >
		inline const TPart* QueryPart() const;


		// Whether the component may be constructed according to placement rules.
		template< typename TPart >
		inline const bool CanConstructPart() const;

		// Whether the composition has allocated a component.
		template< typename TPart >
		inline const bool HasPart() const;

		// Whether the composition has allocated any of components.
		template< typename... TParts >
		inline const bool HasAnyPart() const;

		// Whether the composition has allocated all the components.
		template< typename... TParts >
		inline const bool HasAllParts() const;

	// Private types.
	private:
		// Construction policy template.
		template< typename TPart >
		using ConstructionPolicy	= Internal::PartConstructionPolicy<TPart, THost>;


		// Part mapping.
		using PartitionMap				= Internal::PartitionMap<TAllowedParts...>;

		// Allocation mediator type.
		using AllocationMediator	= Internal::PartitionMediator<PartitionMap>;

		// List of used components.
		using Parts					= typename PartitionMap::Parts;

	// Private interface.
	private:
		// Get ordinal index of component.
		template< typename TPart >
		static constexpr const size_t GetPartIndex()
		{
			static_assert( Parts::IS_EXIST<TPart>, "Invalid type of composition part." );
			return Parts::INDEX_OF<TPart>;
		}

		// Get the offset from host base to the base of composition mediator.
		inline const size_t GetHostOffset() const;
	};
}
}
}
