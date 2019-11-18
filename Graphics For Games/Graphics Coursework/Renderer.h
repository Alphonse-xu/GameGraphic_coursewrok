#pragma once

#include "../../nclgl/OGLRenderer.h"
#include "../../nclgl/Camera.h"

#include "SceneNode.h"
#include "Island.h"

class Renderer : public OGLRenderer	{
public:
	Renderer(Window &parent);
	virtual ~Renderer(void);

	//渲染画面
	virtual void UpdateScene(float msec);
	virtual void RenderScene();

protected:
	void DrawNode(SceneNode* n);

	SceneNode* root;
	Camera* camera;
	
};
