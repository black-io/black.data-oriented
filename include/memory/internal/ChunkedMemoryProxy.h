#pragma once


namespace Black
{
inline namespace DataOriented
{
inline namespace Memory
{
namespace Internal
{
	// Proxy type to hide the concrete memory collection.
	template< size_t CHUNKS_PER_PAGE, size_t MAX_FREE_PAGES, size_t MEMORY_ALIGNMENT >
	class ChunkedMemoryProxy final : private Black::NonCopyable
	{
	// Public inner types.
	public:
		// Dedicated state of allocator, that is required due to nature of standard allocators.
		struct AllocatorState final
		{
			std::shared_ptr<BasicChunkedMemoryPage>		host_memory_page;	// Host of allocated memory.
			std::weak_ptr<BasicChunkedMemoryCollection>	host_collection;	// Host memory collection.
		};

	// Public interface.
	public:
		// Get the memory collection, that can store objects of required type.
		template< typename TObject >
		inline const std::shared_ptr<BasicChunkedMemoryCollection>& GetMemoryCollection();


		// Whether the collection store no used pages.
		inline const bool IsEmpty() const;

	// Private inner types.
	private:
		// Memory collection template for type of desired objects to store.
		template< typename TObject >
		using MemoryCollection = ChunkedMemoryCollection<CHUNKS_PER_PAGE, sizeof( TObject ), std::max( alignof( TObject ), MEMORY_ALIGNMENT ), MAX_FREE_PAGES>;

	// Private state.
	private:
		std::shared_ptr<BasicChunkedMemoryCollection>	m_collection;			// Actual stored collection.
		size_t											m_chunk_size	= 0;	// Size of chunks in stored collection.
	};
}
}
}
}
