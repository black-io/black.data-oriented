#pragma once


namespace Black
{
inline namespace DataOriented
{
inline namespace Memory
{
namespace Internal
{
	template< size_t ITEM_SIZE, size_t ITEMS_PER_PAGE, size_t ITEM_ALIGNMENT >
	SlicedAlignedMemoryPage<ITEM_SIZE, ITEMS_PER_PAGE, ITEM_ALIGNMENT>::SlicedAlignedMemoryPage()
	{
		constexpr size_t ALIGNED_ITEM_SIZE = Black::GetAlignedSize( ITEM_SIZE, ITEM_ALIGNMENT );

		uint8_t* item_place = Parent::GetMemoryBegin();
		for( auto& item_slot : m_free_items )
		{
			EXPECTS_DEBUG( item_place < Parent::GetMemoryEnd() );
			EXPECTS_DEBUG( ( item_place + ALIGNED_ITEM_SIZE ) <= Parent::GetMemoryEnd() );
			item_slot	= item_place;
			item_place	+= ALIGNED_ITEM_SIZE;
		}
	}

	template< size_t ITEM_SIZE, size_t ITEMS_PER_PAGE, size_t ITEM_ALIGNMENT >
	SlicedAlignedMemoryPage<ITEM_SIZE, ITEMS_PER_PAGE, ITEM_ALIGNMENT>::~SlicedAlignedMemoryPage()
	{
		EXPECTS( IsEmpty() );
	}

	template< size_t ITEM_SIZE, size_t ITEMS_PER_PAGE, size_t ITEM_ALIGNMENT >
	void* SlicedAlignedMemoryPage<ITEM_SIZE, ITEMS_PER_PAGE, ITEM_ALIGNMENT>::RetainMemory()
	{
		EXPECTS( m_remaining_items > 0 );
		return m_free_items[ --m_remaining_items ];
	}

	template< size_t ITEM_SIZE, size_t ITEMS_PER_PAGE, size_t ITEM_ALIGNMENT >
	void SlicedAlignedMemoryPage<ITEM_SIZE, ITEMS_PER_PAGE, ITEM_ALIGNMENT>::ReleaseMemory( Black::NotNull<void*> item )
	{
		EXPECTS( m_remaining_items < ITEMS_PER_PAGE );
		EXPECTS( item.Get<uint8_t*>() >= Parent::GetMemoryBegin() );
		EXPECTS( item.Get<uint8_t*>() < Parent::GetMemoryEnd() );
		EXPECTS( ( (std::uintptr_t)item.Get() % ITEM_ALIGNMENT ) == 0 );
		// Just put item back and shift the head of stack.
		m_free_items[ m_remaining_items++ ] = item.Get();
	}

	template< size_t ITEM_SIZE, size_t ITEMS_PER_PAGE, size_t ITEM_ALIGNMENT >
	inline const bool SlicedAlignedMemoryPage<ITEM_SIZE, ITEMS_PER_PAGE, ITEM_ALIGNMENT>::IsItemValid( Black::NotNull<void*> item ) const
	{
		// This page almost empty.
		CRET( m_remaining_items == ITEMS_PER_PAGE, false );
		// `item` not in range of page.
		CRET( item.Get<uint8_t*>() < Parent::GetMemoryBegin(), false );
		// `item` not in range of page.
		CRET( item.Get<uint8_t*>() >= Parent::GetMemoryEnd(), false );

		// Expecting that `item` has right alignment.
		EXPECTS( ( (std::uintptr_t)item.Get() % ITEM_ALIGNMENT ) == 0 );
		return true;
	}
}
}
}
}
