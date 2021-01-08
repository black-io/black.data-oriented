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

		@warning	Before the pool may be destroyed, all the allocated objects should be already destroyed. It means even all `std::weak_ptr` should be released.

		@tparam	TObject				Type of object to be stored and produced by pool
		@tparam	OBJECTS_PER_PAGE	Length of single memory page in objects.
		@tparam	TAllocator			Allocator template to be used for object construction.
		@tparam	MAX_FREE_PAGES		Number of empty pages, that will not be destroyed after it refined.
		@tparam	MEMORY_ALIGNMENT	Basic alignment of memory. Should satisfy the requirement of `TObject`.
	*/
	template< typename TObject, size_t OBJECTS_PER_PAGE, template< typename, typename > class TAllocator, size_t MAX_FREE_PAGES = 1, size_t MEMORY_ALIGNMENT = 16 >
	class ObjectPool final
	{
	// Public inner types.
	public:
		// Type of stored objects.
		using StoredType	= TObject;

		// Type of allocation result.
		using ProductType	= std::shared_ptr<StoredObject>;

	// Construction and destruction.
	public:
		~ObjectPool();

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
		template< typename Tobject, typename... TArguments >
		inline ProductType ConstructObject( TArguments&&... arguments );

	// Private inner types.
	private:
		//
		using ChunkedAllocator = Internal::ChunkedMemoryAllocator<OBJECTS_PER_PAGE, MAX_FREE_PAGES, std::max( alignof( TObject ), MEMORY_ALIGNMENT )>;

	// Private state.
	private:
		ChunkedAllocator m_collection_proxy; //
	};
}
}
}
