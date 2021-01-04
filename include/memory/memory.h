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


#include "internal/RawAlignedMemoryPage.h"
#include "internal/RawMemoryPageCollection.h"
#include "internal/SlicedAlignedMemoryPage.h"
#include "internal/MemoryPageCollection.h"
#include "internal/MemoryPageDirector.h"

#include "WeakObjectAllocator.h"
#include "StrongObjectAllocator.h"
#include "internal/WeakSharedAllocator.h"
#include "internal/StrongSharedAllocator.h"
#include "internal/RawMemoryObjectAllocator.h"
#include "internal/RawMemorySharedAllocator.h"

#include "ObjectPool.h"
#include "MemoryFrame.h"
#include "MemoryPool.h"


#include "internal/RawAlignedMemoryPage.inl"
#include "internal/RawMemoryPageCollection.inl"
#include "internal/SlicedAlignedMemoryPage.inl"
#include "internal/MemoryPageCollection.inl"
#include "internal/MemoryPageDirector.inl"
#include "MemoryFrame.inl"
