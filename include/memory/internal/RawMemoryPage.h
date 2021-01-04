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
		@brief	Representation of trivial memory arena.

		This page can allocate arbitrary size of memory, each allocated pointer will be aligned with page alignment.
		Any allocated memory may be returned to page at any time, but the page will not reuse it on allocation until be refined.
		The memory page may be refined manually using corresponded function of automatically when all the memory is returned to page.
		Such behavior may be used to track the memory access violations and other methods of inappropriate memory usage tracking.

		@tparam	RAW_MEMORY_SIZE		Desired size of page memory.
		@tparam	MEMORY_ALIGNMENT	Basic alignment of memory.
	*/
	template< size_t RAW_MEMORY_SIZE, size_t MEMORY_ALIGNMENT >
	class RawMemoryPage final : private Black::RawMemoryBlock<RAW_MEMORY_SIZE, MEMORY_ALIGNMENT>
	{
	// Construction and initialization.
	public:
		virtual ~RawMemoryPage();

	// Public interface.
	public:
		// Grab the memory of given size from page.
		inline void* Allocate( const size_t size );

		// Return the memory back to the page.
		inline void Free( Black::NotNull<void*> memory );


		// Refine the memory of page, considering it unused after.
		inline void Refine();


		// Whether the page has enough memory for placing given size.
		inline const bool HasEnoughMemory( const size_t size ) const;

		// Whether the page already empty (no pointer was retained from page)?
		inline const bool IsEmpty() const	{ return m_blocks_count == 0; };

	// Private inner types.
	private:
		/// @brief	Alias for parent class.
		using Parent = Black::RawMemoryBlock<RAW_MEMORY_SIZE, MEMORY_ALIGNMENT>;

	// Private state.
	private:
		void*	m_unused_head	= Parent::GetMemory( 0 );	// The pointer to first unused byte of page.
		size_t	m_blocks_count	= 0;						// Count of currently allocated blocks.
	};
}
}
}
}
