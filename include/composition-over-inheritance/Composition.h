#pragma once


namespace Black
{
inline namespace DataOriented
{
inline namespace CompositionOverInheritance
{
	/**
		@brief	Composition implementation.
		This template implements the storage for `Composition over inheritance` paradigm.
		The collection of composition parts should be used to instantiate the type from template.
		The type of composition host is required also. It is required to implement the access to host from composition part.

		It's allowed to combine the composition parts inside of `Black::TypesCollection` and `Black::TypesUnion` collections.
		In this case, the parts in `Black::TypesCollection` will be considered as grouped sequence of parts in memory.
		In opposition, the `Black::TypesUnion` describes the parts inside as `union` of types, which occupy the same wide-enough memory.
		It is possible to declare any combination of collections and unions of components.

		The composition totally controls the lifetime of its parts. Memory controlled as well. All parts are placed inside of composition.
		But also some notes should be taken in consideration about memory usage. There not all parts takes its own memory in composition.
		All parts of `Black::TypesUnion` shares same memory space, which is well-aligned and well-sized to fit any part of union.
		The `Black::TypesUnion` collection may be used to declare the list of mutually exclusive parts, which will never been constructed together for host.
		All parts of `Black::TypesCollection` shares only the top-alignment, but in memory the parts will be placed one by one.
		So that the `Black::TypesCollection` implements the semantics of `struct`, but `Black::TypesUnion` implements the semantics of `union` for parts.

		Before destruction, the composition destructs all currently constructed parts automatically.

		@tparam	THost			Host of composition.
		@tparam	TAllowedParts	Structured list of composition parts.
	*/
	template< typename THost, typename... TAllowedParts >
	class Composition : private Black::NonTransferable, private Internal::PartitionMediator<Internal::PartitionMap<TAllowedParts...>>
	{
	// Construction and assignment.
	public:
		Composition();
		~Composition();

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
		using ConstructionPolicy		= Internal::PartConstructionPolicy<TPart, THost>;

		// Part mapping.
		using PartitionMap				= Internal::PartitionMap<TAllowedParts...>;

		// Allocation mediator type.
		using AllocationMediator		= Internal::PartitionMediator<PartitionMap>;

		// List of used components.
		using Parts						= typename PartitionMap::Parts;

		// Collection of part intersection indices.
		using PartIntersectionIndices	= typename Internal::PartIntersectionHelper<Parts, TAllowedParts...>::Indices;

	// Private interface.
	private:
		// Get ordinal index of component.
		template< typename TPart >
		static constexpr const size_t GetPartIndex()
		{
			static_assert( Parts::template IS_EXIST<TPart>, "Invalid type of composition part." );
			return Parts::template INDEX_OF<TPart>;
		}

		// Get the offset from host base to the base of composition mediator.
		inline const size_t GetHostOffset() const;
	};
}
}
}
