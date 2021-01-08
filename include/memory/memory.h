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
#include "internal/interfaces.h"

#include "internal/RawMemoryPage.h"
#include "internal/ChunkedMemoryPage.h"

#include "internal/RawMemoryCollection.h"
#include "internal/ChunkedMemoryCollection.h"
#include "internal/ChunkedMemoryProxy.h"

#include "internal/RawMemoryAllocator.h"
#include "internal/ChunkedMemoryAllocator.h"


// Public implementations.
#include "MemoryFrame.h"
#include "MemoryPool.h"
#include "UnmanagedObjectPool.h"
#include "ManagedObjectPool.h"


// Deferred stuff.
#include "ManagedObjectPool.inl"
#include "UnmanagedObjectPool.inl"
#include "MemoryPool.inl"
#include "MemoryFrame.inl"

#include "internal/ChunkedMemoryAllocator.inl"
#include "internal/RawMemoryAllocator.inl"

#include "internal/ChunkedMemoryProxy.inl"
#include "internal/ChunkedMemoryCollection.inl"
#include "internal/RawMemoryCollection.inl"

#include "internal/ChunkedMemoryPage.inl"
#include "internal/RawMemoryPage.inl"
