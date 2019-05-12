#pragma once


// Library initialization header.
#include "init.h"


namespace Black
{
inline namespace DataOriented
{
// `CompositionOverInheritance over inheritance` subsystem.
inline namespace CompositionOverInheritance
{
// Internal definitions for composition.
namespace Internal
{

}
}
}
}


// Most core definitions.
#include "composition-over-inheritance/intrenal/internal.struct.h"

// Regular support for composition storage.
#include "composition-over-inheritance/intrenal/AllocationMask.h"
#include "composition-over-inheritance/intrenal/MemoryBlock.h"
#include "composition-over-inheritance/intrenal/PartitionBlock.h"
#include "composition-over-inheritance/intrenal/PartitionMediator.h"

// Structured information about composition parts.
#include "composition-over-inheritance/intrenal/TypesCollectionOffsetBuilder.h"
#include "composition-over-inheritance/intrenal/PartitionMap.h"

// Implementation of composition mediator.
#include "composition-over-inheritance/CompositionPart.h"
#include "composition-over-inheritance/intrenal/PartConstructionPolicy.h"
#include "composition-over-inheritance/intrenal/PartDestructorList.h"
#include "composition-over-inheritance/Composition.h"

// Deferred implementations.
#include "composition-over-inheritance/Composition.inl"
