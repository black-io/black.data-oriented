#pragma once


namespace Black
{
inline namespace DataOriented
{
inline namespace Memory
{
namespace Internal
{
	template< typename TProduct, typename TMemoryCollection >
	RawMemoryAllocator<TProduct, TMemoryCollection>::RawMemoryAllocator( const std::shared_ptr<TMemoryCollection>& collection )
		: m_shared_state{ std::make_shared<AllocatorState>() }
	{
		m_shared_state->collection = collection;
	}

	template< typename TProduct, typename TMemoryCollection >
	template< typename TOtherProduct, typename >
	RawMemoryAllocator<TProduct, TMemoryCollection>::RawMemoryAllocator( const RawMemoryAllocator<TOtherProduct, TMemoryCollection>& other )
		: m_shared_state{ other.GetSharedState() }
	{
	}

	template< typename TProduct, typename TMemoryCollection >
	inline typename RawMemoryAllocator<TProduct, TMemoryCollection>::value_type*
	RawMemoryAllocator<TProduct, TMemoryCollection>::allocate( const size_t sequence_length )
	{
		EXPECTS( sequence_length == 1 );
		EnsureEnoughMemory();

		value_type* memory = reinterpret_cast<value_type*>( m_shared_state->memory_page->Allocate( sizeof( TProduct ) ) );
		ENSURES( memory != nullptr );
		return memory;
	}

	template< typename TProduct, typename TMemoryCollection >
	inline void RawMemoryAllocator<TProduct, TMemoryCollection>::deallocate( value_type* memory, const size_t sequence_length ) noexcept
	{
		EXPECTS( sequence_length == 1 );
		EXPECTS( m_shared_state != nullptr );
		EXPECTS( m_shared_state->memory_page != nullptr );

		m_shared_state->memory_page->Free( memory );
		m_shared_state->collection->ReleaseMemoryPage( m_shared_state->memory_page );
		m_shared_state->memory_page.reset();
		m_shared_state.reset();
	}

	template< typename TProduct, typename TMemoryCollection >
	void RawMemoryAllocator<TProduct, TMemoryCollection>::EnsureEnoughMemory() const
	{
		EXPECTS( m_shared_state != nullptr );
		EXPECTS( m_shared_state->collection != nullptr );

		if( !m_shared_state->memory_page )
		{
			m_shared_state->memory_page = m_shared_state->collection->RetainMemoryPage( sizeof( TProduct ) );
		}

		ENSURES( m_shared_state->memory_page && m_shared_state->memory_page->HasEnoughMemory( sizeof( TProduct ) ) );
	}
}
}
}
}
