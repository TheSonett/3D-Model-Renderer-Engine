#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <chrono>

#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "vendor/glm/gtc/type_ptr.hpp"

#include "Window.h"
#include "Shader.h"
#include "Mesh.h"
#include "Camera.h"
#include "Texture.h"
#include "Light.h"
#include "Material.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include"SpotLight.h"
#include "Model.h"
#include "Skybox.h"

std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
std::vector<std::string> skyboxFaces;

float deltaTime = 0.0f;
float lastTime = 0.0f;

PointLight pointLight[MAX_POINT_LIGHTS];
SpotLight spotLight[MAX_SPOT_LIGHTS];

unsigned int uniformProjection = 0, uniformModel = 0, uniformView = 0;
unsigned int uniformEyePosition = 0, uniformSpecularIntensity = 0, uniformShininess = 0;

void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, float* vertices,
	unsigned int verticeCount, unsigned int vLength, unsigned int normalOffSet) {

	for (unsigned int i = 0; i < indiceCount; i += 3) {
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;

		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffSet;
		in1 += normalOffSet;
		in2 += normalOffSet;

		vertices[in0] += normal.x;
		vertices[in0 + 1] += normal.y;
		vertices[in0 + 2] += normal.z;

		vertices[in1] += normal.x;
		vertices[in1 + 1] += normal.y;
		vertices[in1 + 2] += normal.z;

		vertices[in2] += normal.x;
		vertices[in2 + 1] += normal.y;
		vertices[in2 + 2] += normal.z;
	}

	for (unsigned int i = 0; i < verticeCount / vLength; i++) {
		unsigned int nOffSet = i * vLength + normalOffSet;

		glm::vec3 vec(vertices[nOffSet], vertices[nOffSet + 1], vertices[nOffSet + 2]);
		vec = glm::normalize(vec);

		vertices[nOffSet] = vec.x;
		vertices[nOffSet + 1] = vec.y;
		vertices[nOffSet + 2] = vec.z;
	}
}


int main()
{
	auto start = std::chrono::high_resolution_clock::now();

	Window window(1366, 768, "Graphics Window");
	window.Initialize();
	window.WindowCallBacks();

	GLfloat vertices[] = {
	   -1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f,  1.0f, 0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, -0.6f,	1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f,  1.0f,  0.0f,	0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	 0.0f, 0.0f,    0.0f, -1.0f, 0.0f,
		 10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	 0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		 10.0f, 0.0f, 10.0f,	10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	// Mesh Objects
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj2);

	// Shader Object
	Shader* shader1 = new Shader();
	shader1->CreateShader("src/shaders/shader.vert", "src/shaders/shader.frag");
	shaderList.push_back(*shader1);

	// Camera Object
	Camera camera;
	camera = Camera(glm::vec3(0.0f, 20.0f, 0.0f), glm::vec3(0.0f, 3.0f, 0.0f), -90.0f, 0.0f, 10.0f, 0.5f);

	// Light Object
	//Material shinyMaterial = Material(4.0f, 256);
	DirectionalLight dLight = DirectionalLight(1.0f, 1.0f, 1.0f, 0.3f, 0.5f, 0.0f, 0.0f, -1.0f);

	// Model Object
	Model sponza;
	sponza.LoadModel("src/models/sponza.obj");

	// Skybox Objects
	skyboxFaces.push_back("src/skybox/right.jpg");
	skyboxFaces.push_back("src/skybox/left.jpg");
	skyboxFaces.push_back("src/skybox/top.jpg");
	skyboxFaces.push_back("src/skybox/bottom.jpg");
	skyboxFaces.push_back("src/skybox/front.jpg");
	skyboxFaces.push_back("src/skybox/back.jpg");
	Skybox skybox = Skybox(skyboxFaces);

	/*unsigned int pointLightCount = 2;
	pointLight[0] = PointLight(0.0f, 0.0f, 1.0f,
							   0.0f, 1.0f,
							   1.0f, 2.0f, 0.0f,
							   0.3f, 0.2f, 0.1f);
	pointLight[1] = PointLight(0.0f, 1.0f, 0.0f,
							   0.0f, 1.0f,
							  -4.0f, 3.0f, 0.0f,
							   0.3f, 0.2f, 0.1f);

	unsigned int spotLightCount = 1;
	spotLight[0] = SpotLight(1.0f, 0.0f, 0.0f,
							 0.3f, 1.0f, 
							 0.0f, 0.0f, 0.0f, 
							 0.0f, -1.0f, 0.0f, 
							 0.3f, 0.2f, 0.1f, 
							 15.0f);*/

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)window.getBufferWidth() / window.getBufferHeight(), 0.1f, 600.0f);

	uniformModel = shaderList[0].GetModelLocation();
	uniformProjection = shaderList[0].GetProjectionLocation();
	uniformView = shaderList[0].GetViewLocation();

	uniformEyePosition = shaderList[0].GetEyeLocation();
	uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
	//uniformShininess = shaderList[0].GetShininessLocation();

	while (!window.getShouldClose())
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		float now = (float)glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;

		camera.keyControl(window.getKeys(), deltaTime);
		camera.mouseControl(window.getXChange(), window.getYChange());

		glEnable(GL_DEPTH_TEST);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);

		shaderList[0].UseShader();
		shaderList[0].SetDirectionalLight(&dLight);
		//shaderList[0].SetPointLight(pointLight, pointLightCount);
		//shaderList[0].SetSpotLight(spotLight, spotLightCount);

		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));

		/*float x, y, z;
		ImGui::Begin("3D Triangle");
		ImGui::SliderFloat("Scale X", &x, 0.2f, 5.0f);
		ImGui::SliderFloat("Scale Y", &y, 0.2f, 5.0f);
		ImGui::SliderFloat("Scale Z", &z, 0.2f, 5.0f);
		model = glm::scale(model, glm::vec3(x, y, z));
		ImGui::End();*/

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);
		//shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[0]->RenderMesh();

		glm::mat4 model2(1.0f);
		model2 = glm::translate(model2, glm::vec3(-7.0f, 0.0f, 0.0f));
		model2 = glm::scale(model2, glm::vec3(0.08f, 0.08f, 0.08f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model2));
		//shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		sponza.RenderModel();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		window.swapBuffers();
		glfwPollEvents();
	}

	// Destroy All
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	shaderList[0].ClearShader();
	shader1->EndShader();

	delete shader1;
	//delete obj1;

	std::chrono::steady_clock::time_point end = std::chrono::high_resolution_clock::now();
	auto duration = end - start;
	std::cout << "\nTime taken to load: " << std::chrono::duration_cast<std::chrono::seconds>(duration) << std::endl;
}