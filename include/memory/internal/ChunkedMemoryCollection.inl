#pragma once


namespace Black
{
inline namespace DataOriented
{
inline namespace Memory
{
namespace Internal
{
	template< size_t ITEM_SIZE, size_t ITEMS_PER_PAGE, size_t ALIGNMENT, size_t MAX_FREE_PAGES >
	MemoryPageCollection<ITEM_SIZE, ITEMS_PER_PAGE, ALIGNMENT, MAX_FREE_PAGES>::MemoryPageCollection()
	{
		m_free_pages.reserve( MAX_FREE_PAGES );
	}

	template< size_t ITEM_SIZE, size_t ITEMS_PER_PAGE, size_t ALIGNMENT, size_t MAX_FREE_PAGES >
	MemoryPageCollection<ITEM_SIZE, ITEMS_PER_PAGE, ALIGNMENT, MAX_FREE_PAGES>::~MemoryPageCollection()
	{
		CRET( m_used_pages.empty() );
		BLACK_LOG_WARNING( "MemoryTraits", "{} page(s) still remains for objects of {}b size.", m_used_pages.size(), ITEM_SIZE );
	}

	template< size_t ITEM_SIZE, size_t ITEMS_PER_PAGE, size_t ALIGNMENT, size_t MAX_FREE_PAGES >
	inline void* MemoryPageCollection<ITEM_SIZE, ITEMS_PER_PAGE, ALIGNMENT, MAX_FREE_PAGES>::RetainObjectMemory()
	{
		auto& page = RetainMemoryPage();
		return page->RetainMemory();
	}

	template< size_t ITEM_SIZE, size_t ITEMS_PER_PAGE, size_t ALIGNMENT, size_t MAX_FREE_PAGES >
	inline void MemoryPageCollection<ITEM_SIZE, ITEMS_PER_PAGE, ALIGNMENT, MAX_FREE_PAGES>::ReleaseObjectMemory( Black::NotNull<void*> object_memory )
	{
		EXPECTS( !m_used_pages.empty() );
		if( m_used_pages.back()->IsItemValid( object_memory.Get() ) )
		{
			m_used_pages.back()->ReleaseMemory( object_memory.Get() );
			TryRefineUsedPage( --m_used_pages.end() );
			return;
		}

		EXPECTS( m_used_pages.size() > 1 );
		// Last memory page already checked and can not be used.
		auto search_end		= --m_used_pages.end();
		auto usable_page	= std::find_if(
			m_used_pages.begin(), search_end,
			[&object_memory]( const SharedStoringPage& page ) { return page->IsItemValid( object_memory.Get() ); }
		);

		EXPECTS( usable_page != search_end );
		(*usable_page)->ReleaseMemory( object_memory.Get() );
		TryRefineUsedPage( usable_page );
	}

	template< size_t ITEM_SIZE, size_t ITEMS_PER_PAGE, size_t ALIGNMENT, size_t MAX_FREE_PAGES >
	inline const typename MemoryPageCollection<ITEM_SIZE, ITEMS_PER_PAGE, ALIGNMENT, MAX_FREE_PAGES>::SharedStoringPage&
	MemoryPageCollection<ITEM_SIZE, ITEMS_PER_PAGE, ALIGNMENT, MAX_FREE_PAGES>::RetainMemoryPage()
	{
		CRET( m_used_pages.empty(), PullEmptyPage() );
		CRET( m_used_pages.back()->HasFreeItems(), m_used_pages.back() );
		CRET( m_used_pages.size() == 1, PullEmptyPage() );

		// Last memory page already checked and can not be used.
		auto search_end		= --m_used_pages.end();
		// Older pages are placed at beginning, it might have more free memory.
		auto usable_page	= std::find_if( m_used_pages.begin(), search_end, []( const SharedStoringPage& page ) { return page->HasFreeItems(); } );
		return ( usable_page == search_end )? PullEmptyPage() : *usable_page;
	}

	template< size_t ITEM_SIZE, size_t ITEMS_PER_PAGE, size_t ALIGNMENT, size_t MAX_FREE_PAGES >
	inline void MemoryPageCollection<ITEM_SIZE, ITEMS_PER_PAGE, ALIGNMENT, MAX_FREE_PAGES>::ReleaseMemoryPage( const SharedStoringPage& used_page )
	{
		CRET( !used_page->IsEmpty() );
		auto found_page = std::find_if(
			m_used_pages.begin(), m_used_pages.end(),
			[&used_page]( const SharedStoringPage& stored_page ) { return stored_page == used_page; }
		);

		EXPECTS( found_page != m_used_pages.end() );
		TryRefineUsedPage( found_page );
	}

	template< size_t ITEM_SIZE, size_t ITEMS_PER_PAGE, size_t ALIGNMENT, size_t MAX_FREE_PAGES >
	inline void MemoryPageCollection<ITEM_SIZE, ITEMS_PER_PAGE, ALIGNMENT, MAX_FREE_PAGES>::ReleaseMemoryPage( const std::shared_ptr<BasicMemoryPage>& used_page )
	{
		ReleaseMemoryPage( std::static_pointer_cast<StoringPage>( used_page ) );
	}

	template< size_t ITEM_SIZE, size_t ITEMS_PER_PAGE, size_t ALIGNMENT, size_t MAX_FREE_PAGES >
	inline const typename MemoryPageCollection<ITEM_SIZE, ITEMS_PER_PAGE, ALIGNMENT, MAX_FREE_PAGES>::SharedStoringPage&
	MemoryPageCollection<ITEM_SIZE, ITEMS_PER_PAGE, ALIGNMENT, MAX_FREE_PAGES>::AllocateNewPage()
	{
		m_used_pages.emplace_back( std::make_shared<StoringPage>() );
		return m_used_pages.back();
	}

	template< size_t ITEM_SIZE, size_t ITEMS_PER_PAGE, size_t ALIGNMENT, size_t MAX_FREE_PAGES >
	inline const typename MemoryPageCollection<ITEM_SIZE, ITEMS_PER_PAGE, ALIGNMENT, MAX_FREE_PAGES>::SharedStoringPage&
	MemoryPageCollection<ITEM_SIZE, ITEMS_PER_PAGE, ALIGNMENT, MAX_FREE_PAGES>::PullEmptyPage()
	{
		CRET( m_free_pages.empty(), AllocateNewPage() );
		m_used_pages.emplace_back( std::move( m_free_pages.back() ) );
		m_free_pages.pop_back();
		return m_used_pages.back();
	}

	template< size_t ITEM_SIZE, size_t ITEMS_PER_PAGE, size_t ALIGNMENT, size_t MAX_FREE_PAGES >
	inline void MemoryPageCollection<ITEM_SIZE, ITEMS_PER_PAGE, ALIGNMENT, MAX_FREE_PAGES>::TryRefineUsedPage( typename std::deque<SharedStoringPage>::iterator used_page )
	{
		CRET( !(*used_page)->IsEmpty() );
		SharedStoringPage free_page = *used_page;
		m_used_pages.erase( used_page );

		CRET( m_free_pages.size() == MAX_FREE_PAGES );
		m_free_pages.emplace_back( std::move( free_page ) );
	}
}
}
}
}
