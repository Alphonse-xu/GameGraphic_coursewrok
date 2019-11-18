#pragma once
#include"OGLRenderer.h"
enum MeshBuffer {
	VERTEX_BUFFER, COLOUR_BUFFER,TEXTURE_BUFFER, INDEX_BUFFER, MAX_BUFFER
};
class Mesh
{
public:
	Mesh(void);
	~Mesh(void);

	static Mesh* GenerateQuad();

	//渲染
	virtual void Draw(); 
	static Mesh* GenerateTriangle(); //返回一个MESH类的指针，包含一个彩色三角形的VAO和VBO数据

	//纹理
	void SetTexture(GLuint tex) { texture = tex; }
	GLuint GetTexture() { return texture; }

protected:
	void BufferData(); //复制网格顶点数据到显存中
	GLuint arrayObject; //VAO
	GLuint bufferObject[MAX_BUFFER]; //VBO数组 位置颜色
	GLuint numVertices; //记录网格顶点个数
	GLuint type; //顶点的绘制类型（图元类型）
	GLuint texture; //纹理句柄

	Vector2* textureCoords;//纹理坐标
	Vector3* vertices; //存储顶点
	Vector4* colours; //存储颜色

	//EBO
	GLuint numIndices;
	unsigned int* indices;
};

