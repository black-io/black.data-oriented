#pragma once


namespace Black
{
inline namespace DataOriented
{
inline namespace Memory
{
	/**
		@brief
	*/
	template< size_t RAW_MEMORY_SIZE, size_t MAX_FREE_PAGES = 1, size_t ALIGNMENT = 16>
	class MemoryFrame final : private Black::NonTransferable
	{
	public:
		MemoryFrame();
		~MemoryFrame();

		/// @brief		Reset and refine the frame.
		/// @warning	All previously allocated memory considered free.
		void Clear();

		/// @brief		Reset the state of memory frame.
		/// @warning	All previously allocated memory considered free.
		void Reset();

		/// @brief	Free all currently unused memory pages.
		void Refine();

		/// @brief		Returns the free memory, which is enough to store `TObjectType` type.
		/// @warning	No actual constructors will be invoked on returned memory.
		template< typename TObjectType >
		inline TObjectType* AllocateObject();

		/**
			@brief		Returns the free memory, which is enough to store `TObjectType[ objects_count ]` type.
			@warning	No actual constructors will be invoked on returned memory.
			@param	objects_count	Count of objects to place in requested memory.
		*/
		template< typename TObjectType >
		inline TObjectType* AllocateObject( size_t objects_count );

		/// @brief		Returns the memory, where the constructed `TObjectType` object is placed.
		/// @warning	Placement `new` operator is used to construct the object.
		template< typename TObjectType, typename... TArguments >
		inline TObjectType* ConstructObject( TArguments... arguments );

	private:
		using MemoryPage		= Internal::RawAlignedMemoryPage<RAW_MEMORY_SIZE, ALIGNMENT>;
		using SharedMemoryPage	= std::shared_ptr<MemoryPage>;

	private:
		inline MemoryPage& GetFreeMemoryPage( const size_t size );
		inline MemoryPage& AllocateFreePage();

	private:
		std::deque<SharedMemoryPage>	m_used_pages;
		std::vector<SharedMemoryPage>	m_free_pages;
	};
}
}
}
