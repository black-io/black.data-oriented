#pragma once


namespace Black
{
inline namespace DataOriented
{
inline namespace Memory
{
	/**
		@brief	Memory pool implementation.

		The memory pool implements the arena allocator with additional tracking of memory.
		This pool may be used to host the memory for object of any type, whatever trivial or invariant one.

		@warning	Before the pool may be destroyed, all the allocated objects should be already destroyed. It means even all `std::weak_ptr` should be released.

		@tparam	RAW_MEMORY_SIZE		Desired size of page memory.
		@tparam	MAX_FREE_PAGES		Number of empty pages, that will not be destroyed after it refined.
		@tparam	MEMORY_ALIGNMENT	Basic alignment of memory.
	*/
	template< size_t RAW_MEMORY_SIZE, size_t MAX_FREE_PAGES = 1, size_t MEMORY_ALIGNMENT = 16 >
	class MemoryPool final : private Black::NonTransferable
	{
	// Construction and destruction.
	public:
		~MemoryPool();

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
		inline std::shared_ptr<Tobject> ConstructObject( TArguments&&... arguments );

	// Private state.
	private:
		Internal::RawMemoryCollection<RAW_MEMORY_SIZE, MEMORY_ALIGNMENT, MAX_FREE_PAGES> m_storage; // The storage of raw memory.
	};
}
}
}
