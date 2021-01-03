#pragma once


namespace Black
{
inline namespace DataOriented
{
inline namespace Memory
{
namespace Internal
{
	/// @brief	The page of aligned memory, which can store objects of arbitrary size.
	template< size_t RAW_MEMORY_SIZE, size_t MEMORY_ALIGNMENT >
	class RawAlignedMemoryPage final : public AlignedMemoryPage<RAW_MEMORY_SIZE, MEMORY_ALIGNMENT>
	{
	public:
		/// @brief	Alias for parent class.
		using Parent = AlignedMemoryPage<RAW_MEMORY_SIZE, MEMORY_ALIGNMENT>;

		virtual ~RawAlignedMemoryPage();

		/// @brief	Refine the memory of page, considering it unused after.
		inline void Refine();

		/// @brief	Retain memory of given size from page.
		inline void* RetainMemory( const size_t size );

		/// @see	BasicMemoryPage::ReleaseMemory
		inline void ReleaseMemory( Black::NotNull<void*> item );

		/// @brief	Checks the page has enough memory for placing given size.
		inline const bool HasEnoughMemory( const size_t size ) const;

		/// @brief	Check, is this page already empty (no pointer was grabbed from page)?
		inline const bool IsEmpty() const	{ return m_blocks_count == 0; };

	private:
		size_t		m_blocks_count	= 0;						// Count of currently allocated blocks.
		uint8_t*	m_unused_head	= Parent::GetMemoryBegin();	// The pointer to first unused byte of page.
	};
}
}
}
}
