#include "DirectXFramework.h"

// DirectX libraries that are needed
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

DirectXFramework * _dxFramework = nullptr;

DirectXFramework::DirectXFramework() : DirectXFramework(800, 600)
{
}

DirectXFramework::DirectXFramework(unsigned int width, unsigned int height) : Framework(width, height)
{
	_dxFramework = this;

	// Set default background colour
	_backgroundColour[0] = 0.0f;
	_backgroundColour[1] = 0.0f;
	_backgroundColour[2] = 0.0f;
	_backgroundColour[3] = 0.0f;

	// Camera initialisation
	_camera = Camera(
		Vector3(0.0f, 20.0f, -90.0f),	// Eye position
		Vector3(0.0f, 20.0f, 0.0f),		// Focal point position
		0.05f,		// Mouse sensitivity
		0.2f,		// Move speed
		0.4f,		// Move speed addition
		1.0f,		// Scroll speed
		2.0f,		// Scroll speed addition
		XM_PIDIV4,	// FOV
		300.0f		// Render distance
	);
}

void DirectXFramework::OnMouseClick(const int x, const int y, const bool isDown)
{
	_mouseDown = isDown;
	_mouseCoords = { x, y };
}

void DirectXFramework::OnMouseMoveRaw(const int x, const int y)
{
	if (_mouseDown)

	{
		_camera.AdjustEyeRotation(Vector3(
			(float)x * (_camera.GetMouseSensitivity() * _camera.GetMouseSensitivity()),
			(float)y * (_camera.GetMouseSensitivity() * _camera.GetMouseSensitivity()),
			0.0f
		));
		_mouseCoords = { x, y };
	}
}

void DirectXFramework::OnMouseWheel(const int x, const int y, const bool isDown)
{
	if (isDown)
	{
		_scrollCount -= 1;
	}
	else
	{
		_scrollCount += 1;
	}
}

void DirectXFramework::OnKey(const WPARAM wParam, const bool isDown)
{
	if (_keysDown.count((int)wParam))
	{
		if ((int)wParam == 16)
		{
			if (isDown && !(_isRunning))
			{
				_isRunning = true;
				_camera.AdjustMoveSpeed(_camera.GetMoveSpeedAdd());
				_camera.AdjustScrollSpeed(_camera.GetScrollSpeedAdd());
			}
			else if (!(isDown) && _isRunning)
			{
				_isRunning = false;
				_camera.AdjustMoveSpeed(-_camera.GetMoveSpeedAdd());
				_camera.AdjustScrollSpeed(-_camera.GetScrollSpeedAdd());
			}
		}
		else
		{
			_keysDown[(int)wParam] = isDown;
		}
	}
}

DirectXFramework * DirectXFramework::GetDXFramework()
{
	return _dxFramework;
}

const Matrix& DirectXFramework::GetViewTransformation() const
{
	return _viewTransformation;
}

const Matrix& DirectXFramework::GetProjectionTransformation() const
{
	return _projectionTransformation;
}

void DirectXFramework::SetBackgroundColour(Vector4 backgroundColour)
{
	_backgroundColour[0] = backgroundColour.x;
	_backgroundColour[1] = backgroundColour.y;
	_backgroundColour[2] = backgroundColour.z;
	_backgroundColour[3] = backgroundColour.w;
}

void DirectXFramework::CreateSceneGraph()
{
}

void DirectXFramework::UpdateSceneGraph()
{
}

bool DirectXFramework::Initialise()
{
	// The call to CoInitializeEx is needed if we are using
	// textures since the WIC library used requires it, so we
	// take care of initialising it here
	if FAILED(CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED))
	{
		return false;
	}
	if (!GetDeviceAndSwapChain())
	{
		return false;
	}
	OnResize(WM_EXITSIZEMOVE);

	// Create camera and projection matrices 
	_projectionTransformation = XMMatrixPerspectiveFovLH(_camera.GetFOV(), static_cast<float>(GetWindowWidth()) / GetWindowHeight(), 1.0f, _camera.GetRenderDistance());
	_sceneGraph = make_shared<SceneGraph>();
	CreateSceneGraph();
	return _sceneGraph->Initialise();
}

void DirectXFramework::Shutdown()
{
	// Required because we called CoInitialize above
	_sceneGraph->Shutdown();
	CoUninitialize();
}

