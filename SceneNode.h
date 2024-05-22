#pragma once
#include "core.h"
#include "DirectXCore.h"
#include "Camera.h"

using namespace std;

// Abstract base class for all nodes of the scene graph.  
// This scene graph implements the Composite Design Pattern

class SceneNode;

typedef shared_ptr<SceneNode>	SceneNodePointer;

class SceneNode : public enable_shared_from_this<SceneNode>
{
public:
	SceneNode(wstring name)
	{
		_name = name;
		_colour = Vector4(Colors::Gray);
		_directionalLightColour = Vector4(Colors::Gray);
		_directionalLightVector = Vector4(-2.0f, 0.0f, 1.0f, 0.0f);
		_pointLightColour = Vector4(Colors::Cyan);
		_pointLightPosition = Vector3(0.0f, 0.0f, -3.0f);
		_pointLightRange = 16.0f;
		_specularColour = Vector4(Colors::White);
		_specularPower = 32.0f;
	};
	SceneNode(wstring name, Vector4 colour, Vector4 dlc, Vector4 dlv, Vector4 plc, Vector3 plp, float plr, Vector4 sc, float sp)
	{
		_name = name;
		_colour = colour;
		_directionalLightColour = dlc;
		_directionalLightVector = dlv;
		_pointLightColour = plc;
		_pointLightPosition = plp;
		_pointLightRange = plr;
		_specularColour = sc;
		_specularPower = sp;
	};
	~SceneNode(void) {};

	// Core methods
	virtual bool Initialise() = 0;
	virtual void Update(const Matrix& worldTransformation) { _cumulativeWorldTransformation = _thisWorldTransformation * worldTransformation; }	
	virtual void Render() = 0;
	virtual void Shutdown() = 0;

	void SetWorldTransform(const Matrix& worldTransformation) { _thisWorldTransformation = worldTransformation; }
		
	// Although only required in the composite class, these are provided
	// in order to simplify the code base for recursive operations

	virtual void Add(SceneNodePointer node) {}
	virtual void Remove(SceneNodePointer node) {};
	virtual	SceneNodePointer Find(wstring name) { return (_name == name) ? shared_from_this() : nullptr; }

protected:
	Matrix				_thisWorldTransformation;
	Matrix				_cumulativeWorldTransformation;
	wstring				_name;

	Vector4				_colour;
	Vector4				_directionalLightColour;
	Vector4				_directionalLightVector;
	Vector4				_pointLightColour;
	Vector3				_pointLightPosition;
	float				_pointLightRange{ 0.0f };
	Vector4				_specularColour;
	float				_specularPower{ 0.0f };
};

