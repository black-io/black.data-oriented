#pragma once


namespace Black
{
inline namespace DataOriented
{
inline namespace Memory
{
namespace Internal
{
	// The page of aligned memory, which can store objects of arbitrary size.
	template< size_t RAW_MEMORY_SIZE, size_t MEMORY_ALIGNMENT >
	class RawAlignedMemoryPage final : private Black::RawMemoryBlock<RAW_MEMORY_SIZE, MEMORY_ALIGNMENT>
	{
	// Construction and initialization.
	public:
		virtual ~RawAlignedMemoryPage();

	// Public interface.
	public:
		// Retain the memory of given size from page.
		inline void* RetainMemory( const size_t size );

		// Return the memory back to the page.
		inline void ReleaseMemory( Black::NotNull<void*> item );


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
		std::byte*	m_unused_head	= Parent::GetBeginAddress();	// The pointer to first unused byte of page.
		size_t		m_blocks_count	= 0;							// Count of currently allocated blocks.
	};
}
}
}
}
