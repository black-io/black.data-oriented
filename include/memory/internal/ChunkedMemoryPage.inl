#pragma once


namespace Black
{
inline namespace DataOriented
{
inline namespace Memory
{
namespace Internal
{
	template< size_t CHUNKS_COUNT, size_t CHUNK_SIZE, size_t CHUNK_ALIGNMENT >
	ChunkedMemoryPage<CHUNKS_COUNT, CHUNK_SIZE, CHUNK_ALIGNMENT>::ChunkedMemoryPage()
	{
		constexpr size_t aligned_chunk_size = Black::GetAlignedSize( CHUNK_SIZE, CHUNK_ALIGNMENT );

		size_t current_chunk_offset = 0;
		for( auto& chunk_slot : m_free_chunks )
		{
			chunk_slot = Parent::GetMemory( current_chunk_offset );
			current_chunk_offset += aligned_chunk_size;
		}
	}

	template< size_t CHUNKS_COUNT, size_t CHUNK_SIZE, size_t CHUNK_ALIGNMENT >
	ChunkedMemoryPage<CHUNKS_COUNT, CHUNK_SIZE, CHUNK_ALIGNMENT>::~ChunkedMemoryPage()
	{
		EXPECTS( IsEmpty() );
	}

	template< size_t CHUNKS_COUNT, size_t CHUNK_SIZE, size_t CHUNK_ALIGNMENT >
	void* ChunkedMemoryPage<CHUNKS_COUNT, CHUNK_SIZE, CHUNK_ALIGNMENT>::Allocate()
	{
		EXPECTS( m_remaining_chunks > 0 );
		return m_free_chunks[ --m_remaining_chunks ];
	}

	template< size_t CHUNKS_COUNT, size_t CHUNK_SIZE, size_t CHUNK_ALIGNMENT >
	void ChunkedMemoryPage<CHUNKS_COUNT, CHUNK_SIZE, CHUNK_ALIGNMENT>::Free( Black::NotNull<void*> memory )
	{
		EXPECTS( m_remaining_chunks < CHUNKS_COUNT );
		EXPECTS( Parent::IsInside( memory.Get() ) );
		EXPECTS( ( reinterpret_cast<std::uintptr_t>( memory.Get() ) % CHUNK_ALIGNMENT ) == 0 );

		// Just put item back and shift the head of stack.
		m_free_chunks[ m_remaining_chunks++ ] = memory.Get<std::byte*>();
	}

	template< size_t CHUNKS_COUNT, size_t CHUNK_SIZE, size_t CHUNK_ALIGNMENT >
	const bool ChunkedMemoryPage<CHUNKS_COUNT, CHUNK_SIZE, CHUNK_ALIGNMENT>::HasEnoughMemory( const size_t size ) const
	{
		return size == CHUNK_SIZE;
	}

	template< size_t CHUNKS_COUNT, size_t CHUNK_SIZE, size_t CHUNK_ALIGNMENT >
	inline const bool ChunkedMemoryPage<CHUNKS_COUNT, CHUNK_SIZE, CHUNK_ALIGNMENT>::IsResidentChunk( Black::NotNull<void*> chunk ) const
	{
		CRET( IsEmpty(), false );
		CRET( !Parent::IsInside( chunk.Get() ), false );

		// Expecting that `chunk` is properly aligned.
		EXPECTS( ( reinterpret_cast<std::uintptr_t>( chunk.Get() ) % CHUNK_ALIGNMENT ) == 0 );
		return true;
	}
}
}
}
}
