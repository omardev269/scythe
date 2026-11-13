// BismIllahIRRahmaanIRRaheem
/* graphics pipeline vclass decl */

#ifndef SC_COREGFXPIPELINE_H
#define SC_COREGFXPIPELINE_H
#include <graphics/IGraphicsDevice.h>

namespace scythe {
	struct IGraphicsPipeline : IUpdateable1 /* update should push to the master RTV */ {
		virtual bool Create(IGraphicsDevice* aiRegistoire) = 0;
	};
	void CreateAlbedoGraphicsPipeline(IGraphicsPipeline*& oiPipeline, IGraphicsDevice* aiRegistoire);
	void CreatePhongGraphicsPipeline(IGraphicsPipeline*& oiPipeline, IGraphicsDevice* aiRegistoire);
}

#endif // !SC_COREGFXPIPELINE_H