#pragma once
#include <vector>
#include <map>
#include "Framework.h"
#include "DirectXCore.h"
#include "SceneGraph.h"
#include "Camera.h"

class DirectXFramework : public Framework
{
public:
	DirectXFramework();
	DirectXFramework(unsigned int width, unsigned int height);

	virtual void CreateSceneGraph();
	virtual void UpdateSceneGraph();

	bool Initialise();
	void Update();
	void Render();
	void OnResize(WPARAM wParam);
	void Shutdown();

	static DirectXFramework *			GetDXFramework();

	inline SceneGraphPointer			GetSceneGraph() { return _sceneGraph; }
	inline ComPtr<ID3D11Device>			GetDevice() { return _device; }
	inline ComPtr<ID3D11DeviceContext>	GetDeviceContext() { return _deviceContext; }

	const Matrix&						GetViewTransformation() const;
	const Matrix&						GetProjectionTransformation() const;

	void								SetBackgroundColour(Vector4 backgroundColour);

	//Vector3							GetEyePosition() const { return _eyePosition; };
	Camera								GetCamera() const { return _camera; };

	void								OnMouseClick(const int x, const int y, const bool isDown);
	void								OnMouseMoveRaw(const int x, const int y);
	void								OnMouseWheel(const int x, const int y, const bool isDown);
	void								OnKey(const WPARAM wParam, const bool isDown);

private:
	ComPtr<ID3D11Device>				_device;
	ComPtr<ID3D11DeviceContext>			_deviceContext;
	ComPtr<IDXGISwapChain>				_swapChain;
	ComPtr<ID3D11Texture2D>				_depthStencilBuffer;
	ComPtr<ID3D11RenderTargetView>		_renderTargetView;
	ComPtr<ID3D11DepthStencilView>		_depthStencilView;

	D3D11_VIEWPORT						_screenViewport{ 0 };
	/*
	Vector3								_eyePosition;
	Vector3								_focalPointPosition;
	Vector3								_upVector;
	*/
	Matrix								_viewTransformation;
	Matrix								_projectionTransformation;

	SceneGraphPointer					_sceneGraph;

	float							    _backgroundColour[4];

	Camera								_camera;
	struct								MouseCoords { int x; int y; };

	int									_scrollCount{ 0 };
	bool								_mouseDown{ false };
	MouseCoords							_mouseCoords;
	bool								_isRunning{ false };
	map<int, bool>						_keysDown = {
											{87, false}, // Forward
											{65, false}, // Left
											{83, false}, // Back
											{68, false}, // Right
											{32, false}, // Up
											{67, false}, // Down
											{16, false}  // Shift
										};


	bool GetDeviceAndSwapChain();
};

