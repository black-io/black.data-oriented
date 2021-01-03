#pragma once


namespace Black
{
inline namespace DataOriented
{
inline namespace Memory
{
namespace Internal
{
	/// @brief	Collection of memory pages with raw memory.
	template< size_t RAW_MEMORY_SIZE, size_t MAX_FREE_PAGES, size_t ALIGNMENT >
	class RawMemoryPageCollection final : private Black::NonTransferable
	{
	public:
		/// @brief	The type of memory pages this collection stores.
		using MemoryPage		= RawAlignedMemoryPage<RAW_MEMORY_SIZE, ALIGNMENT>;

		/// @brief	Shared memory page.
		using SharedMemoryPage	= std::shared_ptr<MemoryPage>;

		RawMemoryPageCollection();
		~RawMemoryPageCollection();

		/// @brief	Returns the memory page, which can allocate `required_size` of memory.
		inline const SharedMemoryPage& RetainMemoryPage( const size_t required_size );

		/// @brief	Releases the memory page. Deletes `memory_page` according to internal logics.
		inline void ReleaseMemoryPage( const SharedMemoryPage& memory_page );

	private:
		inline SharedMemoryPage& PullEmptyPage();
		inline SharedMemoryPage& AllocateNewPage();

	private:
		std::deque<SharedMemoryPage>	m_used_pages;
		std::vector<SharedMemoryPage>	m_free_pages;
	};
}
}
}
}
