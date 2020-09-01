#include "Triangle.h"
#include "IndirectBuffer.h"
#include "Queries.h"
#include "VertexBuffer.h"
#include "Texture.h"
#include "CubeTexture.h"
#include "RenderToTexture.h"
#include "MultipleRenderTargets.h"
#include "MultipleSwapChains.h"
#include "Instancing.h"
#include "GeometryShader.h"
#include "TessellationShader.h"
#include "ComputeShader.h"
#include "IcosahedronTessellation.h"
#include "InstancedCubes.h"
#include "Mesh.h"
#include "Scene.h"
//-----------------------------------------------------------------------------
//#pragma comment(lib, "3rdparty_assimp.lib")
#pragma comment(lib, "3rdparty_other.lib")
#pragma comment(lib, "MikoEngine.lib")
#pragma comment(lib, "TGD.lib")
//-----------------------------------------------------------------------------
int main(
	[[maybe_unused]] int argc,
	[[maybe_unused]] const char *argv[])
{
	try
	{
		InstancedCubes app;
		return app.Run(argc, argv);
	}
	catch ( const std::exception &e )
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
//-----------------------------------------------------------------------------