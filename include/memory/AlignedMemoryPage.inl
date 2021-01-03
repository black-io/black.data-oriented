#pragma once


namespace Black
{
inline namespace DataOriented
{
inline namespace Memory
{
namespace Internal
{
	template< size_t RAW_MEMORY_SIZE, size_t MEMORY_ALIGNMENT >
	AlignedMemoryPage<RAW_MEMORY_SIZE, MEMORY_ALIGNMENT>::AlignedMemoryPage()
	{
		#if( BLACK_DEBUG_BUILD )
		Black::ZeroMemory( m_raw_memory );
		#endif
	}
}
}
}
}
