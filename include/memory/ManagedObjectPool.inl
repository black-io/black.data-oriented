#pragma once


namespace Black
{
inline namespace DataOriented
{
inline namespace Memory
{
	template< typename TObject, size_t OBJECTS_PER_PAGE, template< typename, typename > class TAllocator, size_t MAX_FREE_PAGES, size_t MEMORY_ALIGNMENT >
	ObjectPool<TObject, OBJECTS_PER_PAGE, TAllocator, MAX_FREE_PAGES, MEMORY_ALIGNMENT>::~ObjectPool()
	{

	}

	template< typename TObject, size_t OBJECTS_PER_PAGE, template< typename, typename > class TAllocator, size_t MAX_FREE_PAGES, size_t MEMORY_ALIGNMENT >
	template< typename Tobject, typename... TArguments >
	inline typename ObjectPool<TObject, OBJECTS_PER_PAGE, TAllocator, MAX_FREE_PAGES, MEMORY_ALIGNMENT>::ProductType
	ObjectPool<TObject, OBJECTS_PER_PAGE, TAllocator, MAX_FREE_PAGES, MEMORY_ALIGNMENT>::ConstructObject( TArguments&&... arguments )
	{

	}
}
}
}
