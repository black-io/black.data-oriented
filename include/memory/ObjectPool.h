#pragma once


namespace Black
{
inline namespace DataOriented
{
inline namespace Memory
{
	/// @brief	Object pool.
	template< typename TStoredObject, size_t ITEMS_PER_PAGE, template< typename, typename > class TAllocator, size_t MAX_FREE_PAGES = 1, size_t ALIGNMENT = 16 >
	class ObjectPool final : private Internal::MemoryPageDirector<ITEMS_PER_PAGE, ALIGNMENT, MAX_FREE_PAGES>
	{
	public:
		using StoredObject	= TStoredObject;
		using SharedObject	= std::shared_ptr<StoredObject>;
		using Allocator		= TAllocator< StoredObject, Internal::MemoryPageDirector<ITEMS_PER_PAGE, ALIGNMENT, MAX_FREE_PAGES> >;

		template< typename... TConstructionArguments >
		inline SharedObject AllocateObject( TConstructionArguments&&... construction_arguments )
		{
			return std::allocate_shared<TStoredObject>( Allocator{ *this }, std::forward<TConstructionArguments>( construction_arguments )... );
		}
	};

	/// @brief	Object pool without linkage between produced objects and its host memory pages.
	template< typename TStoredObject, size_t ITEMS_PER_PAGE, size_t MAX_FREE_PAGES = 1, size_t ALIGNMENT = 16 >
	using WeakObjectPool	= ObjectPool<TStoredObject, ITEMS_PER_PAGE, WeakObjectAllocator, MAX_FREE_PAGES, ALIGNMENT>;

	/// @brief	Object pool with strong linkage between produced objects and its host memory pages.
	template< typename TStoredObject, size_t ITEMS_PER_PAGE, size_t MAX_FREE_PAGES = 1, size_t ALIGNMENT = 16 >
	using StrongObjectPool	= ObjectPool<TStoredObject, ITEMS_PER_PAGE, StrongObjectAllocator, MAX_FREE_PAGES, ALIGNMENT>;
}
}
}
