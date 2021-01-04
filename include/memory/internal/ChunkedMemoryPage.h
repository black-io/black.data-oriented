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
		@brief	Representation of number of chunks, stored in solid block of memory.

		The chunk stored in this page represents only the storage for some unknown object, but not the object itself. The memory page does not construct
		or destruct the objects in its memory.

		@warning	Any address, grabbed from page, must be returned to page back before page will be destroyed.

		@tparam	CHUNKS_COUNT	The number of chunks the page stores.
		@tparam	CHUNK_SIZE		Size of single chunk.
		@tparam	CHUNK_ALIGNMENT	Alignment of single chunk. Only positive `PoT` values, beginning from `1`, allowed.
	*/
	template< size_t CHUNKS_COUNT, size_t CHUNK_SIZE, size_t CHUNK_ALIGNMENT >
	class ChunkedMemoryPage final
		: public BasicMemoryPage
		, private Black::RawMemoryBlock<Black::GetAlignedSize( CHUNK_SIZE, CHUNK_ALIGNMENT ) * CHUNKS_COUNT, CHUNK_ALIGNMENT>
	{
	// Construction and initialization.
	public:
		ChunkedMemoryPage();
		virtual ~ChunkedMemoryPage();

	// Public interface.
	public:
		/// @see	BasicMemoryPage::RetainMemory
		void* Allocate() override;

		/// @see	BasicMemoryPage::ReleaseMemory
		void Free( Black::NotNull<void*> memory ) override;


		// Whether the chunk belongs to this page.
		inline const bool IsResidentChunk( Black::NotNull<void*> chunk ) const;

		// Get the count of memory pointers remaining in this page.
		inline const size_t GetRemainingChunks() const	{ return m_remaining_chunks; };

		// Whether the page is empty (i.e. no chunks allocated).
		inline const bool IsEmpty() const				{ return m_remaining_chunks == CHUNKS_COUNT; };

		// Whether the page already full (i.e. no free chunk remains).
		inline const bool IsFull() const				{ return m_remaining_chunks == 0; };

		// Whether the page holds some free chunks.
		inline const bool HasFreeChunks() const			{ return m_remaining_chunks > 0; };


	// Private inner types.
	private:
		using Parent = Black::RawMemoryBlock<Black::GetAlignedSize( CHUNK_SIZE, CHUNK_ALIGNMENT ) * CHUNKS_COUNT, CHUNK_ALIGNMENT>;

	// Private state.
	private:
		std::byte*	m_free_chunks[ CHUNKS_COUNT ];		// Stack of free chunks.
		size_t		m_remaining_chunks = CHUNKS_COUNT;	// Head of free items stack. Actually stores count of free items.
	};
}
}
}
}
