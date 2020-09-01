#include "MultipleSwapChains.h"
#include "Color4.h"

[[nodiscard]] LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// Get pointer to the swap chain implementation
	Rhi::ISwapChain* swapChain = (NULL_HANDLE != hWnd) ? reinterpret_cast<Rhi::ISwapChain*>(GetWindowLongPtr(hWnd, GWLP_USERDATA)) : nullptr;

	// Evaluate message
	switch ( message )
	{
	case WM_SIZE:
		if ( nullptr != swapChain )
		{
			swapChain->resizeBuffers();
		}
		return 0;

	case WM_SYSKEYDOWN:
		// Toggle fullscreen right now? (Alt-Return)
		if ( nullptr != swapChain && VK_RETURN == wParam && (lParam & (1 << 29)) )	// Bit 29 = the ALT-key
		{
			// Toggle fullscreen mode
			swapChain->setFullscreenState(!swapChain->getFullscreenState());
		}
		return 0;

		// Let the OS handle this message
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}

bool MultipleSwapChains::init(int argc, const char * argv[])
{
	// Create the buffer manager
	mBufferManager = rhi->createBufferManager();

	{ // Create the root signature
		// Setup
		Rhi::RootSignatureBuilder rootSignatureBuilder;
		rootSignatureBuilder.initialize(0, nullptr, 0, nullptr, Rhi::RootSignatureFlags::ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

		// Create the instance
		mRootSignature = rhi->createRootSignature(rootSignatureBuilder);
	}

	// Vertex input layout
	static constexpr Rhi::VertexAttribute vertexAttributesLayout[] =
	{
		{ // Attribute 0
			// Data destination
			Rhi::VertexAttributeFormat::FLOAT_2,	// vertexAttributeFormat (Rhi::VertexAttributeFormat)
			"Position",								// name[32] (char)
			"POSITION",								// semanticName[32] (char)
			0,										// semanticIndex (uint32_t)
			// Data source
			0,										// inputSlot (uint32_t)
			0,										// alignedByteOffset (uint32_t)
			sizeof(float) * 2,						// strideInBytes (uint32_t)
			0										// instancesPerElement (uint32_t)
		}
	};
	const Rhi::VertexAttributes vertexAttributes(static_cast<uint32_t>(GLM_COUNTOF(vertexAttributesLayout)), vertexAttributesLayout);

	{ // Create vertex array object (VAO)
		// Create the vertex buffer object (VBO)
		// -> Clip space vertex positions, left/bottom is (-1,-1) and right/top is (1,1)
		static constexpr float VERTEX_POSITION[] =
		{					// Vertex ID	Triangle on screen
			 0.0f, 1.0f,	// 0				0
			 1.0f, 0.0f,	// 1			   .   .
			-0.5f, 0.0f		// 2			  2.......1
		};
		Rhi::IVertexBufferPtr vertexBuffer(mBufferManager->createVertexBuffer(sizeof(VERTEX_POSITION), VERTEX_POSITION));

		// Create vertex array object (VAO)
		// -> The vertex array object (VAO) keeps a reference to the used vertex buffer object (VBO)
		// -> This means that there's no need to keep an own vertex buffer object (VBO) reference
		// -> When the vertex array object (VAO) is destroyed, it automatically decreases the
		//    reference of the used vertex buffer objects (VBO). If the reference counter of a
		//    vertex buffer object (VBO) reaches zero, it's automatically destroyed.
		const Rhi::VertexArrayVertexBuffer vertexArrayVertexBuffers[] = { vertexBuffer };
		mVertexArray = mBufferManager->createVertexArray(vertexAttributes, static_cast<uint32_t>(GLM_COUNTOF(vertexArrayVertexBuffers)), vertexArrayVertexBuffers);
	}

	{
		// Create the graphics program
		Rhi::IGraphicsProgramPtr graphicsProgram;
		{
			// Get the shader source code (outsourced to keep an overview)
			const char* vertexShaderSourceCode = nullptr;
			const char* fragmentShaderSourceCode = nullptr;
#include "MultipleSwapChains_GLSL_450.h"	// For Vulkan
#include "MultipleSwapChains_GLSL_410.h"	// macOS 10.11 only supports OpenGL 4.1 hence it's our OpenGL minimum
#include "MultipleSwapChains_GLSL_ES3.h"
#include "MultipleSwapChains_HLSL_D3D11_D3D12.h"
#include "MultipleSwapChains_Null.h"

			// Create the graphics program
			Rhi::IShaderLanguage& shaderLanguage = rhi->getDefaultShaderLanguage();
			graphicsProgram = shaderLanguage.createGraphicsProgram(
				*mRootSignature,
				vertexAttributes,
				shaderLanguage.createVertexShaderFromSourceCode(vertexAttributes, vertexShaderSourceCode),
				shaderLanguage.createFragmentShaderFromSourceCode(fragmentShaderSourceCode));
		}

		// Create the graphics pipeline state object (PSO)
		if ( nullptr != graphicsProgram )
		{
			mGraphicsPipelineState = rhi->createGraphicsPipelineState(Rhi::GraphicsPipelineStateBuilder(mRootSignature, graphicsProgram, vertexAttributes, getMainRenderTarget()->getRenderPass()));
		}
	}

	{ // Create the swap chain
		handle nativeWindowHandle = NULL_HANDLE;

		// Create the OS native window instance
		// -> This is only a simple and close-to-the-metal example, don't use OS stuff directly in more complex projects
#ifdef _WIN32
		{ // Setup and register the window class for the this example window
			WNDCLASS windowDummyClass;
			windowDummyClass.hInstance = ::GetModuleHandle(nullptr);
			windowDummyClass.lpszClassName = TEXT("MultipleSwapChains");
			windowDummyClass.lpfnWndProc = WndProc;
			windowDummyClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
			windowDummyClass.hIcon = nullptr;
			windowDummyClass.hCursor = ::LoadCursor(nullptr, IDC_ARROW);
			windowDummyClass.lpszMenuName = nullptr;
			windowDummyClass.cbClsExtra = 0;
			windowDummyClass.cbWndExtra = 0;
			windowDummyClass.hbrBackground = nullptr;
			::RegisterClass(&windowDummyClass);
		}

		// Create the OS native window instance
		HWND hWnd = ::CreateWindowA("MultipleSwapChains", "Another window", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 640, 480, nullptr, nullptr, ::GetModuleHandle(nullptr), nullptr);
		if ( NULL_HANDLE != hWnd )
		{
			// Backup the native window handle
			nativeWindowHandle = reinterpret_cast<handle>(hWnd);

			// Show the created OS window
			::ShowWindow(hWnd, SW_SHOWDEFAULT);
		}
#elif defined LINUX
		gSwapChainWindow = new SwapChainWindow();
		gSwapChainWindow->setTitle("Another window");
		gSwapChainWindow->show();
		XSync(X11Application::instance()->getDisplay(), False);
		nativeWindowHandle = gSwapChainWindow->winId();
#else
#error "Unsupported platform"
#endif

// Create the swap chain
		mSwapChain = rhi->createSwapChain(getMainRenderTarget()->getRenderPass(), Rhi::WindowHandle{ nativeWindowHandle, nullptr });

		// This is only a simple and close-to-the-metal example, don't use OS stuff directly in more complex projects
#ifdef _WIN32
	// Is there a valid OS native window instance?
		if ( NULL_HANDLE != hWnd )
		{
			// In case of an error, destroy the OS native window instance at once
			if ( nullptr == mSwapChain )
			{
				// Destroy the native OS window instance
				::DestroyWindow(hWnd);

				// Unregister the window class for this example window
				::UnregisterClass(TEXT("MultipleSwapChains"), ::GetModuleHandle(nullptr));
			}
			else
			{
				// Set window pointer and handle (SetWindowLongPtr is the 64bit equivalent to SetWindowLong)
				::SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(mSwapChain->GetPointer()));
			}
		}
#elif defined LINUX
	// Is there a valid OS native window instance?
		if ( NULL_HANDLE != nativeWindowHandle )
		{
			// In case of an error, destroy the OS native window instance at once
			if ( nullptr == mSwapChain )
			{
				// Destroy the native OS window instance
				delete gSwapChainWindow;
				gSwapChainWindow = nullptr;
			}
		}
#endif
	}
	return true;
}