void DirectXFramework::Update()
{
	// Camera
	for (const auto& key : _keysDown)
	{
		if (key.second)
		{
			switch (key.first)
			{
			case 87: // Forward
				_camera.AdjustEyePosition(_camera.GetFowardVector() * _camera.GetMoveSpeed());
				break;

			case 65: // Left
				_camera.AdjustEyePosition(_camera.GetLeftVector() * _camera.GetMoveSpeed());
				break;

			case 83: // Backward
				_camera.AdjustEyePosition(_camera.GetBackwardVector() * _camera.GetMoveSpeed());
				break;

			case 68: // Right
				_camera.AdjustEyePosition(_camera.GetRightVector() * _camera.GetMoveSpeed());
				break;

			case 32: // Up
				_camera.AdjustEyePosition(_camera.GetUpVector() * _camera.GetMoveSpeed());
				break;

			case 67: // Down
				_camera.AdjustEyePosition(-(_camera.GetUpVector() * _camera.GetMoveSpeed()));
				break;
			}
		}
	}

	if (_scrollCount > 0)
	{
		for (int i = 0; i < _scrollCount; i++)
		{
			_camera.AdjustEyePosition(_camera.GetFowardVector() * _camera.GetScrollSpeed());
		}
	}
	else if (_scrollCount < 0)
	{
		for (int i = 0; i > _scrollCount; i--)
		{
			_camera.AdjustEyePosition(_camera.GetBackwardVector() * _camera.GetScrollSpeed());
		}
	}

	_camera.SetRotationMatrix(Matrix::CreateFromYawPitchRoll(_camera.GetEyeRotationX(), _camera.GetEyeRotationY(), _camera.GetEyeRotationZ()));
	_camera.SetFocalPointPosition(XMVector3TransformCoord(_camera.GetDefaultFowardVector(), _camera.GetRotationMatrix()));
	_camera.AdjustFocalPointPosition(_camera.GetEyePosition());

	_camera.SetUpVector(XMVector3TransformCoord(_camera.GetDefaultUpVector(), _camera.GetRotationMatrix()));

	_camera.SetVectorRotationMatrix(Matrix::CreateFromYawPitchRoll(_camera.GetEyeRotationX(), _camera.GetEyeRotationY(), 0.0f));

	_camera.SetFowardVector(XMVector3TransformCoord(_camera.GetDefaultFowardVector(), _camera.GetVectorRotationMatrix()));
	_camera.SetBackwardVector(XMVector3TransformCoord(_camera.GetDefaultBackwardVector(), _camera.GetVectorRotationMatrix()));
	_camera.SetLeftVector(XMVector3TransformCoord(_camera.GetDefaultLeftVector(), _camera.GetVectorRotationMatrix()));
	_camera.SetRightVector(XMVector3TransformCoord(_camera.GetDefaultRightVector(), _camera.GetVectorRotationMatrix()));

	_scrollCount = 0;

	// Do any updates to the scene graph nodes
	UpdateSceneGraph();
	// Now apply any updates that have been made to world transformations
	// to all the nodes
	Matrix identity;
	_sceneGraph->Update(identity);
}

