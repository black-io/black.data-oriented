#pragma once


namespace Black
{
inline namespace DataOriented
{
inline namespace Memory
{
	template< size_t RAW_MEMORY_SIZE, size_t MAX_FREE_PAGES, size_t MEMORY_ALIGNMENT >
	template< typename TObject >
	inline TObject* MemoryFrame<RAW_MEMORY_SIZE, MAX_FREE_PAGES, MEMORY_ALIGNMENT>::AllocateMemory( const size_t objects_count )
	{
		EXPECTS( objects_count > 0 );

		const size_t buffer_size = sizeof( TObject ) * objects_count;
		EXPECTS_DEBUG( buffer_size <= RAW_MEMORY_SIZE );

		if constexpr( alignof( TObject ) <= MEMORY_ALIGNMENT )
		{
			return reinterpret_cast<TObject*>( Parent::RetainMemoryPage( buffer_size )->Allocate( buffer_size ) );
		}
		else
		{
			// Wide-enough buffer length to change the alignment of memory.
			const size_t aligned_buffer_size = buffer_size + alignof( TObject );
			EXPECTS_DEBUG( aligned_buffer_size <= RAW_MEMORY_SIZE );

			// Allocated memory is aligned using `MEMORY_ALIGNMENT` and should be realigned to alignment of desired type.
			void* buffer_memory = Parent::RetainMemoryPage( aligned_buffer_size )->Allocate( aligned_buffer_size );
			return reinterpret_cast<TObject*>( Black::GetAlignedPointer( buffer_memory, alignof( TObject ) ) );
		}
	}

	template< size_t RAW_MEMORY_SIZE, size_t MAX_FREE_PAGES, size_t MEMORY_ALIGNMENT >
	template< typename TObject, typename... TArguments >
	inline TObject* MemoryFrame<RAW_MEMORY_SIZE, MAX_FREE_PAGES, MEMORY_ALIGNMENT>::ConstructObject( TArguments&&... arguments )
	{
		auto object_memory = AllocateMemory<TObject>();
		return new( object_memory ) TObject{ std::forward<TArguments>( arguments )... };
	}

	template< size_t RAW_MEMORY_SIZE, size_t MAX_FREE_PAGES, size_t MEMORY_ALIGNMENT >
	inline void MemoryFrame<RAW_MEMORY_SIZE, MAX_FREE_PAGES, MEMORY_ALIGNMENT>::ReleseAllocatedMemory()
	{
		Parent::Release();
	}

	template< size_t RAW_MEMORY_SIZE, size_t MAX_FREE_PAGES, size_t MEMORY_ALIGNMENT >
	inline void MemoryFrame<RAW_MEMORY_SIZE, MAX_FREE_PAGES, MEMORY_ALIGNMENT>::RefineUnusedMemory()
	{
		Parent::Refine();
	}

	template< size_t RAW_MEMORY_SIZE, size_t MAX_FREE_PAGES, size_t MEMORY_ALIGNMENT >
	inline void MemoryFrame<RAW_MEMORY_SIZE, MAX_FREE_PAGES, MEMORY_ALIGNMENT>::DisposeMemory()
	{
		Parent::Reset();
	}
}
}
}
