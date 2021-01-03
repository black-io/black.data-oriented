#pragma once


namespace Black
{
inline namespace DataOriented
{
inline namespace Memory
{
	/**
		@brief	Object allocator with weak bond to memory page director.
		Object of this type may be used as allocator for `std::allocate_shared`.

		@note		Produced shared object will be valid only while object pool is alive. In other way SIGSEGV/AV exception will be generated.
		@warning	So that is a weak bond with memory page director. All produced objects must be destroyed before destruction of object pool.

		@tparam	TStoredObject		Type of object the `std::allocate_shared` must produce.
		@tparam	TMemoryPageDirector	Type of memory page director, that will be used to store shared pointer of produced object.
	*/
	template< typename TStoredObject, typename TMemoryPageDirector >
	class WeakObjectAllocator
	{
	public:
		/// @see `allocator::value_type`
		using value_type = TStoredObject;

		/// @see `allocator::rebind`
		template< typename TRebindObject >
		struct rebind
		{
			/// @brief	Whether the rebind is falsely provoked by STL.
			static constexpr bool IS_SAME = std::is_same_v<TStoredObject, TRebindObject>;

			/// @see	`allocator::rebind::other`
			using other = std::conditional_t<IS_SAME, WeakObjectAllocator, Internal::WeakSharedAllocator<TRebindObject, TMemoryPageDirector>>;
		};

		WeakObjectAllocator() = delete;
		WeakObjectAllocator( TMemoryPageDirector& director )
			: m_director( director )
		{

		}

		/// @brief	Get the stored memory page director.
		inline TMemoryPageDirector& GetMemoryDirector() const	{ return m_director; };

	private:
		TMemoryPageDirector& m_director; // Stored memory page director.
	};
}
}
}
