#pragma once


namespace Black
{
inline namespace DataOriented
{
// Black Compositions subsystem.
inline namespace Compositions
{
// Internal stuff for compositions.
namespace Internal
{

}
}
}
}


// Most core definitions.
#include "internal/internal.struct.h"

// Regular support for composition storage.
#include "internal/AllocationMask.h"
#include "internal/PartitionBlock.h"
#include "internal/PartitionMediator.h"

// Structured information about composition parts.
#include "internal/TypesCollectionOffsetBuilder.h"
#include "internal/PartitionMap.h"

// Implementation of composition mediator.
#include "HostRelatedComponent.h"
#include "internal/PartConstructionPolicy.h"
#include "internal/PartDestructorList.h"
#include "Composition.h"


// Deferred implementations.
#include "Composition.inl"
