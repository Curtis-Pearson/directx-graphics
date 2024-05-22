#include "DirectXApp.h"
#include "CubeNode.h"
#include "TexturedCubeNode.h"
#include "TeapotNode.h"

DirectXApp app;

void DirectXApp::CreateSceneGraph()
{
	// Get the Scene Graph
	SceneGraphPointer sceneGraph = GetSceneGraph();

	// Create a non-textured cube
	shared_ptr<CubeNode> cube = make_shared<CubeNode>(
		L"Cube",
		Vector4(Colors::Gray),
		Vector4(Colors::Blue),
		Vector4(-2.0f, 0.0f, 1.0f, 0.0f),
		Vector4(Colors::Green),
		Vector3(2.0f, 0.0f, -3.0f),
		16.0f,
		Vector4(Colors::White),
		32.0f
	);
	cube->SetWorldTransform(Matrix::CreateTranslation(Vector3(4.0f, 0.0f, 0.0f)));
	sceneGraph->Add(cube);

	// Create a non-textured teapot
	shared_ptr<TeapotNode> teapot = make_shared<TeapotNode>(
		L"Teapot",
		Vector4(Colors::Gray),
		Vector4(Colors::Green),
		Vector4(-2.0f, 0.0f, 1.0f, 0.0f),
		Vector4(Colors::Red),
		Vector3(0.0f, 0.0f, -3.0f),
		16.0f,
		Vector4(Colors::White),
		32.0f
	);
	teapot->SetWorldTransform(Matrix::CreateTranslation(Vector3(0.0f, 0.0f, 0.0f)));
	sceneGraph->Add(teapot);

	// Create a textured cube
	shared_ptr<TexturedCubeNode> texturedCube = make_shared<TexturedCubeNode>(
		L"Textured Cube",
		Vector4(Colors::Gray),
		Vector4(Colors::Red),
		Vector4(-2.0f, 0.0f, 1.0f, 0.0f),
		Vector4(Colors::Blue),
		Vector3(-2.0f, 0.0f, -3.0f),
		16.0f,
		Vector4(Colors::White),
		32.0f,
		L"Woodbox.bmp"
	);
	texturedCube->SetWorldTransform(Matrix::CreateTranslation(Vector3(-4.0f, 0.0f, 0.0f)));
	sceneGraph->Add(texturedCube);
}

void DirectXApp::UpdateSceneGraph()
{
	// Get the Scene Graph
	SceneGraphPointer sceneGraph = GetSceneGraph();

	// Get the non-textured cube and apply rotation
	SceneNodePointer object = sceneGraph->Find(L"Cube");
	object->SetWorldTransform(Matrix::CreateRotationY(_rotationAngle * XM_PI / 180.0f) *
		Matrix::CreateRotationX(_rotationAngle * XM_PI / 180.0f) *
		Matrix::CreateTranslation(Vector3(4.0f, 0.0f, 0.0f)));

	// Get the non-textured teapot and apply rotation
	object = sceneGraph->Find(L"Teapot");
	object->SetWorldTransform(Matrix::CreateRotationY(-_rotationAngle * XM_PI / 180.0f) * 
		Matrix::CreateRotationZ(-_rotationAngle * XM_PI / 180.0f) *
		Matrix::CreateTranslation(Vector3(0.0f, 0.0f, 0.0f)));

	// Get the textured cube and apply rotation
	object = sceneGraph->Find(L"Textured Cube");
	object->SetWorldTransform(Matrix::CreateRotationX(_rotationAngle * XM_PI / 180.0f) * 
		Matrix::CreateRotationZ(_rotationAngle * XM_PI / 180.0f) *
		Matrix::CreateTranslation(Vector3(-4.0f, 0.0f, 0.0f)));

	// Update the rotation angle
	_rotationAngle = (_rotationAngle + 1) % 360;
}
