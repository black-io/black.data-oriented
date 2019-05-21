#pragma once


namespace Black
{
inline namespace DataOriented
{
inline namespace CompositionOverInheritance
{
namespace Internal
{
	// Component offset builder.
	template< size_t BASE_OFFSET, typename... TTypes >
	struct TypesCollectionOffsetBuilder;

	// Deduction branch. Offset for regular type.
	template< size_t BASE_OFFSET, typename THead, typename... TRest >
	struct TypesCollectionOffsetBuilder<BASE_OFFSET, THead, TRest...> final
	{
		// Current offset information.
		using OffsetInfo	= TypeOffset<THead, BASE_OFFSET>;

		// Collection tail information.
		using RestInfo		= TypesCollectionOffsetBuilder<OffsetInfo::NEXT_OFFSET, TRest...>;

		// Collection offsets.
		using Offsets		= typename NumericCollection<OffsetInfo::OFFSET>::template MergeTailCollection<typename RestInfo::Offsets>;
	};

	// Deduction branch. Offset for regular type.
	template< size_t BASE_OFFSET, typename... TInnerTypes, typename... TRest >
	struct TypesCollectionOffsetBuilder<BASE_OFFSET, Black::TypesCollection<TInnerTypes...>, TRest...> final
	{
		// Current offset information.
		using OffsetInfo	= TypeOffset<Black::TypesCollection<TInnerTypes...>, BASE_OFFSET>;

		// Collection tail information.
		using RestInfo		= TypesCollectionOffsetBuilder<OffsetInfo::NEXT_OFFSET, TRest...>;

		// Inner offsets.
		using InnerOffsets	= typename TypesCollectionOffsetBuilder<BASE_OFFSET, Black::TypesCollection<TInnerTypes...>>::Offsets;

		// Collection offsets.
		using Offsets		= typename InnerOffsets::template MergeTailCollection<typename RestInfo::Offsets>;
	};

	// Deduction branch. Offset for regular type.
	template< size_t BASE_OFFSET, typename... TInnerTypes, typename... TRest >
	struct TypesCollectionOffsetBuilder<BASE_OFFSET, Black::TypesUnion<TInnerTypes...>, TRest...> final
	{
		// Current offset information.
		using OffsetInfo	= TypeOffset<Black::TypesUnion<TInnerTypes...>, BASE_OFFSET>;

		// Collection tail information.
		using RestInfo		= TypesCollectionOffsetBuilder<OffsetInfo::NEXT_OFFSET, TRest...>;

		// Inner offsets.
		using InnerOffsets	= typename TypesCollectionOffsetBuilder<BASE_OFFSET, Black::TypesUnion<TInnerTypes...>>::Offsets;

		// Collection offsets.
		using Offsets		= typename InnerOffsets::template MergeTailCollection<typename RestInfo::Offsets>;
	};

	// Terminal branch. Offsets for last collection.
	template< size_t BASE_OFFSET, typename... TInnerTypes >
	struct TypesCollectionOffsetBuilder<BASE_OFFSET, Black::TypesCollection<TInnerTypes...>> final
	{
		// Information about offset of collection.
		using OffsetInfo	= TypeOffset<Black::TypesCollection<TInnerTypes...>, BASE_OFFSET>;

		// Collection offsets.
		using Offsets		= typename TypesCollectionOffsetBuilder<OffsetInfo::OFFSET, TInnerTypes...>::Offsets;
	};

	// Terminal branch. Offsets for last union.
	template< size_t BASE_OFFSET, typename... TInnerTypes >
	struct TypesCollectionOffsetBuilder<BASE_OFFSET, Black::TypesUnion<TInnerTypes...>> final
	{
		// Information about offset of union.
		using OffsetInfo	= TypeOffset<Black::TypesUnion<TInnerTypes...>, BASE_OFFSET>;

		// Collection offsets.
		using Offsets		= typename NumericCollectionMergeHelper<
			typename TypesCollectionOffsetBuilder<OffsetInfo::OFFSET, TInnerTypes>::Offsets...
		>::Collection;
	};

	// Terminal branch. The last type found.
	template< size_t BASE_OFFSET, typename TLast >
	struct TypesCollectionOffsetBuilder<BASE_OFFSET, TLast> final
	{
		// Information about offset of current type.
		using OffsetInfo	= TypeOffset<TLast, BASE_OFFSET>;

		// Collection offsets.
		using Offsets		= NumericCollection<OffsetInfo::OFFSET>;
	};
}
}
}
}
