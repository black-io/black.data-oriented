#pragma once


namespace Black
{
inline namespace DataOriented
{
inline namespace Memory
{
namespace Internal
{
	template< size_t RAW_MEMORY_SIZE, size_t MEMORY_ALIGNMENT >
	RawAlignedMemoryPage<RAW_MEMORY_SIZE, MEMORY_ALIGNMENT>::~RawAlignedMemoryPage()
	{
		EXPECTS( IsEmpty() );
	}

	template< size_t RAW_MEMORY_SIZE, size_t MEMORY_ALIGNMENT >
	inline void RawAlignedMemoryPage<RAW_MEMORY_SIZE, MEMORY_ALIGNMENT>::Refine()
	{
		m_blocks_count	= 0;
		m_unused_head	= Parent::GetMemoryBegin();
	}

	template< size_t RAW_MEMORY_SIZE, size_t MEMORY_ALIGNMENT >
	inline void* RawAlignedMemoryPage<RAW_MEMORY_SIZE, MEMORY_ALIGNMENT>::RetainMemory( const size_t size )
	{
		CRET( !HasEnoughMemory( size ), nullptr );
		auto current_head	= m_unused_head;
		m_unused_head		+= Black::GetAlignedSize( size, MEMORY_ALIGNMENT );
		++m_blocks_count;
		return current_head;
	}

	template< size_t RAW_MEMORY_SIZE, size_t MEMORY_ALIGNMENT >
	inline void RawAlignedMemoryPage<RAW_MEMORY_SIZE, MEMORY_ALIGNMENT>::ReleaseMemory( Black::NotNull<void*> item )
	{
		EXPECTS_DEBUG( item.Get<uint8_t*>() >= Parent::GetMemoryBegin() );
		EXPECTS_DEBUG( item.Get<uint8_t*>() < Parent::GetMemoryEnd() );

		EXPECTS_DEBUG( m_blocks_count > 0 );
		--m_blocks_count;

		CRET( m_blocks_count > 0 );
		Refine();
	}

	template< size_t RAW_MEMORY_SIZE, size_t MEMORY_ALIGNMENT >
	inline const bool RawAlignedMemoryPage<RAW_MEMORY_SIZE, MEMORY_ALIGNMENT>::HasEnoughMemory( const size_t size ) const
	{
		return ( m_unused_head + size ) <= Parent::GetMemoryEnd();
	}
}
}
}
}
