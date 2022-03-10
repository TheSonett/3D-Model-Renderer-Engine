#pragma once

#include "GL/glew.h"

class Mesh
{
public:
	Mesh();
	~Mesh();
public:
	void CreateMesh(float* vertices, unsigned int* indices, unsigned int numberOfVertices, unsigned int numberOfIndices);
	void RenderMesh();
	void ClearMesh();

private:
	unsigned int VAO, VBO, IBO;
	unsigned int indexCount;
};