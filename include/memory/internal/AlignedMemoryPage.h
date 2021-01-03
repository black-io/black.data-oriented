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
	*/
	template< size_t RAW_MEMORY_SIZE, size_t MEMORY_ALIGNMENT >
	class AlignedMemoryPage
	{
		static_assert( RAW_MEMORY_SIZE > 0, "The size of raw memory may not be less than 1b." );
		static_assert( MEMORY_ALIGNMENT > 0, "Memory alignment may not be less of `1`." );
	public:
		AlignedMemoryPage();

	protected:
		inline uint8_t* GetMemoryBegin() const	{ return m_memory_begin; };
		inline uint8_t* GetMemoryEnd() const	{ return m_memory_end; };

	protected:
		static constexpr size_t ALIGNED_MEMORY_SIZE = Black::GetAlignedSize( RAW_MEMORY_SIZE, MEMORY_ALIGNMENT ) + MEMORY_ALIGNMENT;

	private:
		uint8_t		m_raw_memory[ ALIGNED_MEMORY_SIZE ];
		uint8_t*	m_memory_begin	= Black::GetAlignedPointer( m_raw_memory, MEMORY_ALIGNMENT );
		uint8_t*	m_memory_end	= m_memory_begin + Black::GetAlignedSize( RAW_MEMORY_SIZE, MEMORY_ALIGNMENT );
	};
}
}
}
}
