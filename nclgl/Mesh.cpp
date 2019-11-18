#include "Mesh.h"

Mesh::Mesh(void) {
	//初始化VBO数组
	for (int i = 0; i < MAX_BUFFER; ++i) {
		bufferObject[i] = 0;
	}
	glGenVertexArrays(1, &arrayObject);  //创建VAO

	//初始化
	texture = 0;
	textureCoords = NULL;
	numVertices = 0;
	vertices = NULL;
	colours = NULL;
	type = GL_TRIANGLES; //绘制类型设置为三角形
	indices = NULL;
	numIndices = 0;
}

Mesh ::~Mesh(void) {
	glDeleteVertexArrays(1, &arrayObject);
	glDeleteBuffers(MAX_BUFFER, bufferObject);
	glDeleteTextures(1, &texture); //删除坐标和纹理
	//glDeleteTextures(1, &bumpTexture);

	delete[] textureCoords;
	delete[] vertices;
	delete[] colours;
	delete[] indices;
}

//顶点属性
Mesh* Mesh::GenerateTriangle() {
	Mesh * m = new Mesh();
	m -> numVertices = 3; //一个三角形三个顶点

	//顶点
	m -> vertices = new Vector3[m -> numVertices]; //顶点指针顺序右上，右下，左下
	m -> vertices[0] = Vector3(0.0f, 0.5f, 0.0f);
	m -> vertices[1] = Vector3(0.5f, -0.5f, 0.0f);
	m -> vertices[2] = Vector3(-0.5f, -0.5f, 0.0f);

	//颜色
	m -> colours = new Vector4[m -> numVertices]; //颜色指针红色，绿色和蓝色
	m -> colours[0] = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
	m -> colours[1] = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
	m -> colours[2] = Vector4(0.0f, 0.0f, 1.0f, 1.0f);

	//纹理
	m->textureCoords = new Vector2[m->numVertices];
	m->textureCoords[0] = Vector2(0.5f, 0.0f);
	m->textureCoords[1] = Vector2(1.0f, 1.0f);
	m->textureCoords[2] = Vector2(0.0f, 1.0f);

	m -> BufferData(); //顶点数据传入显存
	return m;
}

//缓存传输
void Mesh::BufferData() { //VBO数组分别保存了位置和颜色


	//位置VBO
	glBindVertexArray(arrayObject); 
	glGenBuffers(1, &bufferObject[VERTEX_BUFFER]); //创建VBO名称，将其名称存储在我们的bufferObject数组的第一个索引中
	glBindBuffer(GL_ARRAY_BUFFER, bufferObject[VERTEX_BUFFER]);
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vector3), vertices, GL_STATIC_DRAW);
	//sizeof(float)表示了数组两个元素之间的跨度，而函数说明上写着，如果元素是紧密排列的，也可以使用0来做参数，达到一样的效果。
	glVertexAttribPointer(VERTEX_BUFFER, 3, GL_FLOAT, GL_FALSE, 0, 0);  
	glEnableVertexAttribArray(VERTEX_BUFFER);//设置开启顶点属性（索引为0的属性,与shader中的内容有交互）

	//纹理VBO
	if (textureCoords) { 
		glGenBuffers(1, &bufferObject[TEXTURE_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, bufferObject[TEXTURE_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vector2),textureCoords, GL_STATIC_DRAW);
		glVertexAttribPointer(TEXTURE_BUFFER, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(TEXTURE_BUFFER);
	}

	//EBO
	if (indices) {
		glGenBuffers(1, &bufferObject[INDEX_BUFFER]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
		bufferObject[INDEX_BUFFER]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(GLuint),
		indices, GL_STATIC_DRAW);
		
	}

	//颜色VBO
	if (colours) { // Just in case the data has no colour attribute ...
		glGenBuffers(1, &bufferObject[COLOUR_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, bufferObject[COLOUR_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vector4),colours, GL_STATIC_DRAW);
		glVertexAttribPointer(COLOUR_BUFFER, 4, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(COLOUR_BUFFER);
	}
	glBindVertexArray(0); //解绑VAO（解绑主要是为了不影响后续VAO的设置，有点类似于C++中指针delete后置空，是个好习惯）
}

//绘制图像
void Mesh::Draw() {
	glBindTexture(GL_TEXTURE_2D, texture);
	glBindVertexArray(arrayObject);
	glDrawArrays(type, 0, numVertices); //参数 绘制的基本类型,绘制的索引0，绘制顶点个数值存储在numVertices中

	glBindVertexArray(arrayObject);
	if (bufferObject[INDEX_BUFFER]) {
		glDrawElements(type, numIndices, GL_UNSIGNED_INT, 0);
		
	}
	else {
		glDrawArrays(type, 0, numVertices);
		
	}
	glBindVertexArray(0);



	//解绑
	glBindVertexArray(0); //解绑VAO
	glBindTexture(GL_TEXTURE_2D, 0);
}

//生成一个正方形
Mesh* Mesh::GenerateQuad() {
	Mesh * m = new Mesh();
	m->numVertices = 4;
	m->type = GL_TRIANGLE_STRIP;
	
	m->vertices = new Vector3[m->numVertices];
	m->textureCoords = new Vector2[m->numVertices];
	m->colours = new Vector4[m->numVertices];
	
	m->vertices[0] = Vector3(-1.0f, -1.0f, 0.0f);
	m->vertices[1] = Vector3(-1.0f, 1.0f, 0.0f);
	m->vertices[2] = Vector3(1.0f, -1.0f, 0.0f);
	m->vertices[3] = Vector3(1.0f, 1.0f, 0.0f);
	
	m->textureCoords[0] = Vector2(0.0f, 1.0f);
	m->textureCoords[1] = Vector2(0.0f, 0.0f);
	m->textureCoords[2] = Vector2(1.0f, 1.0f);
	m->textureCoords[3] = Vector2(1.0f, 0.0f);
	
	for (int i = 0; i < 4; ++i) {
		m->colours[i] = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		
	}
	
		m->BufferData();
	return m;
	
}