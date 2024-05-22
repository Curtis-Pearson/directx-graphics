#include "TeapotNode.h"
#include "TeapotGeometry.h"

bool TeapotNode::Initialise()
{
	BuildVertices();
	BuildNormals();
	BuildGeometryBuffers();
	BuildShaders();
	BuildVertexLayout();
	BuildConstantBuffer();
	BuildRasteriserState();

	return true;
}

void TeapotNode::Render()
{
	// Create a complete matrix of the cumulative world, view, and projection transformations
	Matrix completeTransformation = _cumulativeWorldTransformation * DirectXFramework::GetDXFramework()->GetViewTransformation() * DirectXFramework::GetDXFramework()->GetProjectionTransformation();
	CBuffer constantBuffer;
	// Apply the transformations to the constant buffer
	constantBuffer.WorldViewProjection = completeTransformation;
	constantBuffer.WorldTransformation = _cumulativeWorldTransformation;
	// Apply the colour to the cube
	constantBuffer.AmbientLightColour = _colour;
	// Create a directional light
	constantBuffer.DirectionalLightColour = _directionalLightColour;
	constantBuffer.DirectionalLightVector = _directionalLightVector;
	// Create a point light
	constantBuffer.PointLightColour = _pointLightColour;
	constantBuffer.PointLightPosition = _pointLightPosition;
	constantBuffer.PointLightRange = _pointLightRange;
	// Create the specular light
	constantBuffer.SpecularColour = _specularColour;
	constantBuffer.SpecularPower = _specularPower;
	// Get the eye position
	constantBuffer.EyePosition = DirectXFramework::GetDXFramework()->GetCamera().GetEyePosition();

	// Update the constant buffer. Note the layout of the constant buffer must match that in the shader
	_deviceContext->VSSetConstantBuffers(0, 1, _constantBuffer.GetAddressOf());
	_deviceContext->PSSetConstantBuffers(0, 1, _constantBuffer.GetAddressOf());
	_deviceContext->UpdateSubresource(_constantBuffer.Get(), 0, 0, &constantBuffer, 0, 0);

	// Now render the cube
	// Specify the distance between vertices and the starting point in the vertex buffer
	UINT32 stride = sizeof(Vertex);
	UINT32 offset = 0;
	// Set the vertex buffer and index buffer we are going to use
	_deviceContext->IASetVertexBuffers(0, 1, _vertexBuffer.GetAddressOf(), &stride, &offset);
	_deviceContext->IASetIndexBuffer(_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	// Specify the layout of the polygons (it will rarely be different to this)
	_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Specify the layout of the input vertices.  This must match the layout of the input vertices in the shader
	_deviceContext->IASetInputLayout(_layout.Get());

	// Specify the vertex and pixel shaders we are going to use
	_deviceContext->VSSetShader(_vertexShader.Get(), 0, 0);
	_deviceContext->PSSetShader(_pixelShader.Get(), 0, 0);

	// Specify details about how the object is to be drawn
	_deviceContext->RSSetState(_rasteriserState.Get());

	// Now draw the first cube
	_deviceContext->DrawIndexed(ARRAYSIZE(teapotIndices), 0, 0);
}

void TeapotNode::BuildVertices()
{
	// Populate the vertices array with Vertex3 fields containing the values from vertexFloats array and add a normal field
	for (int i = 0; i < ARRAYSIZE(vertexFloats); i += 3)
	{
		teapotVertices[i / 3] = { Vector3(vertexFloats[i], vertexFloats[i + 1], vertexFloats[i + 2]), Vector3(0.0f) };
	}
}

void TeapotNode::BuildNormals()
{
	UINT contributingCounts[ARRAYSIZE(teapotVertices)];

	// For each Vertex, set the corresponding contributing count array entry to 0.
	for (int i = 0; i < ARRAYSIZE(teapotVertices); i++)
	{
		contributingCounts[i] = 0;
	}

	// For each Polygon, calcuate its normal then add it to the vertex normal for each
	// of the 3 vertices included in the polygon, incrementing the correspending contributing count by 1
	for (int i = 0; i < ARRAYSIZE(teapotIndices); i += 3)
	{
		Vector3 vectorA = teapotVertices[teapotIndices[i + 1]].Position - teapotVertices[teapotIndices[i]].Position;
		Vector3 vectorB = teapotVertices[teapotIndices[i + 2]].Position - teapotVertices[teapotIndices[i]].Position;
		Vector3 normal = vectorA.Cross(vectorB);

		teapotVertices[teapotIndices[i]].Normal += normal;
		teapotVertices[teapotIndices[i + 1]].Normal += normal;
		teapotVertices[teapotIndices[i + 2]].Normal += normal;

		contributingCounts[teapotIndices[i]]++;
		contributingCounts[teapotIndices[i + 1]]++;
		contributingCounts[teapotIndices[i + 2]]++;
	}

	// For each Vertex, divide the summed vertex normals by the number of times they were contributed to
	// then normalise the resulting normal vector
	for (int i = 0; i < ARRAYSIZE(teapotVertices); i++)
	{
		teapotVertices[i].Normal = teapotVertices[i].Normal / (float)contributingCounts[i];
		teapotVertices[i].Normal.Normalize();
	}
}

void TeapotNode::BuildGeometryBuffers()
{
	// Setup the structure that specifies how big the vertex 
	// buffer should be
	D3D11_BUFFER_DESC vertexBufferDescriptor = { 0 };
	vertexBufferDescriptor.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBufferDescriptor.ByteWidth = sizeof(Vertex) * ARRAYSIZE(teapotVertices);
	vertexBufferDescriptor.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDescriptor.CPUAccessFlags = 0;
	vertexBufferDescriptor.MiscFlags = 0;
	vertexBufferDescriptor.StructureByteStride = 0;

	// Now set up a structure that tells DirectX where to get the
	// data for the vertices from
	D3D11_SUBRESOURCE_DATA vertexInitialisationData = { 0 };
	vertexInitialisationData.pSysMem = teapotVertices;

	// and create the vertex buffer
	ThrowIfFailed(_device->CreateBuffer(&vertexBufferDescriptor, &vertexInitialisationData, _vertexBuffer.GetAddressOf()));

	// Setup the structure that specifies how big the index 
	// buffer should be
	D3D11_BUFFER_DESC indexBufferDescriptor = { 0 };
	indexBufferDescriptor.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDescriptor.ByteWidth = sizeof(UINT) * ARRAYSIZE(teapotIndices);
	indexBufferDescriptor.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDescriptor.CPUAccessFlags = 0;
	indexBufferDescriptor.MiscFlags = 0;
	indexBufferDescriptor.StructureByteStride = 0;

	// Now set up a structure that tells DirectX where to get the
	// data for the indices from
	D3D11_SUBRESOURCE_DATA indexInitialisationData;
	indexInitialisationData.pSysMem = teapotIndices;

	// and create the index buffer
	ThrowIfFailed(_device->CreateBuffer(&indexBufferDescriptor, &indexInitialisationData, _indexBuffer.GetAddressOf()));
}

void TeapotNode::BuildShaders()
{
	DWORD shaderCompileFlags = 0;
#if defined( _DEBUG )
	shaderCompileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ComPtr<ID3DBlob> compilationMessages = nullptr;

	//Compile vertex shader
	HRESULT hr = D3DCompileFromFile(ShaderFileName,
		nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		VertexShaderName, "vs_5_0",
		shaderCompileFlags, 0,
		_vertexShaderByteCode.GetAddressOf(),
		compilationMessages.GetAddressOf());

	if (compilationMessages.Get() != nullptr)
	{
		// If there were any compilation messages, display them
		MessageBoxA(0, (char*)compilationMessages->GetBufferPointer(), 0, 0);
	}
	// Even if there are no compiler messages, check to make sure there were no other errors.
	ThrowIfFailed(hr);
	ThrowIfFailed(_device->CreateVertexShader(_vertexShaderByteCode->GetBufferPointer(), _vertexShaderByteCode->GetBufferSize(), NULL, _vertexShader.GetAddressOf()));

	// Compile pixel 
	hr = D3DCompileFromFile(ShaderFileName,
		nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		PixelShaderName, "ps_5_0",
		shaderCompileFlags, 0,
		_pixelShaderByteCode.GetAddressOf(),
		compilationMessages.GetAddressOf());

	if (compilationMessages.Get() != nullptr)
	{
		// If there were any compilation messages, display them
		MessageBoxA(0, (char*)compilationMessages->GetBufferPointer(), 0, 0);
	}
	ThrowIfFailed(hr);
	ThrowIfFailed(_device->CreatePixelShader(_pixelShaderByteCode->GetBufferPointer(), _pixelShaderByteCode->GetBufferSize(), NULL, _pixelShader.GetAddressOf()));
}

void TeapotNode::BuildVertexLayout()
{
	ThrowIfFailed(_device->CreateInputLayout(teapotVertexDesc, ARRAYSIZE(teapotVertexDesc), _vertexShaderByteCode->GetBufferPointer(), _vertexShaderByteCode->GetBufferSize(), _layout.GetAddressOf()));
}

void TeapotNode::BuildConstantBuffer()
{
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(CBuffer);
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	ThrowIfFailed(_device->CreateBuffer(&bufferDesc, NULL, _constantBuffer.GetAddressOf()));
}

void TeapotNode::BuildRasteriserState()
{
	// Set default and wireframe rasteriser states
	D3D11_RASTERIZER_DESC rasteriserDesc;
	rasteriserDesc.CullMode = D3D11_CULL_BACK;
	rasteriserDesc.FrontCounterClockwise = false;
	rasteriserDesc.DepthBias = 0;
	rasteriserDesc.SlopeScaledDepthBias = 0.0f;
	rasteriserDesc.DepthBiasClamp = 0.0f;
	rasteriserDesc.DepthClipEnable = true;
	rasteriserDesc.ScissorEnable = false;
	rasteriserDesc.MultisampleEnable = false;
	rasteriserDesc.AntialiasedLineEnable = true;
	rasteriserDesc.FillMode = D3D11_FILL_SOLID;
	ThrowIfFailed(_device->CreateRasterizerState(&rasteriserDesc, _rasteriserState.GetAddressOf()));
}
