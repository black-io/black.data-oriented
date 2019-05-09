#pragma once


namespace Black
{
inline namespace DataOriented
{
inline namespace TypeUtils
{
	// Forward declaration for types collection.
	template< typename... TTypes >
	struct TypesCollection;

	// Forward declaration for types union.
	template< typename... TTypes >
	struct TypesUnion;


namespace Internal
{
	// Helper class to find the ordinal index of type in collection.
	template< size_t BASE_INDEX, typename TSearchedType, typename TCollection >
	struct TypesCollectionIndexHelper;

	// Deduction branch. Keep looking for type in collection.
	template< size_t BASE_INDEX, typename TSearchedType, typename THead, typename... TRest >
	struct TypesCollectionIndexHelper<BASE_INDEX, TSearchedType, TypesCollection<THead, TRest...>> final
	{
		// We need to go deeper.
		static constexpr const size_t RESULT	= TypesCollectionIndexHelper<BASE_INDEX + 1, TSearchedType, TypesCollection<TRest...>>::RESULT;
	};

	// Terminal branch. Returning the index of searched type.
	template< size_t BASE_INDEX, typename TSearchedType, typename... TTypes >
	struct TypesCollectionIndexHelper<BASE_INDEX, TSearchedType, TypesCollection<TSearchedType, TTypes...>> final
	{
		// Index of searched type.
		static constexpr const size_t RESULT	= BASE_INDEX;
	};

	// Terminal branch. No type found.
	template< size_t BASE_INDEX, typename TSearchedType >
	struct TypesCollectionIndexHelper<BASE_INDEX, TSearchedType, TypesCollection<>> final
	{
		// The index of searched type is undefined.
		static constexpr const size_t RESULT	= Black::UNDEFINED_INDEX;
	};

	// Helper to unfold the types collection.
	template< typename TCollection, typename... TUnfoldedTypes >
	struct TypesCollectionUnfoldHelper;

	// Deduction branch. Regular unfolding.
	template< typename TRawHead, typename... TRawRest, typename... TUnfoldedTypes >
	struct TypesCollectionUnfoldHelper<TypesCollection<TRawHead, TRawRest...>, TUnfoldedTypes...>
		: TypesCollectionUnfoldHelper<TypesCollection<TRawRest...>, TUnfoldedTypes..., TRawHead>
	{
		static_assert( !Black::IS_VOID<TRawHead>, "The type should not be void." );
		static_assert( !Black::IS_CONST<TRawHead>, "The type should not be const." );
		static_assert( !Black::IS_POINTER<TRawHead>, "The type should not be pointer." );
		static_assert( !Black::IS_REFERENCE<TRawHead>, "The type should not be reference." );
	};

	// Deduction branch. Unfold the inner collection.
	template< typename... TInnerRawTypes, typename... TRawRest, typename... TUnfoldedTypes >
	struct TypesCollectionUnfoldHelper<TypesCollection<TypesCollection<TInnerRawTypes...>, TRawRest...>, TUnfoldedTypes...>
		: TypesCollectionUnfoldHelper<TypesCollection<TInnerRawTypes..., TRawRest...>, TUnfoldedTypes...>
	{

	};

	// Deduction branch. Unfold the inner union.
	template< typename... TInnerRawTypes, typename... TRawRest, typename... TUnfoldedTypes >
	struct TypesCollectionUnfoldHelper<TypesCollection<TypesUnion<TInnerRawTypes...>, TRawRest...>, TUnfoldedTypes...>
		: TypesCollectionUnfoldHelper<TypesCollection<TInnerRawTypes..., TRawRest...>, TUnfoldedTypes...>
	{

	};

	// Terminal branch. The collection is unfolded.
	template< typename... TUnfoldedTypes >
	struct TypesCollectionUnfoldHelper<TypesCollection<>, TUnfoldedTypes...>
	{
		// Unfolded collection.
		using Collection = TypesCollection<TUnfoldedTypes...>;
	};
}
}
}
}
