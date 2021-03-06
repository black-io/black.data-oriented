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
	RawMemoryPage<RAW_MEMORY_SIZE, MEMORY_ALIGNMENT>::~RawMemoryPage()
	{
		EXPECTS( IsEmpty() );
	}

	template< size_t RAW_MEMORY_SIZE, size_t MEMORY_ALIGNMENT >
	inline void RawMemoryPage<RAW_MEMORY_SIZE, MEMORY_ALIGNMENT>::Refine()
	{
		m_unused_head	= Parent::GetMemory( 0 );
		m_blocks_count	= 0;
	}

	template< size_t RAW_MEMORY_SIZE, size_t MEMORY_ALIGNMENT >
	inline void* RawMemoryPage<RAW_MEMORY_SIZE, MEMORY_ALIGNMENT>::Allocate( const size_t size )
	{
		CRET( !HasEnoughMemory( size ), nullptr );
		auto current_head	= m_unused_head;
		m_unused_head		= reinterpret_cast<std::byte*>( m_unused_head ) + Black::GetAlignedSize( size, MEMORY_ALIGNMENT );
		++m_blocks_count;
		return current_head;
	}

	template< size_t RAW_MEMORY_SIZE, size_t MEMORY_ALIGNMENT >
	inline void RawMemoryPage<RAW_MEMORY_SIZE, MEMORY_ALIGNMENT>::Free( Black::NotNull<void*> memory )
	{
		EXPECTS_DEBUG( memory.Get<std::byte*>() >= Parent::GetBeginAddress() );
		EXPECTS_DEBUG( memory.Get<std::byte*>() < m_unused_head );

		EXPECTS_DEBUG( m_blocks_count > 0 );
		--m_blocks_count;

		CRET( m_blocks_count > 0 );
		Refine();
	}

	template< size_t RAW_MEMORY_SIZE, size_t MEMORY_ALIGNMENT >
	inline const bool RawMemoryPage<RAW_MEMORY_SIZE, MEMORY_ALIGNMENT>::HasEnoughMemory( const size_t size ) const
	{
		return ( reinterpret_cast<std::byte*>( m_unused_head ) + size ) <= Parent::GetEndAddress();
	}
}
}
}
}
