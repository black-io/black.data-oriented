#pragma once


namespace Black
{
inline namespace DataOriented
{
inline namespace Composition
{
	/**
		@brief	FeatureComposition implementation.
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
		@tparam	TAllowedFeatures	Structured list of composition parts.
	*/
	template< typename THost, typename... TAllowedFeatures >
	class FeatureComposition : private Black::NonTransferable, private Internal::PartitionMediator<Internal::PartitionMap<TAllowedFeatures...>>
	{
	// Construction and assignment.
	public:
		FeatureComposition();
		~FeatureComposition();

	// Public interface.
	public:
		// Assign the new component. Returns the actual copy of component if one already exists.
		template< typename TFeature >
		inline TFeature& ConstructFeature();

		// Delete the existing component. Does nothing if component already deleted.
		template< typename TFeature >
		inline void DestructFeature();

		// Delete all allocated components.
		inline void DestructAllFeatures();


		// Get the allocated component. Expects that component already assigned.
		template< typename TFeature >
		inline TFeature& GetFeature();

		// Get the allocated component. Expects that component already assigned.
		template< typename TFeature >
		inline const TFeature& GetFeature() const;

		// Query the component. `nullptr` will be returned if component not assigned.
		template< typename TFeature >
		inline TFeature* const QueryFeature();

		// Query the component. `nullptr` will be returned if component not assigned.
		template< typename TFeature >
		inline const TFeature* const QueryFeature() const;


		// Whether the component may be constructed according to placement rules.
		template< typename TFeature >
		inline const bool CanConstructFeature() const;

		// Whether the composition has allocated a component.
		template< typename TFeature >
		inline const bool HasFeature() const;

		// Whether the composition has allocated any of components.
		template< typename... TParts >
		inline const bool HasAnyFeature() const;

		// Whether the composition has allocated all the components.
		template< typename... TParts >
		inline const bool HasAllFeatures() const;

	// Private types.
	private:
		// Construction policy template.
		template< typename TFeature >
		using ConstructionPolicy		= Internal::PartConstructionPolicy<TFeature, THost>;

		// Part mapping.
		using PartitionMap				= Internal::PartitionMap<TAllowedFeatures...>;

		// Allocation mediator type.
		using AllocationMediator		= Internal::PartitionMediator<PartitionMap>;

		// List of used components.
		using Parts						= typename PartitionMap::Parts;

		// Collection of part intersection indices.
		using PartIntersectionIndices	= typename Internal::PartIntersectionHelper<Parts, TAllowedFeatures...>::Indices;

	// Private interface.
	private:
		// Get ordinal index of component.
		template< typename TFeature >
		static constexpr const size_t GetPartIndex()
		{
			static_assert( Parts::template IS_EXIST<TFeature>, "Invalid type of composition part." );
			return Parts::template INDEX_OF<TFeature>;
		}

		// Get the offset from host base to the base of composition mediator.
		inline const size_t GetHostOffset() const;
	};
}
}
}