void DirectXFramework::Render()
{
	// Clear the render target and the depth stencil view
	_deviceContext->ClearRenderTargetView(_renderTargetView.Get(), _backgroundColour);
	_deviceContext->ClearDepthStencilView(_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	// Camera
	_viewTransformation = XMMatrixLookAtLH(_camera.GetEyePosition(), _camera.GetFocalPointPosition(), _camera.GetUpVector());
	_projectionTransformation = XMMatrixPerspectiveFovLH(_camera.GetFOV(), static_cast<float>(GetWindowWidth()) / GetWindowHeight(), 1.0f, _camera.GetRenderDistance());

	// Now recurse through the scene graph, rendering each object
	_sceneGraph->Render();
	// Now display the scene
	ThrowIfFailed(_swapChain->Present(0, 0));
}

void DirectXFramework::OnResize(WPARAM wParam)
{
	// We only want to resize the buffers when the user has 
	// finished dragging the window to the new size.  Windows
	// sends a value of WM_EXITSIZEMOVE to WM_SIZE when the
	// resizing is complete.
	if (wParam != WM_EXITSIZEMOVE)
	{
		return;
	}

	// Update view and projection matrices to allow for the window size change
	_viewTransformation = XMMatrixLookAtLH(_camera.GetEyePosition(), _camera.GetFocalPointPosition(), _camera.GetUpVector());
	_projectionTransformation = XMMatrixPerspectiveFovLH(_camera.GetFOV(), static_cast<float>(GetWindowWidth()) / GetWindowHeight(), 1.0f, _camera.GetRenderDistance());


	// This will free any existing render and depth views (which
	// would be the case if the window was being resized)
	_renderTargetView = nullptr;
	_depthStencilView = nullptr;
	_depthStencilBuffer = nullptr;

	ThrowIfFailed(_swapChain->ResizeBuffers(1, GetWindowWidth(), GetWindowHeight(), DXGI_FORMAT_R8G8B8A8_UNORM, 0));

	// Create a drawing surface for DirectX to render to
	ComPtr<ID3D11Texture2D> backBuffer;
	ThrowIfFailed(_swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer)));
	ThrowIfFailed(_device->CreateRenderTargetView(backBuffer.Get(), NULL, _renderTargetView.GetAddressOf()));
	
	// The depth buffer is used by DirectX to ensure
	// that pixels of closer objects are drawn over pixels of more
	// distant objects.

	// First, we need to create a texture (bitmap) for the depth buffer
	D3D11_TEXTURE2D_DESC depthBufferTexture = { 0 };
	depthBufferTexture.Width = GetWindowWidth();
	depthBufferTexture.Height = GetWindowHeight();
	depthBufferTexture.ArraySize = 1;
	depthBufferTexture.MipLevels = 1;
	depthBufferTexture.SampleDesc.Count = 4;
	depthBufferTexture.Format = DXGI_FORMAT_D32_FLOAT;
	depthBufferTexture.Usage = D3D11_USAGE_DEFAULT;
	depthBufferTexture.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	// Create the depth buffer.  
	ComPtr<ID3D11Texture2D> depthBuffer;
	ThrowIfFailed(_device->CreateTexture2D(&depthBufferTexture, NULL, depthBuffer.GetAddressOf()));
	ThrowIfFailed(_device->CreateDepthStencilView(depthBuffer.Get(), 0, _depthStencilView.GetAddressOf()));

	// Bind the render target view buffer and the depth stencil view buffer to the output-merger stage
	// of the pipeline. 
	_deviceContext->OMSetRenderTargets(1, _renderTargetView.GetAddressOf(), _depthStencilView.Get());

	// Specify a viewport of the required size
	D3D11_VIEWPORT viewPort = { 0 };
	viewPort.Width = static_cast<float>(GetWindowWidth());
	viewPort.Height = static_cast<float>(GetWindowHeight());
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	_deviceContext->RSSetViewports(1, &viewPort);
}

bool DirectXFramework::GetDeviceAndSwapChain()
{
	UINT createDeviceFlags = 0;

	// We are going to only accept a hardware driver or a WARP
	// driver
	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP
	};
	unsigned int totalDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0
	};
	unsigned int totalFeatureLevels = ARRAYSIZE(featureLevels);

	DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = GetWindowWidth();
	swapChainDesc.BufferDesc.Height = GetWindowHeight();
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	// Set the refresh rate to 0 and let DXGI determine the best option (refer to DXGI best practices)
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = GetHWnd();
	// Start out windowed
	swapChainDesc.Windowed = true;
	// Enable multi-sampling to give smoother lines (set to 1 if performance becomes an issue)
	swapChainDesc.SampleDesc.Count = 4;
	swapChainDesc.SampleDesc.Quality = 0;

	// Loop through the driver types to determine which one is available to us
	D3D_DRIVER_TYPE driverType = D3D_DRIVER_TYPE_UNKNOWN;

	for (unsigned int driver = 0; driver < totalDriverTypes && driverType == D3D_DRIVER_TYPE_UNKNOWN; driver++)
	{
		if (SUCCEEDED(D3D11CreateDeviceAndSwapChain(0,
			driverTypes[driver],
			0,
			createDeviceFlags,
			featureLevels,
			totalFeatureLevels,
			D3D11_SDK_VERSION,
			&swapChainDesc,
			_swapChain.GetAddressOf(),
			_device.GetAddressOf(),
			0,
			_deviceContext.GetAddressOf()
		)))

		{
			driverType = driverTypes[driver];
		}
	}
	if (driverType == D3D_DRIVER_TYPE_UNKNOWN)
	{
		// Unable to find a suitable device driver
		return false;
	}
	return true;
}

