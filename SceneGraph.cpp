#include "SceneGraph.h"

bool SceneGraph::Initialise()
{
	for (SceneNodePointer child : _children)
	{
		if (!child->Initialise())
		{
			return false;
		}
	}
	return true;
}

void SceneGraph::Update(const Matrix& worldTransformation)
{
	_cumulativeWorldTransformation = _thisWorldTransformation * worldTransformation;
	for (SceneNodePointer child : _children)
	{
		child->Update(_cumulativeWorldTransformation);
	}
}

void SceneGraph::Render()
{
	for (SceneNodePointer child : _children)
	{
		child->Render();
	}
}

void SceneGraph::Shutdown()
{
	for (SceneNodePointer child : _children)
	{
		child->Shutdown();
	}
}

void SceneGraph::Add(SceneNodePointer node)
{
	_children.push_back(node);
}

void SceneGraph::Remove(SceneNodePointer node)
{
	for (SceneNodePointer child : _children)
	{
		child->Remove(node);
		if (child == node)
		{
			_children.erase(find(_children.begin(), _children.end(), child));
			break;
		}
	}
}

SceneNodePointer SceneGraph::Find(wstring name)
{
	if (_name == name)
	{
		return static_cast<SceneNodePointer>(this);
	}
	for (SceneNodePointer child : _children)
	{
		if (child->Find(name) != nullptr)
		{
			return child;
		}
	}
	return nullptr;
}
