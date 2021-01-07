#pragma once


namespace Black
{
inline namespace DataOriented
{
inline namespace Memory
{
namespace Internal
{
	// Basic page of memory.
	class BasicChunkedMemoryPage : private Black::NonTransferable
	{
	// Construction and destruction.
	public:
		virtual ~BasicChunkedMemoryPage() = default;

	// Public interface.
	public:
		// Grab the memory from page.
		virtual void* Allocate() = 0;

		// Return memory for single object back to page.
		virtual void Free( Black::NotNull<void*> memory ) = 0;
	};

	// Basic collection of abstract memory pages.
	class BasicChunkedMemoryCollection : private Black::NonTransferable
	{
	// Construction and destruction.
	public:
		virtual ~BasicChunkedMemoryCollection() = default;

	// Public interface.
	public:
		// Get the size of chunk in collection.
		virtual const size_t GetChunkSize() const = 0;
	};
}
}
}
}