void MultipleSwapChains::update(double delta)
{
	// Usually you draw into a swap chain when getting informed by the OS that the
		// used native OS window requests a redraw of it's content. In order to avoid
		// adding to much unnecessary overhead in here we just draw into the created
		// swap chain as soon as the main swap chain gets redrawn.

		// Debug methods: When using Direct3D <11.1, these methods map to the Direct3D 9 PIX functions
		// (D3DPERF_* functions, also works directly within VisualStudio 2017 out-of-the-box)
		// -> In this example we're using multiple swap chains and calling "Rhi::ISwapChain::present()" twice per application
		// -> Usually, a swap chain present is interpreted by the debug/profile tool as a single frame, which is of course correct
		// -> In this example this behaviour makes it difficult to catch the desired frame of the desired native OS window

	{ // Draw into the main swap chain
		Rhi::IRenderTarget* mainRenderTarget = getMainRenderTarget();
		if ( nullptr != mainRenderTarget )
		{
			// Begin scene rendering
			if ( rhi->beginScene() )
			{
				{ // Fill the command buffer
					// Scoped debug event
					COMMAND_SCOPED_DEBUG_EVENT(mCommandBuffer, "Draw into the main swap chain")

						// Set the graphics render target to render into
						Rhi::Command::SetGraphicsRenderTarget::create(mCommandBuffer, mainRenderTarget);

					{ // Set the graphics viewport
						// Get the render target with and height
						uint32_t width = 1;
						uint32_t height = 1;
						mainRenderTarget->getWidthAndHeight(width, height);

						// Set the graphics viewport and scissor rectangle
						Rhi::Command::SetGraphicsViewportAndScissorRectangle::create(mCommandBuffer, 0, 0, width, height);
					}

					// Draw into the main swap chain
					fillCommandBuffer(Color4::GRAY, mCommandBuffer);
				}

				// Submit command buffer to the RHI implementation
				mCommandBuffer.submitToRhiAndClear(*rhi);

				// End scene rendering
				rhi->endScene();

				// Present the content of the current back buffer
				if ( mainRenderTarget->getResourceType() == Rhi::ResourceType::SWAP_CHAIN )
				{
					static_cast<Rhi::ISwapChain*>(mainRenderTarget)->present();
				}
			}
		}
	}

	// Render to the swap chain created in this example, but only if it's valid: Begin scene rendering
	if ( nullptr != mSwapChain && rhi->beginScene() )
	{
		{ // Fill the command buffer
			// Scoped debug event
			COMMAND_SCOPED_DEBUG_EVENT(mCommandBuffer, "Render to the swap chain created in this example")

				// Set the graphics render target to render into
				Rhi::Command::SetGraphicsRenderTarget::create(mCommandBuffer, mSwapChain);

			{ // Set the graphics viewport
				// Please note that for some graphics APIs its really important that the viewport
				// is inside the bounds of the currently used render target
				// -> For Direct3D 10 and Direct3D 11 TODO(co)(Check OpenGL and OpenGL ES 3 behaviour) it's OK
				//    when using a viewport which is outside the bounds of the currently used render target.
				//    Within this example you can intentionally set no new viewport in order to see what
				//    happens when using a viewport other than one covering the whole native OS window.
				// -> When using Direct3D 9 you will get a
				//      "Direct3D9: (ERROR) :Viewport outside the render target surface"
				//      "D3D9 Helper: IDirect3DDevice9::DrawPrimitive failed: D3DERR_INVALIDCAL"
				//    in case the viewport is inside the bounds of the currently used render target

				// Get the render target with and height
				uint32_t width = 1;
				uint32_t height = 1;
				mSwapChain->getWidthAndHeight(width, height);

				// Set the graphics viewport and scissor rectangle
				Rhi::Command::SetGraphicsViewportAndScissorRectangle::create(mCommandBuffer, 0, 0, width, height);
			}

			// Draw into the swap chain created in this example
			fillCommandBuffer(Color4::GREEN, mCommandBuffer);
		}

		// Submit command buffer to the RHI implementation
		mCommandBuffer.submitToRhiAndClear(*rhi);

		// End scene rendering
		rhi->endScene();

		// Present the content of the current back buffer
		mSwapChain->present();
	}
}

