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
		@brief	Shared object allocator with weak bond to memory page director.
		Object of this type will be deduced by `std::allocate_shared` from `WeakObjectAllocator`.

		@tparam	TStoredObject		Type of shared object.
		@tparam	TMemoryPageDirector	Type of memory page director, that will be used to store shared pointer of produced object.
	*/
	template< typename TStoredObject, typename TMemoryPageDirector >
	class WeakSharedAllocator
	{
	public:
		/// @see `allocator::value_type`
		using value_type = TStoredObject;

		/// @brief	Type of parent allocator.
		template< typename TRebindObject >
		using OriginAllocator = Black::WeakObjectAllocator<TRebindObject, TMemoryPageDirector>;

		/// @see `allocator::rebind`
		template< typename TRebindObject >
		struct rebind
		{
			/// @see `allocator::rebind::other`
			using other = OriginAllocator<TRebindObject>;
		};

		WeakSharedAllocator() = delete;

		template< typename TRebindObject >
		WeakSharedAllocator( const OriginAllocator<TRebindObject>& origin_allocator )
			: m_director( origin_allocator.GetMemoryDirector() )
		{

		}

		/// @see `allocator::allocate`
		inline value_type* allocate( size_t objects_count )
		{
			constexpr size_t STORED_OBJECT_SIZE = sizeof( TStoredObject );

			EXPECTS( objects_count == 1 );
			return (value_type*)m_director.template GetCollection<STORED_OBJECT_SIZE>()->RetainObjectMemory();
		}

		/// @see `allocator::deallocate`
		inline void deallocate( value_type* object_ptr, size_t objects_count )
		{
			constexpr size_t STORED_OBJECT_SIZE = sizeof( TStoredObject );

			EXPECTS( objects_count == 1 );
			m_director.template GetCollection<STORED_OBJECT_SIZE>()->ReleaseObjectMemory( object_ptr );
		}

	private:
		TMemoryPageDirector& m_director; // Stored memory page director.
	};
}
}
}
}
