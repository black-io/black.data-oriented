#pragma once


// Library initialization header.
#include "core/init.h"


namespace Black
{
inline namespace DataOriented
{
// Composition subsystem.
inline namespace Composition
{
// Internal definitions for composition subsystem.
namespace Internal
{

}
}
}
}


// Most core definitions.
#include "composition/internal/internal.struct.h"

// Regular support for composition storage.
#include "composition/internal/AllocationMask.h"
#include "composition/internal/PartitionBlock.h"
#include "composition/internal/PartitionMediator.h"

// Structured information about composition parts.
#include "composition/internal/TypesCollectionOffsetBuilder.h"
#include "composition/internal/PartitionMap.h"

// Implementation of composition mediator.
#include "composition/CompositionPart.h"
#include "composition/internal/PartConstructionPolicy.h"
#include "composition/internal/PartDestructorList.h"
#include "composition/Composition.h"

// Deferred implementations.
#include "composition/Composition.inl"
