#pragma once
#include "SceneNode.h"
#include "DirectXFramework.h"

using namespace std;

class TexturedCubeNode : public SceneNode
{
public:
	TexturedCubeNode() : SceneNode(L"Textured Cube") { _textureFileName = L"Woodbox.bmp"; };
	TexturedCubeNode(wstring name, Vector4 colour, Vector4 dlc, Vector4 dlv, Vector4 plc, Vector3 plp, float plr, Vector4 sc, float sp, wstring textureFileName) : 
		SceneNode(name, colour, dlc, dlv, plc, plp, plr, sc, sp) { _textureFileName = textureFileName; };
	~TexturedCubeNode() {};

	virtual bool Initialise(void) override;
	virtual void Render(void) override;
	virtual void Shutdown(void) override {};

private:
	ComPtr<ID3D11Device>			_device = DirectXFramework::GetDXFramework()->GetDevice();
	ComPtr<ID3D11DeviceContext>		_deviceContext = DirectXFramework::GetDXFramework()->GetDeviceContext();

	ComPtr<ID3D11ShaderResourceView> _texture;

	ComPtr<ID3D11Buffer>			_vertexBuffer;
	ComPtr<ID3D11Buffer>			_indexBuffer;

	ComPtr<ID3DBlob>				_vertexShaderByteCode = nullptr;
	ComPtr<ID3DBlob>				_pixelShaderByteCode = nullptr;
	ComPtr<ID3D11VertexShader>		_vertexShader;
	ComPtr<ID3D11PixelShader>		_pixelShader;
	ComPtr<ID3D11InputLayout>		_layout;
	ComPtr<ID3D11Buffer>			_constantBuffer;

	ComPtr<ID3D11RasterizerState>   _rasteriserState;

	wstring							_textureFileName;

	void BuildNormals();
	void BuildGeometryBuffers();
	void BuildShaders();
	void BuildVertexLayout();
	void BuildConstantBuffer();
	void BuildRasteriserState();
	void BuildTexture();
};
