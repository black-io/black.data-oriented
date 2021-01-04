#pragma once


namespace Black
{
inline namespace DataOriented
{
inline namespace Memory
{
namespace Internal
{
	// Collection of memory pages with raw memory.
	template< size_t RAW_MEMORY_SIZE, size_t MAX_FREE_PAGES, size_t ALIGNMENT >
	class RawMemoryPageCollection final : private Black::NonTransferable
	{
	// Inner public types.
	public:
		// The type of memory pages this collection stores.
		using MemoryPage		= RawAlignedMemoryPage<RAW_MEMORY_SIZE, ALIGNMENT>;

		// Shared memory page.
		using SharedMemoryPage	= std::shared_ptr<MemoryPage>;

	// Construction and initialization.
	public:
		RawMemoryPageCollection();
		~RawMemoryPageCollection();

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
		inline SharedMemoryPage& PullEmptyPage();

		// Allocate the new empty page as already used and return it's shared pointer.
		inline SharedMemoryPage& AllocateNewPage();

	// Private state.
	private:
		static constexpr const char*	LOG_CHANNEL		= "Black/Memory/RawMemoryPageCollection";

		std::deque<SharedMemoryPage>	m_used_pages;	// Collection of currently used pages.
		std::vector<SharedMemoryPage>	m_free_pages;	// Collection of empty pages.
	};
}
}
}
}
