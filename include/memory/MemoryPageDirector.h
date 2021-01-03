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
		@brief	Wrapper for collection of memory pages.
		`MemoryPageDirector` is an abstraction layer for object pool, since memory pages must store shared pointers instead of only objects.
		The allocator passed into `std::allocate_shared` will not be used directly, but only as proxy for allocator of shared pointer (shared allocator).
		And only such shared allocator will be used to request memory from collection of memory pages. So memory pages must store pointers with size of shared pointers.
		So, `MemoryPageDirector` splits the deducing the type of memory page collection into two stages.
		First stage is an declaration of `MemoryPageDirector`, where all options of memory collection specified.
		Second stage is an first request of concrete collection. At this moment the size of direct object become defined and type of collection may be deduced.

		@warning	After first request of actual collection, any other request of collection with different object size will be considered ill-formed.

		@tparam	ITEMS_PER_PAGE	Size of single memory page.
		@tparam	ALIGNMENT		Alignment of memory page.
		@tparam	MAX_FREE_PAGES	Maximum count of free pages, that director can hold.
	*/
	template< size_t ITEMS_PER_PAGE, size_t ALIGNMENT, size_t MAX_FREE_PAGES >
	class MemoryPageDirector : private std::shared_ptr<BasicMemoryPageCollection>, private Black::NonCopyable
	{
	public:
		/// @brief	Alias to this class for derived classes.
		using Parent = MemoryPageDirector<ITEMS_PER_PAGE, ALIGNMENT, MAX_FREE_PAGES>;

		/// @brief	Type of actual memory page collection.
		template< size_t ITEM_SIZE >
		using PageCollection = MemoryPageCollection<ITEM_SIZE, ITEMS_PER_PAGE, ALIGNMENT, MAX_FREE_PAGES>;

		/// @brief	Request of actual memory page collection.
		template< size_t ITEM_SIZE >
		inline std::shared_ptr<PageCollection<ITEM_SIZE>> GetCollection();

	private:
		/// @brief	Construction of actual memory page collection.
		template< size_t ITEM_SIZE >
		inline std::shared_ptr<PageCollection<ITEM_SIZE>> AcuireCollection();

	private:
		size_t m_used_item_size	= 0;	// Size of element the shared collection stores.
	};
}
}
}
}
