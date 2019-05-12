#pragma once


// Library initialization header.
#include "init.h"


namespace Black
{
inline namespace DataOriented
{
// `Composition over inheritance` subsystem.
inline namespace Composition
{
// Internal definitions for composition.
namespace Internal
{

}
}
}
}


// Most core definitions.
#include "composition/intrenal/internal.struct.h"

// Regular support for composition storage.
#include "composition/intrenal/AllocationMask.h"
#include "composition/intrenal/MemoryBlock.h"
#include "composition/intrenal/PartitionBlock.h"
#include "composition/intrenal/PartitionMediator.h"

// Structured information about composition parts.
#include "composition/intrenal/TypesCollectionOffsetBuilder.h"
#include "composition/intrenal/PartitionMap.h"

// Implementation of composition mediator.
#include "composition/CompositionPart.h"
#include "composition/intrenal/PartConstructionPolicy.h"
#include "composition/intrenal/PartDestructorList.h"
#include "composition/Composition.h"

// Deferred implementations.
#include "composition/Composition.inl"
