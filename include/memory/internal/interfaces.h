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


		// Whether the page has enough memory for placing given size.
		virtual const bool HasEnoughMemory( const size_t size ) const = 0;
	};

	// Basic collection of abstract memory pages.
	class BasicChunkedMemoryCollection : private Black::NonTransferable
	{
	// Construction and destruction.
	public:
		virtual ~BasicChunkedMemoryCollection() = default;

	// Public interface.
	public:
		// Grab the memory page that can allocate at last one memory chunk.
		virtual std::shared_ptr<BasicChunkedMemoryPage> RetainMemoryPage() = 0;

		// Return the memory page back to collection.
		virtual void ReleaseMemoryPage( const std::shared_ptr<BasicChunkedMemoryPage>& used_page ) = 0;


		// Whether the collection store no used pages.
		virtual const bool IsEmpty() const = 0;
	};
}
}
}
}
