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
	ChunkedMemoryAllocator<TProduct, TMemoryCollection>::ChunkedMemoryAllocator( TMemoryCollection& collection )
		: m_collection{ collection }
		, m_shared_state{ std::make_shared<AllocatorState>() }
	{
	}

	template< typename TProduct, typename TMemoryCollection >
	template< typename TOtherProduct, typename >
	ChunkedMemoryAllocator<TProduct, TMemoryCollection>::ChunkedMemoryAllocator( const ChunkedMemoryAllocator<TOtherProduct, TMemoryCollection>& other )
		: m_collection{ other.GetMemoryCollection() }
		, m_shared_state{ other.GetSharedState() }
	{
	}

	template< typename TProduct, typename TMemoryCollection >
	inline typename ChunkedMemoryAllocator<TProduct, TMemoryCollection>::value_type*
	ChunkedMemoryAllocator<TProduct, TMemoryCollection>::allocate( const size_t sequence_length )
	{
		EXPECTS( sequence_length == 1 );
		EnsureEnoughMemory();

		value_type* memory = reinterpret_cast<value_type*>( m_shared_state->host_memory_page->Allocate() );
		ENSURES( memory != nullptr );
		return memory;
	}

	template< typename TProduct, typename TMemoryCollection >
	inline void ChunkedMemoryAllocator<TProduct, TMemoryCollection>::deallocate( value_type* memory, const size_t sequence_length ) noexcept
	{
		EXPECTS( sequence_length == 1 );
		EXPECTS( m_shared_state != nullptr );
		EXPECTS( m_shared_state->host_memory_page != nullptr );

		m_shared_state->host_memory_page->Free( memory );
		if( !m_shared_state->host_collection.expired() )
		{
			m_shared_state->host_collection.lock()->ReleaseMemoryPage( m_shared_state->host_memory_page );
		}

		m_shared_state->host_memory_page.reset();
		m_shared_state->host_collection.reset();
		m_shared_state.reset();
	}

	template< typename TProduct, typename TMemoryCollection >
	inline void ChunkedMemoryAllocator<TProduct, TMemoryCollection>::EnsureEnoughMemory() const
	{
		EXPECTS_DEBUG( m_shared_state != nullptr );

		const auto& collection = m_collection.template GetMemoryCollection<TProduct>();

		if( m_shared_state->host_collection.expired() )
		{
			m_shared_state->host_collection = collection;
		}

		if( m_shared_state->host_memory_page == nullptr )
		{
			m_shared_state->host_memory_page = collection->RetainMemoryPage();
		}

		ENSURES_DEBUG( m_shared_state->host_memory_page->HasEnoughMemory( sizeof( TProduct ) ) );
	}
}
}
}
}