void MultipleSwapChains::shutdown()
{
	// Release the used resources
	if ( nullptr != mSwapChain )
	{
		// This is only a simple and close-to-the-metal example, don't use OS stuff directly in more complex projects
#ifdef _WIN32
	// Destroy the native OS window instance
		const HWND hWnd = reinterpret_cast<HWND>(mSwapChain->getNativeWindowHandle());
		if ( NULL_HANDLE != hWnd )
		{
			::DestroyWindow(hWnd);
		}

		// Unregister the window class for this example window
		::UnregisterClass(TEXT("MultipleSwapChains"), ::GetModuleHandle(nullptr));
#elif defined LINUX
		if ( nullptr != gSwapChainWindow )
		{
			delete gSwapChainWindow;
			gSwapChainWindow = nullptr;
		}
#endif

		// Release the swap chain
		mSwapChain = nullptr;
	}
	mVertexArray = nullptr;
	mGraphicsPipelineState = nullptr;
	mRootSignature = nullptr;
	mCommandBuffer.clear();
	mBufferManager = nullptr;
}

ApplicationSetting MultipleSwapChains::intial_app_settings()
{
	ApplicationSetting settings;
	settings.width = 1280;
	settings.height = 720;
	settings.title = "MultipleSwapChains";
	settings.rhiApi = RHIApi::Direct3D11;
	return settings;
}

void MultipleSwapChains::window_resized(int width, int height)
{
}

void MultipleSwapChains::fillCommandBuffer(const float color[4], Rhi::CommandBuffer& commandBuffer) const
{
	SE_ASSERT(nullptr != mRootSignature, "Invalid root signature");
	SE_ASSERT(nullptr != mGraphicsPipelineState, "Invalid graphics pipeline state");
	SE_ASSERT(nullptr != mVertexArray, "Invalid vertex array");

	// Scoped debug event
	COMMAND_SCOPED_DEBUG_EVENT_FUNCTION(commandBuffer);

	// Clear the graphics color buffer of the current render target with the provided color, do also clear the depth buffer
	Rhi::Command::ClearGraphics::create(commandBuffer, Rhi::ClearFlag::COLOR_DEPTH, color);

	// Set the used graphics root signature
	Rhi::Command::SetGraphicsRootSignature::create(commandBuffer, mRootSignature);

	// Set the used graphics pipeline state object (PSO)
	Rhi::Command::SetGraphicsPipelineState::create(commandBuffer, mGraphicsPipelineState);

	// Input assembly (IA): Set the used vertex array
	Rhi::Command::SetGraphicsVertexArray::create(commandBuffer, mVertexArray);

	// Render the specified geometric primitive, based on an array of vertices
	Rhi::Command::DrawGraphics::create(commandBuffer, 3);
}