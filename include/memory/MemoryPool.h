#pragma once


namespace Black
{
inline namespace DataOriented
{
inline namespace Memory
{
	/// @brief	Pool of raw and aligned memory, which can be used to store arbitrary object via shared owning.
	template< size_t RAW_MEMORY_SIZE, size_t MAX_FREE_PAGES = 1, size_t ALIGNMENT = 16 >
	class MemoryPool final : private Black::NonTransferable, private std::shared_ptr<Internal::RawMemoryPageCollection<RAW_MEMORY_SIZE, MAX_FREE_PAGES, ALIGNMENT>>
	{
	private:
		using Parent = std::shared_ptr< Internal::RawMemoryPageCollection<RAW_MEMORY_SIZE, MAX_FREE_PAGES, ALIGNMENT> >;

	public:
		MemoryPool() : Parent( std::make_shared<Internal::RawMemoryPageCollection<RAW_MEMORY_SIZE, MAX_FREE_PAGES, ALIGNMENT>>() ) {};

		template< typename TObjectType, typename... TConstructionArguments >
		inline std::shared_ptr<TObjectType> ConstructObject( TConstructionArguments&&... construction_arguments )
		{
			using Allocator = Internal::RawMemoryObjectAllocator<TObjectType, Internal::RawMemoryPageCollection<RAW_MEMORY_SIZE, MAX_FREE_PAGES, ALIGNMENT>>;

			return std::allocate_shared<TObjectType>( Allocator{ *this }, std::forward<TConstructionArguments>( construction_arguments )... );
		}
	};
}
}
}
