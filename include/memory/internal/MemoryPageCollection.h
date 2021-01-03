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
		@brief	Collection of memory pages.
		Collection of memory pages controls the life cycle of pages.
		Also collection proxies grabbing and releasing memory pointers from pages.

		@tparam	ITEM_SIZE		Size of single item in page.
		@tparam	ITEMS_PER_PAGE	Size of single memory page.
		@tparam	ALIGNMENT		Alignment of memory page. Only positive `PoT` values, beginning from `1`, allowed.
		@tparam	MAX_FREE_PAGES	Maximum count of free pages, that director can hold.
	*/
	template< size_t ITEM_SIZE, size_t ITEMS_PER_PAGE, size_t ALIGNMENT, size_t MAX_FREE_PAGES >
	class MemoryPageCollection final : public BasicMemoryPageCollection
	{
	public:
		/// @brief	Type of page this collection stores.
		using StoringPage		= SlicedAlignedMemoryPage<ITEM_SIZE, ITEMS_PER_PAGE, ALIGNMENT>;

		/// @brief	Type of shared page for this collection.
		using SharedStoringPage	= std::shared_ptr<StoringPage>;

		MemoryPageCollection();
		virtual ~MemoryPageCollection();

		/**
			@brief	Grab the pointer for single object from collection.
			@see	BasicMemoryPage::RetainMemory.

			@return		The value returned is an pointer to memory of `ITEM_SIZE` size, aligned to `ALIGNMENT` bytes. Return value may not be `nullptr`.
		*/
		inline void* RetainObjectMemory();

		/**
			@brief	Return memory for single object back to page.
			@see	BasicMemoryPage::ReleaseMemory.
		*/
		inline void ReleaseObjectMemory( Black::NotNull<void*> object_memory );

		/**
			@brief	Grab memory page from collection, that contain at last one free pointer.
			This function abstractly similar to `RetainObjectMemory`, but only promises that pointer will be grabbed from returned page.

			@return		The value returned is an memory page, that contain at last one free pointer.
		*/
		inline const SharedStoringPage& RetainMemoryPage();

		/**
			@brief	Return memory page back to collection.
			This function abstractly similar to `ReleaseObjectMemory`, but only tells that memory pointer already returned to page.

			@param	used_page	Page that was used before and now may be returned to page.
		*/
		inline void ReleaseMemoryPage( const SharedStoringPage& used_page );

		/// @brief	Proxy function that just cast `used_page` to `SharedStoringPage` type.
		inline void ReleaseMemoryPage( const std::shared_ptr<BasicMemoryPage>& used_page );

	private:
		/// @brief	Allocate new page and place it as topmost used one.
		inline const SharedStoringPage& AllocateNewPage();

		/// @brief	Retrieve empty page from `m_free_pages` and place it as topmost used one.
		inline const SharedStoringPage& PullEmptyPage();

		/// @brief	Remove empty page from `m_used_pages` and try to place it to `m_free_pages`.
		inline void TryRefineUsedPage( typename std::deque<SharedStoringPage>::iterator used_page );

	private:
		std::deque<SharedStoringPage>	m_used_pages;
		std::vector<SharedStoringPage>	m_free_pages;
	};
}
}
}
}
