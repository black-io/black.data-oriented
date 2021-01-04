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
		@brief	Collection of raw memory pages.

		@note		This collection tends to reduce the number of memory allocations and stores up to predefined number of empty pages,
					which may be reused in future.
		@warning	All chunks should be returned to collection before the collection may be destroyed.

		@tparam	RAW_MEMORY_SIZE		Desired size of page memory.
		@tparam	MEMORY_ALIGNMENT	Basic alignment of memory.
		@tparam	MAX_FREE_PAGES		Number of empty pages, that will not be destroyed after it refined.
	*/
	template< size_t RAW_MEMORY_SIZE, size_t MEMORY_ALIGNMENT, size_t MAX_FREE_PAGES >
	class RawMemoryCollection final : private Black::NonTransferable
	{
	// Inner public types.
	public:
		// The type of memory pages this collection stores.
		using MemoryPage		= RawMemoryPage<RAW_MEMORY_SIZE, MEMORY_ALIGNMENT>;

		// Shared memory page.
		using SharedMemoryPage	= std::shared_ptr<MemoryPage>;

	// Construction and initialization.
	public:
		RawMemoryCollection();
		~RawMemoryCollection();

	// Public interface.
	public:
		// Grab the memory page, which can allocate `required_size` of memory.
		inline const SharedMemoryPage& RetainMemoryPage( const size_t required_size );

		// Return the empty memory page back to collection. Deletes `memory_page` according to internal logics.
		inline void ReleaseMemoryPage( const SharedMemoryPage& memory_page );

		// Clear unused memory.
		inline void Refine();

	// Private interface.
	private:
		// Pull the empty memory page back to used pages and return it's shared pointer.
		inline const SharedMemoryPage& PullEmptyPage();

		// Allocate the new empty page as already used and return it's shared pointer.
		inline const SharedMemoryPage& AllocateNewPage();

	// Private state.
	private:
		std::deque<SharedMemoryPage>	m_used_pages;	// Collection of currently used pages.
		std::vector<SharedMemoryPage>	m_free_pages;	// Collection of empty pages.

	// Private non-state.
	private:
		static constexpr const char* LOG_CHANNEL = "Black/Memory/RawMemoryCollection";
	};
}
}
}
}
