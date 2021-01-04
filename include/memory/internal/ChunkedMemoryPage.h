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
		@brief	Memory page may be used to store items of arbitrary size in one solid memory block.
		Memory page sores only memory for object, but not objects itself. Memory page will not construct or destruct objects.
		Memory page can align size and pointer of object.
		@warning	Any address, grabbed from page, must be returned to page back before page will be destroyed.

		@tparam	ITEM_SIZE		Size of single item in page.
		@tparam	ITEMS_PER_PAGE	Size of page. (Count of items in page)
		@tparam	ITEM_ALIGNMENT	Address alignment of pointer to item. Only positive `PoT` values, beginning from `1`, allowed.
	*/
	template< size_t ITEM_SIZE, size_t ITEMS_PER_PAGE, size_t ITEM_ALIGNMENT >
	class SlicedAlignedMemoryPage final : public BasicMemoryPage, public AlignedMemoryPage<Black::GetAlignedSize( ITEM_SIZE, ITEM_ALIGNMENT ) * ITEMS_PER_PAGE, ITEM_ALIGNMENT>
	{
	public:
		using Parent = AlignedMemoryPage<Black::GetAlignedSize( ITEM_SIZE, ITEM_ALIGNMENT ) * ITEMS_PER_PAGE, ITEM_ALIGNMENT>;

		SlicedAlignedMemoryPage();
		virtual ~SlicedAlignedMemoryPage();

		/// @see	BasicMemoryPage::RetainMemory
		virtual void* RetainMemory() override;

		/// @see	BasicMemoryPage::ReleaseMemory
		virtual void ReleaseMemory( Black::NotNull<void*> item ) override;

		/**
			@brief	Check if some pointer belongs to this page?

			@param	item	Pointer to be checked.
			@return			Return `true` if `item` belongs to this page.
		*/
		inline const bool IsItemValid( Black::NotNull<void*> item ) const;

		/// @brief	Get the count of memory pointers remaining in this page.
		inline const size_t GetRemainingItems() const	{ return m_remaining_items; };

		/// @brief	Check, is this page already empty (no pointer was grabbed from page)?
		inline const bool IsEmpty() const				{ return m_remaining_items == ITEMS_PER_PAGE; };

		/// @brief	Check, is this page already full (page holds no free pointer)?
		inline const bool IsFull() const				{ return m_remaining_items == 0; };

		/// @brief	Check, does this page holds some free pointers?
		inline const bool HasFreeItems() const			{ return m_remaining_items > 0; };

	private:
		size_t	m_remaining_items = ITEMS_PER_PAGE;	// Head of free items stack. Actually stores count of free items.
		void*	m_free_items[ ITEMS_PER_PAGE ];		// Stack of free items.
	};
}
}
}
}
