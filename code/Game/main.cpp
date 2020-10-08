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

//переписать вручную RHI
//
//https ://github.com/cofenberg/unrimp/commit/ba576e6d6fe076860da60c54cbb9e778a5edc896
//https://github.com/cofenberg/unrimp/commit/d67aa02653c3eabc0c86faef43f3b8d491f5c18c#diff-b43fe52b647de0930a49fcb2b277484c
//https://github.com/cofenberg/unrimp/commit/e6fbefbc7659b8ae452aea4d383c056744628f7d
//https://github.com/cofenberg/unrimp/commit/d37710ea8ed6fc8580394c8624cf6d14f118517b
//
//enduro2d
//raylib

включить в настройках проекта анализ кода


int main(
	[[maybe_unused]] int argc,
	[[maybe_unused]] const char *argv[])
{
	try
	{
		Triangle app;
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