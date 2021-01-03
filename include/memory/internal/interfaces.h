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
	public:
		virtual ~BasicMemoryPageCollection() = default;
	};

	// Basic page of memory.
	class BasicMemoryPage : private Black::NonTransferable
	{
	public:
		virtual ~BasicMemoryPage() = default;

		/// @brief	Grab the memory from page.
		virtual void* RetainMemory() = 0;

		/// @brief	Return memory for single object back to page.
		virtual void ReleaseMemory( Black::NotNull<void*> item ) = 0;
	};
}
}
}
}
