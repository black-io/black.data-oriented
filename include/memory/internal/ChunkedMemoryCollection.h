#pragma once


namespace Black
{
inline namespace DataOriented
{
inline namespace Memory
{
namespace Internal
{
	/**
		@brief	Collection of chunked memory pages.

		This collection operates with fixed-size chunks of memory. It may be used to store the memory of objects with same size and appropriate alignment.
		But collection does mot track the lifetime of objects stored in controlled memory.

		@note		This collection tends to reduce the number of memory allocations and stores up to predefined number of empty pages,
					which may be reused in future.
		@warning	All chunks should be returned to collection before the collection may be destroyed.

		@tparam	CHUNKS_COUNT		The number of chunks the page stores.
		@tparam	CHUNK_SIZE			Size of single chunk.
		@tparam	CHUNK_ALIGNMENT		Alignment of single chunk. Only positive `PoT` values, beginning from `1`, allowed.
		@tparam	MAX_FREE_PAGES		Number of empty pages, that will not be destroyed after it refined.
	*/
	template< size_t CHUNKS_COUNT, size_t CHUNK_SIZE, size_t CHUNK_ALIGNMENT, size_t MAX_FREE_PAGES >
	class ChunkedMemoryCollection final : public BasicChunkedMemoryCollection
	{
	// Inner public types.
	public:
		// The type of memory pages this collection stores.
		using MemoryPage		= ChunkedMemoryPage<CHUNKS_COUNT, CHUNK_SIZE, CHUNK_ALIGNMENT>;

		// Shared memory page.
		using SharedMemoryPage	= std::shared_ptr<MemoryPage>;

	// Construction and initialization.
	public:
		ChunkedMemoryCollection();
		virtual ~ChunkedMemoryCollection();

	// Public interface.
	public:
		// Grab the memory of single chunk.
		inline void* AllocateChunk();

		// Return the memory of single chunk back to collection.
		inline void FreeChunk( Black::NotNull<void*> memory );


		// Grab the memory page that can allocate at last one memory chunk.
		inline const SharedMemoryPage& RetainMemoryPage();

		// Return the memory page back to collection. The behavior is abstractly similar to `FreeChunk`, but only tells that memory returned to page.
		inline void ReleaseMemoryPage( const SharedMemoryPage& used_page );

		// Proxy function that just cast `used_page` to `SharedMemoryPage` type.
		inline void ReleaseMemoryPage( const std::shared_ptr<BasicChunkedMemoryPage>& used_page );

	// Private interface.
	private:
		// Pull the empty memory page back to used pages and return it's shared pointer.
		inline const SharedMemoryPage& PullEmptyPage();

		// Allocate the new empty page as already used and return it's shared pointer.
		inline const SharedMemoryPage& AllocateNewPage();

		// Perform the maintenance of memory used page.
		inline void MaintainUsedMemory( typename std::deque<SharedMemoryPage>::iterator page_slot );

	// Private state.
	private:
		std::deque<SharedMemoryPage>	m_used_pages;	// Collection of currently used pages.
		std::vector<SharedMemoryPage>	m_free_pages;	// Collection of empty pages.

	// Private non-state.
	private:
		static constexpr const char* LOG_CHANNEL = "Black/Memory/ChunkedMemoryCollection";
	};
}
}
}
}
