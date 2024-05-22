#pragma once
#include "Framework.h"
#include "DirectXCore.h"
#include "SimpleMath.h"

using namespace SimpleMath;

class Camera
{
public:
	Camera()
	{
		_mouseSensitivity = 0.05f;
		_moveSpeed = 0.1f;
		_moveSpeedAdd = 0.2f;
		_scrollSpeed = 0.5f;
		_scrollSpeedAdd = 1.0f;
		_fov = XM_PIDIV4;
		_renderDistance = 200.0f;

		_eyePosition = Vector3(0.0f, 0.0f, -10.0f);
		_eyeRotation = Vector3(0.0f, 0.0f, 0.0f);
		_focalPointPosition = Vector3(0.0f, 0.0f, 0.0f);

		_upVector = Vector3(0.0f, 1.0f, 0.0f);
		_forwardVector = Vector3(0.0f, 0.0f, 1.0f);
		_backwardVector = Vector3(0.0f, 0.0f, -1.0f);
		_leftVector = Vector3(-1.0f, 0.0f, 0.0f);
		_rightVector = Vector3(1.0f, 0.0f, 0.0f);
	};
	Camera(Vector3 eyePosition, Vector3 focalPointPosition, float mouseSensitivity, float moveSpeed, float moveSpeedAdd, float scrollSpeed, float scrollSpeedAdd, float fov, float renderDistance)
	{
		_mouseSensitivity = mouseSensitivity;
		_moveSpeed = moveSpeed;
		_moveSpeedAdd = moveSpeedAdd;
		_scrollSpeed = scrollSpeed;
		_scrollSpeedAdd = scrollSpeedAdd;
		_fov = fov;
		_renderDistance = renderDistance;

		_eyePosition = Vector3(0.0f, 0.0f, -10.0f);
		_eyeRotation = Vector3(0.0f, 0.0f, 0.0f);
		_focalPointPosition = Vector3(0.0f, 0.0f, 0.0f);

		_upVector = Vector3(0.0f, 1.0f, 0.0f);
		_forwardVector = Vector3(0.0f, 0.0f, 1.0f);
		_backwardVector = Vector3(0.0f, 0.0f, -1.0f);
		_leftVector = Vector3(-1.0f, 0.0f, 0.0f);
		_rightVector = Vector3(1.0f, 0.0f, 0.0f);
	};
	~Camera() {};

	float	GetMouseSensitivity() const { return _mouseSensitivity; };
	void	SetMouseSensitivity(const float mouseSensitivity) { _mouseSensitivity = mouseSensitivity; };

	float	GetMoveSpeed() const { return _moveSpeed; };
	void	SetMoveSpeed(const float moveSpeed) { _moveSpeed = moveSpeed; };
	void	AdjustMoveSpeed(const float moveSpeed) { _moveSpeed += moveSpeed; };

	float	GetMoveSpeedAdd() const { return _moveSpeedAdd; };
	void	SetMoveSpeedAdd(const float moveSpeedAdd) { _moveSpeedAdd = moveSpeedAdd; };
	void	AdjustMoveSpeedAdd(const float moveSpeedAdd) { _moveSpeedAdd += moveSpeedAdd; };

	float	GetScrollSpeed() const { return _scrollSpeed; };
	void	SetScrollSpeed(const float scrollSpeed) { _scrollSpeed = scrollSpeed; };
	void	AdjustScrollSpeed(const float scrollSpeed) { _scrollSpeed += scrollSpeed; };

	float	GetScrollSpeedAdd() const { return _scrollSpeedAdd; };
	void	SetScrollSpeedAdd(const float scrollSpeedAdd) { _scrollSpeedAdd = scrollSpeedAdd; };
	void	AdjustScrollSpeedAdd(const float scrollSpeedAdd) { _scrollSpeedAdd += scrollSpeedAdd; };

	float	GetFOV() const { return _fov; };
	void	SetFOV(const float fov) { _fov = fov; };

	float	GetRenderDistance() const { return _renderDistance; };
	void	SetRenderDistance(const float renderDistance) { _renderDistance = renderDistance; };

