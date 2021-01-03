#pragma once


// Fundamental external dependencies.
#include <black/core/algorithms.h>


namespace Black
{
inline namespace DataOriented
{
// Memory management subsystem.
inline namespace Memory
{
// Internal stuff for memory management.
namespace Internal
{

}
}
}
}


// Fundamental internal stuff.
#include "internal/forwards.h"
#include "internal/interfaces.h"


// Public implementations.


// Deferred stuff.


#include "AlignedMemoryPage.h"
#include "RawAlignedMemoryPage.h"
#include "RawMemoryPageCollection.h"
#include "SlicedAlignedMemoryPage.h"
#include "MemoryPageCollection.h"
#include "MemoryPageDirector.h"

#include "WeakObjectAllocator.h"
#include "StrongObjectAllocator.h"
#include "WeakSharedAllocator.h"
#include "StrongSharedAllocator.h"
#include "RawMemoryObjectAllocator.h"
#include "RawMemorySharedAllocator.h"

#include "ObjectPool.h"
#include "MemoryFrame.h"
#include "MemoryPool.h"


#include "AlignedMemoryPage.inl"
#include "RawAlignedMemoryPage.inl"
#include "RawMemoryPageCollection.inl"
#include "SlicedAlignedMemoryPage.inl"
#include "MemoryPageCollection.inl"
#include "MemoryPageDirector.inl"
#include "MemoryFrame.inl"
