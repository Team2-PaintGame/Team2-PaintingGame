#ifdef _ORBIS
#include "GNMMemoryAware.h"
#include <.\graphics\api_gnm\toolkit\allocators.h>
#include <.\graphics\api_gnm\toolkit\stack_allocator.h>
using namespace NCL::GNM;

sce::Gnmx::Toolkit::IAllocator*		GNMMemoryAware::onionAllocator;
sce::Gnmx::Toolkit::IAllocator*		GNMMemoryAware::garlicAllocator;
sce::Gnm::OwnerHandle				GNMMemoryAware::ownerHandle;
#endif