#pragma once


namespace Black
{
inline namespace DataOriented
{
inline namespace Memory
{
	/**
		@brief	Object pool implementation.

		The object pool implements the special allocator to reduce the time of concrete object construction.
		It is much useful to produce the objects very intensively with lower allocation costs.

		@warning	This memory pool does not tracks the allocated objects on destruction.
		If pool is destroyed while there are some alive objects previously allocated by it, the host memory pages of this objects will be decoupled from pool.
		It means that the objects will remains alive after the pool is destroyed, but entire memory page of each object will not be released
		until the object destroyed. Even weak pointers should be cleared to release the memory of decoupled memory page.

		Such behavior is useful in rare situations, but it alway may be considered as memory leak.
		This type named as managed meaning that it manages the memory of created objects after destruction of pool.

		@tparam	TObject				Type of object to be stored and produced by pool
		@tparam	OBJECTS_PER_PAGE	Length of single memory page in objects.
		@tparam	MAX_FREE_PAGES		Number of empty pages, that will not be destroyed after it refined.
		@tparam	MEMORY_ALIGNMENT	Basic alignment of memory. Should satisfy the requirement of `TObject`.
	*/
	template< typename TObject, size_t OBJECTS_PER_PAGE, size_t MAX_FREE_PAGES = 1, size_t MEMORY_ALIGNMENT = alignof( void* ) >
	class ManagedObjectPool final
	{
	// Public inner types.
	public:
		// Type of stored objects.
		using StoredType	= TObject;

		// Type of allocation result.
		using ProductType	= std::shared_ptr<StoredType>;

	// Public interface.
	public:
		/**
			@brief	Construct the object in its unique memory.
			This function will use `std::allocate_shared` using the custom allocator. Once the returned shared pointer loss all references,
			the memory will be returned back to pool.

			@tparam		TObject		Type, which describes the requirement of allocated memory.
			@tparam		TArguments	Parameter pack of types for construction arguments.
			@param		arguments	Variable list of construction arguments for object.
			@return					The value returned is shared pointer to constructed object.
		*/
		template< typename... TArguments >
		inline ProductType ConstructObject( TArguments&&... arguments );

	// Private inner types.
	private:
		// Type of used memory collection.
		using MemoryStorage = Internal::ChunkedMemoryProxy<OBJECTS_PER_PAGE, MAX_FREE_PAGES, MEMORY_ALIGNMENT>;

		// Standard allocator to be used.
		using Allocator = Internal::ChunkedMemoryAllocator<TObject, MemoryStorage>;

	// Private state.
	private:
		MemoryStorage m_storage; // The storage of memory.
	};
}
}
}
