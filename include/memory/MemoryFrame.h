#pragma once


namespace Black
{
inline namespace DataOriented
{
inline namespace Memory
{
	/**
		@brief	Memory frame implementation.

		The memory frame implements the arena allocator, which should be refined to reuse the free memory.
		It may be useful to quickly allocate the number of trivial objects. After the memory no more needed, the frame may be just refined
		without necessary to manually destruct the allocated trivial object.

		@warning	The memory frame may be used only to allocate the objects of trivial types.

		@tparam	RAW_MEMORY_SIZE		Desired size of page memory.
		@tparam	MAX_FREE_PAGES		Number of empty pages, that will not be destroyed after it refined.
		@tparam	MEMORY_ALIGNMENT	Basic alignment of memory.
	*/
	template< size_t RAW_MEMORY_SIZE, size_t MAX_FREE_PAGES = 1, size_t MEMORY_ALIGNMENT = 16 >
	class MemoryFrame final : private Black::NonTransferable
	{
	// Public interface.
	public:
		/**
			@brief		Allocate the unique memory which fits the requirements to store the object of given type.
			This function is useful to get the memory for further call of placement new.
			@warning	No actual constructors will be invoked on returned memory.
			@tparam		TObject			Type, which describes the requirement of allocated memory.
			@param		objects_count	Desired count of sequentially placed object. By default it is 1, which means the memory for single object.
			@return		The value returned is only memory storage for object of given type, but not the constructed object.
		*/
		template< typename TObject >
		inline TObject* AllocateMemory( const size_t objects_count = 1 );

		/**
			@brief	Construct the object in its unique memory.
			Placement `new` operator is used to construct the object, so no `delete` operator needed on destruction, but only call of destructor.
			@tparam	TArguments	Parameter pack of types for construction arguments.
			@param	arguments	Variable list of construction arguments for object.
			@return				The value returned is an pointer to memory where the constructed object already stored.
		*/
		template< typename TObject, typename... TArguments >
		inline TObject* ConstructObject( TArguments&&... arguments );


		/**
			@brief		Release the currently used memory.
			All currently used memory will be refined and moved to pool of free pages.
			@warning	All previously allocated memory considered free after the call.
		*/
		inline void ReleseAllocatedMemory();

		/**
			@brief	Dispose only currently unused memory.
			All currently allocated memory will remains allocated. Only the pool of cached free memory will be cleared.
		*/
		inline void RefineUnusedMemory();

		/**
			@brief		Dispose all of currently allocated memory.
			After this call the memory frame will store no memory at all.
			@warning	All previously allocated memory considered free after the call.
		*/
		inline void DisposeMemory();

	// Private state.
	private:
		Internal::RawMemoryCollection<RAW_MEMORY_SIZE, MEMORY_ALIGNMENT, MAX_FREE_PAGES> m_storage; // The storage of raw memory.
	};
}
}
}
