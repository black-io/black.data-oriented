#pragma once


namespace Black
{
inline namespace DataOriented
{
inline namespace Compositions
{
	/**
		@brief	Composition implementation.

		This template implements the storage for `Composition over inheritance` paradigm.
		The collection of composition components should be used to instantiate the type from template.
		The type of composition host is required also. It is required to implement the access to host from some host-related component.

		It's allowed to combine the components inside of `Black::TypesCollection` and `Black::TypesUnion` collections.
		In this case, the components in `Black::TypesCollection` will be considered as grouped sequence in memory.
		In opposition, the `Black::TypesUnion` describes the components inside as `union` of types, which occupy the same wide-enough well-aligned memory.
		It is possible to declare any combination of collections and unions of components.

		The composition totally controls the lifetime of components. Memory controlled as well. All components are placed inside of composition.
		But also some notes should be taken in consideration about memory usage. There not all components takes its own memory in composition.
		All components of `Black::TypesUnion` shares same memory space, which is well-aligned and well-sized to fit any component of union.
		The `Black::TypesUnion` collection may be used to declare the list of mutually exclusive components, which will never been constructed together for host.
		All components of `Black::TypesCollection` shares only the top-alignment, but in memory the components will be placed one by one according to its alignment.
		So that the `Black::TypesCollection` implements the semantics of `struct`, but `Black::TypesUnion` implements the semantics of `union` for components.

		Before destruction, the composition destructs all currently constructed components automatically.

		@tparam	THost			Host of composition.
		@tparam	TComponents		Structured list of components.
	*/
	template< typename THost, typename... TComponents >
	class Composition : private Black::NonTransferable, private Internal::PartitionMediator<Internal::PartitionMap<TComponents...>>
	{
	// Construction and assignment.
	public:
		Composition();
		~Composition();

	// Public interface.
	public:
		// Assign the new component. Returns the actual copy of component if one already exists.
		template< typename TComponent >
		inline TComponent& ConstructComponent();

		// Delete the existing component. Does nothing if component already deleted.
		template< typename TComponent >
		inline void DestructComponent();

		// Delete all allocated components.
		inline void DestructAllComponents();


		// Get the allocated component. Expects that component already assigned.
		template< typename TComponent >
		inline TComponent& GetComponent();

		// Get the allocated component. Expects that component already assigned.
		template< typename TComponent >
		inline const TComponent& GetComponent() const;

		// Query the component. `nullptr` will be returned if component not assigned.
		template< typename TComponent >
		inline TComponent* const QueryComponent();

		// Query the component. `nullptr` will be returned if component not assigned.
		template< typename TComponent >
		inline const TComponent* const QueryComponent() const;


		// Whether the component may be constructed according to placement rules.
		template< typename TComponent >
		inline const bool CanConstructComponent() const;

		// Whether the composition has allocated a component.
		template< typename TComponent >
		inline const bool HasComponent() const;

		// Whether the composition has allocated any of components.
		template< typename... TQueriedComponents >
		inline const bool HasAnyComponent() const;

		// Whether the composition has allocated all the components.
		template< typename... TQueriedComponents >
		inline const bool HasAllComponents() const;

	// Private types.
	private:
		// Construction policy template.
		template< typename TComponent >
		using ConstructionPolicy		= Internal::PartConstructionPolicy<TComponent, THost>;

		// Part mapping.
		using PartitionMap				= Internal::PartitionMap<TComponents...>;

		// Allocation mediator type.
		using AllocationMediator		= Internal::PartitionMediator<PartitionMap>;

		// List of used components.
		using Parts						= typename PartitionMap::Parts;

		// Collection of part intersection indices.
		using PartIntersectionIndices	= typename Internal::PartIntersectionHelper<Parts, TComponents...>::Indices;

	// Private interface.
	private:
		// Get ordinal index of component.
		template< typename TComponent >
		static constexpr const size_t GetComponentIndex()
		{
			static_assert( Parts::template IS_EXIST<TComponent>, "Invalid type of component." );
			return Parts::template INDEX_OF<TComponent>;
		}

		// Get the offset from host base to the base of composition mediator.
		inline const size_t GetHostOffset() const;
	};
}
}
}
