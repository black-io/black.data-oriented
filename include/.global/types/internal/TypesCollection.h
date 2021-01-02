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

	// Helper class to merge two collections.
	template< typename TLeftColection, typename TRightCollection >
	struct TypesCollectionMergeHelper;

	// Terminal branch. Merge the collections.
	template< typename... TLeftTypes, typename... TRightTypes >
	struct TypesCollectionMergeHelper<Black::TypesCollection<TLeftTypes...>, Black::TypesCollection<TRightTypes...>> final
	{
		// The merged collection.
		using Collection = Black::TypesCollection<TLeftTypes..., TRightTypes...>;
	};

	// Helper class to repeat the types in collection given number of times.
	template< size_t REPEAT_COUNT, typename... TTypes >
	struct TypesCollectionRepeatHelper final
	{
		static_assert( REPEAT_COUNT != Black::UNDEFINED_INDEX, "The number of collection repeats is invalid." );

		// Current repeat.
		using HeadCollection	= Black::TypesCollection<TTypes...>;

		// The tail of all repeats.
		using RestHelper		= TypesCollectionRepeatHelper<REPEAT_COUNT - 1, TTypes...>;

		// The result collection.
		using Collection		= typename TypesCollectionMergeHelper<HeadCollection, typename RestHelper::Collection>::Collection;
	};

	// Terminal branch. The last repeat.
	template< typename... TTypes >
	struct TypesCollectionRepeatHelper<0, TTypes...> final
	{
		// The result collection.
		using Collection		= Black::TypesCollection<TTypes...>;
	};

	// Type isolation wrap.
	template< typename TContent >
	struct TypeWrap final
	{
		// Content of wrap.
		using Content = TContent;
	};

	// Helper class to extract N-th type from collection.
	template< size_t TYPE_INDEX, typename = std::make_index_sequence<TYPE_INDEX> >
	struct TypesCollectionSelectionHelper;

	// Terminal branch. Implement the type selection function.
	template< size_t TYPE_INDEX, size_t... INDICES >
	struct TypesCollectionSelectionHelper<TYPE_INDEX, std::index_sequence<INDICES...>> final
	{
		// The function will deduce the type of N-th argument in types collection.
		template< typename TResult >
		static inline TResult Select( decltype( (void*)INDICES )..., TResult*, ... );
	};

	// Helper class to access the type at index of types collection.
	template< size_t TYPE_INDEX, typename... TTypes >
	struct TypesCollectionAccessHelper final
	{
		// The type at index.
		using Result = typename decltype( TypesCollectionSelectionHelper<TYPE_INDEX>::template Select( static_cast<TypeWrap<TTypes>*>(0)... ) )::Content;
	};
}
}
}
}
