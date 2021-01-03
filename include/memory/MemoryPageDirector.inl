#pragma once


namespace Black
{
inline namespace DataOriented
{
inline namespace Memory
{
namespace Internal
{
	template< size_t ITEMS_PER_PAGE, size_t ALIGNMENT, size_t MAX_FREE_PAGES >
	template< size_t ITEM_SIZE >
	inline std::shared_ptr<typename MemoryPageDirector<ITEMS_PER_PAGE, ALIGNMENT, MAX_FREE_PAGES>::template PageCollection<ITEM_SIZE>>
	MemoryPageDirector<ITEMS_PER_PAGE, ALIGNMENT, MAX_FREE_PAGES>::GetCollection()
	{
		CRET( m_used_item_size == 0, AcuireCollection<ITEM_SIZE>() );
		EXPECTS( m_used_item_size == ITEM_SIZE );
		return std::static_pointer_cast<PageCollection<ITEM_SIZE>>( *this );
	}

	template< size_t ITEMS_PER_PAGE, size_t ALIGNMENT, size_t MAX_FREE_PAGES >
	template< size_t ITEM_SIZE >
	inline std::shared_ptr<typename MemoryPageDirector<ITEMS_PER_PAGE, ALIGNMENT, MAX_FREE_PAGES>::template PageCollection<ITEM_SIZE>>
	MemoryPageDirector<ITEMS_PER_PAGE, ALIGNMENT, MAX_FREE_PAGES>::AcuireCollection()
	{
		m_used_item_size	= ITEM_SIZE;
		auto collection		= std::make_shared<PageCollection<ITEM_SIZE>>();
		std::shared_ptr<BasicMemoryPageCollection>::operator=( std::static_pointer_cast<BasicMemoryPageCollection>( collection ) );
		return collection;
	}
}
}
}
}
