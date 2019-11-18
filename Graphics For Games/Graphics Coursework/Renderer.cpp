#include "Renderer.h"

Renderer::Renderer(Window &parent) : OGLRenderer(parent){
	//创建小岛
	SmallIsland::CreateIsland();

	//相机初始化
	camera = new Camera();
	camera->SetPosition(Vector3(0, 30, 175));

	currentShader = new Shader(SHADERDIR"CourseVertexShader.glsl",SHADERDIR"CourseFragmentShader.glsl");
	if(!currentShader->LinkProgram()) {
		return ;
	}

	projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);
	root = new SceneNode;
	root->AddChild(new SmallIsland());
	
	glEnable(GL_DEPTH_TEST);
	init = true;
}

Renderer::~Renderer(void)	{
	delete root;
	delete camera;
	SmallIsland::DeleteIsland();
}

void Renderer::UpdateScene(float msec) {
	camera->UpdateCamera(msec);
	viewMatrix = camera->BuildViewMatrix();
	root->Update(msec);
}

void Renderer::RenderScene()	{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT); //清屏开启深度测试

	glUseProgram(currentShader->GetProgram());
	UpdateShaderMatrices();

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);//

	DrawNode(root);

	glUseProgram(0);
	SwapBuffers();	
}

void Renderer::DrawNode(SceneNode* n) {
	if (n->GetMesh()) {
		Matrix4 transform = n->GetWorldTransform() * Matrix4::Scale(n->GetModelScale());
		glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "modelMatrix"), 1, false, (float*)&transform);
		//location, size count,transpose,value

		glUniform4fv(glGetUniformLocation(currentShader->GetProgram(), "nodeColour"), 1, (float*)&n->GetColour());
		//location, size count,value

		glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "useTexture"), (int)n->GetMesh()->GetTexture());
		//location valuw
		n->Draw(*this);
	}

	for (vector < SceneNode* >::const_iterator i = n->GetChildIteratorStart(); i != n->GetChildIteratorEnd(); ++i) {
		DrawNode(*i);
	}

}