#pragma once


namespace Black
{
inline namespace DataOriented
{
inline namespace Memory
{
	template< typename TObject, size_t OBJECTS_PER_PAGE, size_t MAX_FREE_PAGES, size_t MEMORY_ALIGNMENT >
	UnmanagedObjectPool<TObject, OBJECTS_PER_PAGE, MAX_FREE_PAGES, MEMORY_ALIGNMENT>::~UnmanagedObjectPool()
	{
		EXPECTS( m_storage.IsEmpty() );
	}

	template< typename TObject, size_t OBJECTS_PER_PAGE, size_t MAX_FREE_PAGES, size_t MEMORY_ALIGNMENT >
	template< typename... TArguments >
	inline typename UnmanagedObjectPool<TObject, OBJECTS_PER_PAGE, MAX_FREE_PAGES, MEMORY_ALIGNMENT>::ProductType
	UnmanagedObjectPool<TObject, OBJECTS_PER_PAGE, MAX_FREE_PAGES, MEMORY_ALIGNMENT>::ConstructObject( TArguments&&... arguments )
	{
		return std::allocate_shared<TObject>( Allocator{ m_storage }, std::forward<TArguments>( arguments )... );
	}
}
}
}
