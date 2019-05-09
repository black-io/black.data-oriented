#pragma once


namespace Black
{
inline namespace DataOriented
{
inline namespace TypeUtils
{
	// Collection of types which memory is combined in same block of memory.
	template< typename... TTypes >
	struct TypesUnion final
	{
		// Collection, where all inner collections are unfolded.
		using UnfoldedCollection	= typename Internal::TypesCollectionUnfoldHelper<TypesCollection<TTypes...>>::Collection;


		// Length of collection.
		static constexpr const size_t LENGTH	= sizeof...( TTypes );

		// Ordinal index of type. `UNDEFINED_INDEX` in case of type does not exist in collection.
		template< typename TType >
		static constexpr const size_t INDEX_OF	= Internal::TypesCollectionIndexHelper<0, TType, TypesCollection<TTypes...>>::RESULT;

		// Whether the type exist in collection.
		template< typename TType >
		static constexpr const bool IS_EXIST	= INDEX_OF<TType> != Black::UNDEFINED_INDEX;
	};
}
}
}
