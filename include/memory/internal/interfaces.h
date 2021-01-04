#pragma once


namespace Black
{
inline namespace DataOriented
{
inline namespace Memory
{
namespace Internal
{
	// Basic collection of abstract memory pages.
	class BasicMemoryPageCollection : private Black::NonTransferable
	{
	// Construction and destruction.
	public:
		virtual ~BasicMemoryPageCollection() = default;
	};

	// Basic page of memory.
	class BasicMemoryPage : private Black::NonTransferable
	{
	// Construction and destruction.
	public:
		virtual ~BasicMemoryPage() = default;

	// Public interface.
	public:
		// Grab the memory from page.
		virtual void* Allocate() = 0;

		// Return memory for single object back to page.
		virtual void Free( Black::NotNull<void*> memory ) = 0;
	};
}
}
}
}
