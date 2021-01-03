#pragma once


namespace Black
{
inline namespace DataOriented
{
inline namespace Memory
{
namespace Internal
{
	// Produces memory pointers with weak link with host memory page.
	template< typename TStoredObject, typename TMemoryPageDirector >
	class WeakSharedAllocator;

	// Produces memory pointers with strong link to host memory page.
	template< typename TStoredObject, typename TMemoryPageDirector >
	class StrongSharedAllocator;

	// Produces memory pointers with strong link to host memory page.
	template< typename TStoredObject, typename TRawMemoryPageCollection >
	class RawMemorySharedAllocator;
}
}
}
}
