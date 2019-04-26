#pragma once


namespace Black
{
inline namespace DataOriented
{
inline namespace Composition
{
namespace Internal
{
	/// @brief	Compile-time representation for memory chunk information.
	template< size_t CHUNK_SIZE, size_t CHUNK_ALIGNMENT, size_t CHUNK_OFFSET >
	struct MemoryChunkInfo final
	{
		/// @brief	Specified size of chunk.
		static constexpr const size_t SIZE		= CHUNK_SIZE;

		/// @brief	Specified alignment of chunk.
		static constexpr const size_t ALIGNMENT	= CHUNK_ALIGNMENT;

		/// @brief	Specified internal offset of chunk.
		static constexpr const size_t OFFSET	= CHUNK_OFFSET;
	};

	/// @brief	Trivial list of components.
	template< typename... TComponents >
	struct ComponentCollection final
	{
		static constexpr const size_t LENGTH = sizeof...( TComponents );
	};

	/// @brief	Component indexing helper. Used to associate the component type with ordinal index in list.
	template< typename TComponent, typename TList, size_t BASE_INDEX >
	struct ComponentIndexHelper final
	{
		static const size_t Find();
	};

	/// @brief	Terminal branch. The component is not found.
	template< typename TComponent, size_t BASE_INDEX >
	struct ComponentIndexHelper<TComponent, ComponentCollection<>, BASE_INDEX> final
	{
		static constexpr const size_t Find()	{ return Black::UNDEFINED_INDEX; };
	};

	/// @brief	Terminal branch. The component was found.
	template< typename TComponent, size_t BASE_INDEX, typename... TRest >
	struct ComponentIndexHelper<TComponent, ComponentCollection<TComponent, TRest...>, BASE_INDEX> final
	{
		static constexpr const size_t Find()	{ return BASE_INDEX; };
	};

	/// @brief	Deduction branch. Looking for the component in list.
	template< typename TComponent, size_t BASE_INDEX, typename TCurrent, typename... TRest >
	struct ComponentIndexHelper<TComponent, ComponentCollection<TCurrent, TRest...>, BASE_INDEX> final
	{
		using NextHelper = typename ComponentIndexHelper<TComponent, typename ComponentCollection<TRest...>, BASE_INDEX + 1>;

		static constexpr const size_t Find()	{ return NextHelper::Find(); };
	};
}
}
}
}
