// BismIllahIRRahmaanIRRaheem
/* IUpdateable interface header declaration */

#ifndef IUPDATEABLE_H
#define IUPDATEABLE_H
#include <runtime/engineCore.h>
namespace scythe {
	struct IUpdateable {
		virtual void BeginFrame() = 0;
		virtual void EndFrame() = 0;
		virtual void Dismiss() = 0;
	};
	struct IUpdateable1 {
		virtual void Update() = 0;
		virtual void Dismiss() = 0;
	};
	struct IUpdateable2 {
		virtual void Update(largefloat aDelta) = 0;
		virtual void Dismiss() = 0;
	};
	struct IUpdateable3 {
		virtual void BeginFrame(largefloat aDelta) = 0;
		virtual void EndFrame(largefloat aDelta) = 0;
		virtual void Dismiss() = 0;
	};
}
#endif // !IUPDATEABLE_H