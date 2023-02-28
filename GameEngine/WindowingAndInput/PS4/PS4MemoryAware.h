#pragma once
#ifdef _ORBIS
#include <gnm.h>
#include <gnmx\context.h>

namespace sce{
	namespace Gnmx {
		namespace Toolkit {
			struct IAllocator;
		}
	}
}

namespace NCL {
	namespace GNM {
		class GNMMemoryAware {
		protected:
			GNMMemoryAware() {}
			~GNMMemoryAware() {}
		protected:
			static sce::Gnmx::Toolkit::IAllocator*	onionAllocator;
			static sce::Gnmx::Toolkit::IAllocator*	garlicAllocator;
			static sce::Gnm::OwnerHandle			ownerHandle;
		};
	}
}
#endif