	Vector3 GetEyePosition() const { return _eyePosition; };
	void	SetEyePosition(const Vector3 eyePosition) { _eyePosition = eyePosition; };
	void	AdjustEyePosition(const Vector3 eyePosition) { _eyePosition += eyePosition; };

	Vector3 GetEyeRotation() const { return _eyeRotation; };
	void	SetEyeRotation(const Vector3 eyeRotation) { _eyeRotation = eyeRotation; };
	void	AdjustEyeRotation(const Vector3 eyeRotation) { _eyeRotation += eyeRotation; };
	float	GetEyeRotationX() const { return _eyeRotation.x; };
	void	SetEyeRotationX(const float x) { _eyeRotation.x = x; };
	float	GetEyeRotationY() const { return _eyeRotation.y; };
	void	SetEyeRotationY(const float y) { _eyeRotation.y = y; };
	float	GetEyeRotationZ() const { return _eyeRotation.z; };
	void	SetEyeRotationZ(const float z) { _eyeRotation.z = z; };

	Vector3 GetFocalPointPosition() const { return _focalPointPosition; };
	void	SetFocalPointPosition(const Vector3 focalPointPosition) { _focalPointPosition = focalPointPosition; };
	void	AdjustFocalPointPosition(const Vector3 focalPointPosition) { _focalPointPosition += focalPointPosition; };

	Vector3 GetDefaultUpVector() const { return _defaultUp; };
	Vector3 GetDefaultFowardVector() const { return _defaultForward; };
	Vector3 GetDefaultBackwardVector() const { return _defaultBackward; };
	Vector3 GetDefaultLeftVector() const { return _defaultLeft; };
	Vector3 GetDefaultRightVector() const { return _defaultRight; };

	Vector3 GetUpVector() const { return _upVector; };
	void	SetUpVector(const Vector3 upVector) { _upVector = upVector; };

	Vector3 GetFowardVector() const { return _forwardVector; };
	void	SetFowardVector(const Vector3 forwardVector) { _forwardVector = forwardVector; };

	Vector3 GetBackwardVector() const { return _backwardVector; };
	void	SetBackwardVector(const Vector3 backwardVector) { _backwardVector = backwardVector; };

	Vector3 GetLeftVector() const { return _leftVector; };
	void	SetLeftVector(const Vector3 leftVector) { _leftVector = leftVector; };

	Vector3 GetRightVector() const { return _rightVector; };
	void	SetRightVector(const Vector3 rightVector) { _rightVector = rightVector; };

	Matrix	GetRotationMatrix() const { return _rotationMatrix; };
	void	SetRotationMatrix(const Matrix rotationMatrix) { _rotationMatrix = rotationMatrix; };

	Matrix	GetVectorRotationMatrix() const { return _vectorRotationMatrix; };
	void	SetVectorRotationMatrix(const Matrix vectorRotationMatrix) { _vectorRotationMatrix = vectorRotationMatrix; };

private:
	float	_mouseSensitivity;
	float	_moveSpeed;
	float	_moveSpeedAdd;
	float	_scrollSpeed;
	float	_scrollSpeedAdd;
	float	_fov;
	float	_renderDistance;

	Vector3 _eyePosition;
	Vector3 _eyeRotation;
	Vector3 _focalPointPosition;

	Vector3 _defaultUp = Vector3(0.0f, 1.0f, 0.0f);
	Vector3 _defaultForward = Vector3(0.0f, 0.0f, 1.0f);
	Vector3 _defaultBackward = Vector3(0.0f, 0.0f, -1.0f);
	Vector3 _defaultLeft = Vector3(-1.0f, 0.0f, 0.0f);
	Vector3 _defaultRight = Vector3(1.0f, 0.0f, 0.0f);

	Vector3 _upVector;
	Vector3 _forwardVector;
	Vector3 _backwardVector;
	Vector3 _leftVector;
	Vector3 _rightVector;

	Matrix	_rotationMatrix;
	Matrix	_vectorRotationMatrix;
};