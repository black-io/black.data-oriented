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

#include "internal/RawMemoryPage.h"
#include "internal/ChunkedMemoryPage.h"

#include "internal/RawMemoryCollection.h"
#include "internal/ChunkedMemoryCollection.h"

// Public implementations.
#include "MemoryFrame.h"


// Deferred stuff.
#include "MemoryFrame.inl"

#include "internal/ChunkedMemoryCollection.inl"
#include "internal/RawMemoryCollection.inl"

#include "internal/ChunkedMemoryPage.inl"
#include "internal/RawMemoryPage.inl"
