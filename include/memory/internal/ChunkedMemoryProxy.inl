#pragma once


namespace Black
{
inline namespace DataOriented
{
inline namespace Memory
{
namespace Internal
{
	template< size_t CHUNKS_PER_PAGE, size_t MAX_FREE_PAGES, size_t MEMORY_ALIGNMENT >
	template< typename TObject >
	inline const std::shared_ptr<BasicChunkedMemoryCollection>& ChunkedMemoryProxy<CHUNKS_PER_PAGE, MAX_FREE_PAGES, MEMORY_ALIGNMENT>::GetMemoryCollection()
	{
		if( !m_collection )
		{
			ENSURES_DEBUG( m_chunk_size == 0 );

			auto collection	= std::make_shared<MemoryCollection<TObject>>();
			m_collection	= std::static_pointer_cast<BasicChunkedMemoryCollection>( collection );
			m_chunk_size	= sizeof( TObject );
		}

		ENSURES( m_chunk_size == sizeof( TObject ) );
		return m_collection;
	}

	template< size_t CHUNKS_PER_PAGE, size_t MAX_FREE_PAGES, size_t MEMORY_ALIGNMENT >
	inline const bool ChunkedMemoryProxy<CHUNKS_PER_PAGE, MAX_FREE_PAGES, MEMORY_ALIGNMENT>::IsEmpty() const
	{
		return !m_collection || m_collection->IsEmpty();
	}
}
}
}
}
