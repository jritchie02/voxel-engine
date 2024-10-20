
// ==================== Libraries ==================
// Depending on the operating system we use
// The paths to SDL are actually different.
// The #define statement should be passed in
// when compiling using the -D argument.
// This gives an example of how a programmer
// may support multiple platforms with different
// dependencies.
#if defined(LINUX) || defined(MINGW)
#include <SDL2/SDL.h>
#else // This works for Mac
#include <SDL.h>
#endif

// Third Party Libraries
#include <glad/glad.h>

// C++ Standard Template Library (STL)
#include <iostream>
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#include <map>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>
#include "ChunkManager.hpp"
#include "Camera.hpp"
#include "Voxel.hpp"
#include "Shader.hpp"

// vvvvvvvvvvvvvvvvvvvvvvvvvv Globals vvvvvvvvvvvvvvvvvvvvvvvvvv
// Globals generally are prefixed with 'g' in this application.

// Screen Dimensions
int gScreenWidth = 1280;
int gScreenHeight = 960;
SDL_Window *gGraphicsApplicationWindow = nullptr;
SDL_GLContext gOpenGLContext = nullptr;

// Main loop flag
bool gQuit = false; // If this is quit = 'true' then the program terminates.

// shader
// The following stores the a unique id for the graphics pipeline
// program object that will be used for our OpenGL draw calls.
GLuint gGraphicsPipelineShaderProgram = 0;

// OpenGL Objects
// Vertex Array Object (VAO)
// Vertex array objects encapsulate all of the items needed to render an object.
// For example, we may have multiple vertex buffer objects (VBO) related to rendering one
// object. The VAO allows us to setup the OpenGL state to render that object using the
// correct layout and correct buffers with one call after being setup.
GLuint gVoxelVAO = 0;
// Vertex Buffer Object (VBO)
// Vertex Buffer Objects store information relating to vertices (e.g. positions, normals, textures)
// VBOs are our mechanism for arranging geometry on the GPU.
GLuint gVoxelVBO = 0;
GLuint gVoxelIBO = 0;

GLuint gSunVAO = 0;		// New VAO for the sun
GLuint gSunVBO = 0;		// New VBO for the sun
GLuint gSunIBO = 0;		// New IBO for the sun
float gSunAngle = 0.0f; // Angle of the sun

// Get our cube voxel and sun voxel
Voxel sun(glm::vec3(20.0f, 40.0f, 20.0f), 1.5f, glm::vec2(0.0f, 0.0f));

// Solid Fill = true, Wireframe = false
bool drawType = true;

// Uniforms
float g_uOffset = -2.0f;
float g_uRotate = 0.0f;

// Camera
Camera gCamera;

std::vector<GLfloat> gVoxelVertexData;
std::vector<GLuint> gVoxelIndexBufferData;

std::vector<GLfloat> gSunVertexData;
std::vector<GLuint> gSunIndexBufferData;

// ^^^^^^^^^^^^^^^^^^^^^^^^ Globals ^^^^^^^^^^^^^^^^^^^^^^^^^^^

/**
 * Initialization of the graphics application. Typically this will involve setting up a window
 * and the OpenGL Context (with the appropriate version)
 *
 * @return void
 */
void InitializeProgram()
{
	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "SDL could not initialize! SDL Error: " << SDL_GetError() << "\n";
		exit(1);
	}

	// Setup the OpenGL Context
	// Use OpenGL 4.1 core or greater
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	// We want to request a double buffer for smooth updating.
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	// Create an application window using OpenGL that supports SDL
	gGraphicsApplicationWindow = SDL_CreateWindow("OpenGL First Program",
												  SDL_WINDOWPOS_UNDEFINED,
												  SDL_WINDOWPOS_UNDEFINED,
												  gScreenWidth,
												  gScreenHeight,
												  SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

	// Check if Window did not create.
	if (gGraphicsApplicationWindow == nullptr)
	{
		std::cout << "Window could not be created! SDL Error: " << SDL_GetError() << "\n";
		exit(1);
	}

	// Create an OpenGL Graphics Context
	gOpenGLContext = SDL_GL_CreateContext(gGraphicsApplicationWindow);
	if (gOpenGLContext == nullptr)
	{
		std::cout << "OpenGL context could not be created! SDL Error: " << SDL_GetError() << "\n";
		exit(1);
	}

	// Initialize GLAD Library
	if (!gladLoadGLLoader(SDL_GL_GetProcAddress))
	{
		std::cout << "glad did not initialize" << std::endl;
		exit(1);
	}
}

/**
 * Setup your geometry during the vertex specification step
 *
 * @return void
 */
void VertexSpecification(ChunkManager &chunkManager)
{
	// Get the vertex data for the model
	gVoxelVertexData = chunkManager.GetVertexData();
	gVoxelIndexBufferData = chunkManager.GetIndexData();

	// Store the sun data
	gSunVertexData = sun.GetVertexData();
	GLuint nextSunIndex = 0;
	sun.GetIndexData(gSunIndexBufferData, nextSunIndex);

	// === Setup Cube VAO ===
	glGenVertexArrays(1, &gVoxelVAO);
	glBindVertexArray(gVoxelVAO);

	glGenBuffers(1, &gVoxelVBO);
	glBindBuffer(GL_ARRAY_BUFFER, gVoxelVBO);
	glBufferData(GL_ARRAY_BUFFER, gVoxelVertexData.size() * sizeof(GLfloat),
				 gVoxelVertexData.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &gVoxelIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVoxelIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, gVoxelIndexBufferData.size() * sizeof(GLuint),
				 gVoxelIndexBufferData.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); // Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 8, (void *)0);

	glEnableVertexAttribArray(1); // Texture coordinates
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 8, (GLvoid *)(sizeof(GL_FLOAT) * 3));

	glEnableVertexAttribArray(2); // Normals
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 8, (GLvoid *)(sizeof(GL_FLOAT) * 5));

	glBindVertexArray(0); // Unbind cube VAO

	// === Setup Sun VAO ===
	glGenVertexArrays(1, &gSunVAO);
	glBindVertexArray(gSunVAO);

	glGenBuffers(1, &gSunVBO);
	glBindBuffer(GL_ARRAY_BUFFER, gSunVBO);
	glBufferData(GL_ARRAY_BUFFER, gSunVertexData.size() * sizeof(GLfloat),
				 gSunVertexData.data(), GL_STATIC_DRAW);

	GLuint sunIBO;
	glGenBuffers(1, &sunIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sunIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, gSunIndexBufferData.size() * sizeof(GLuint),
				 gSunIndexBufferData.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); // Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 8, (void *)0);

	glEnableVertexAttribArray(1); // Texture coordinates
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 8, (GLvoid *)(sizeof(GL_FLOAT) * 3));

	glEnableVertexAttribArray(2); // Normals
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 8, (GLvoid *)(sizeof(GL_FLOAT) * 5));

	glBindVertexArray(0); // Unbind sun VAO
}

void LoadTexture()
{
	// Generate the texture
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// load and generate the texture
	int width, height, nrChannels;
	unsigned char *data = stbi_load("./assets/minecraft_texture_atlas.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}

void UpdateSunPosition(float deltaTime, Shader &voxelShader, Shader &sunShader)
{
	float radius = 30.0f;			 // Radius of the circle
	float speed = 0.5f;				 // Angular speed (radians per second)
	float oscillationHeight = 40.0f; // Maximum height of oscillation
	float oscillationSpeed = 1.0f;	 // Speed of vertical oscillation (radians per second)

	// Increment the angle based on speed and elapsed time
	gSunAngle += speed * deltaTime;

	// Keep the angle within [0, 2π] to avoid overflow
	if (gSunAngle > glm::two_pi<float>())
	{
		gSunAngle -= glm::two_pi<float>();
	}

	// Compute new position on the circle (XZ-plane)
	float x = radius * cos(gSunAngle);
	float z = radius * sin(gSunAngle);

	// Calculate the vertical position using sine for oscillation
	float y = oscillationHeight * sin(oscillationSpeed * gSunAngle); // Vary y based on angle

	// Create new position vector
	glm::vec3 newPosition(x, y, z);

	// 1. Update the sun's position using the setter
	sun.SetPosition(newPosition);

	// 2. Pass the model matrix to the sun's visual representation
	glm::mat4 model = glm::translate(glm::mat4(1.0f), newPosition);
	sunShader.use();
	sunShader.setMat4("model", model);
}
/**
 * PreDraw
 * Typically we will use this for setting some sort of 'state'
 * Note: some of the calls may take place at different stages (post-processing) of the
 * 		 pipeline.
 * @return void
 */
void PreDraw(Shader &voxelShader, Shader &sunShader, float deltaTime)
{
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, gScreenWidth, gScreenHeight);
	glClearColor(135.0f / 255.0f, 206.0f / 255.0f, 235.0f / 255.0f, 1.0f); // Sky Blue
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	// Model, view, projection setup
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, g_uOffset));
	model = glm::rotate(model, glm::radians(g_uRotate), glm::vec3(0.0f, 1.0f, 0.0f));

	float farPlane = 100.0f;
	glm::mat4 projection = glm::perspective(glm::radians(45.0f),
											(float)gScreenWidth / (float)gScreenHeight, 0.1f, farPlane);
	glm::mat4 view = gCamera.GetViewMatrix();

	// Update shaders and sun position
	UpdateSunPosition(deltaTime, voxelShader, sunShader);

	// Setup voxel shader
	voxelShader.use();
	voxelShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
	voxelShader.setVec3("lightPos", sun.GetPosition());
	voxelShader.setVec3("viewPos",
						gCamera.GetEyeXPosition(), gCamera.GetEyeYPosition(), gCamera.GetEyeZPosition());
	voxelShader.setMat4("projection", projection);
	voxelShader.setMat4("view", view);
	voxelShader.setMat4("model", model);

	// Set up sun shader
	sunShader.use();
	sunShader.setMat4("projection", projection);
	sunShader.setMat4("view", view);
}

/**
 * Draw
 * The render function gets called once per loop.
 * Typically this includes 'glDraw' related calls, and the relevant setup of buffers
 * for those calls.
 *
 * @return void
 */
void Draw(Shader &voxelShader, Shader &sunShader)
{
	// Set polygon mode
	if (drawType)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	// Draw voxel object
	voxelShader.use();
	glBindVertexArray(gVoxelVAO);
	glBindBuffer(GL_ARRAY_BUFFER, gVoxelVBO);
	int elements = gVoxelIndexBufferData.size();
	glDrawElements(GL_TRIANGLES, elements, GL_UNSIGNED_INT, nullptr);

	// Draw the sun
	sunShader.use();
	glBindVertexArray(gSunVAO);
	glBindBuffer(GL_ARRAY_BUFFER, gSunVBO);
	int sunElements = gSunIndexBufferData.size();
	glDrawElements(GL_TRIANGLES, sunElements, GL_UNSIGNED_INT, nullptr);

	// Reset the graphics pipeline (optional)
	glUseProgram(0);
}

/**
 * Helper Function to get OpenGL Version Information
 *
 * @return void
 */
void getOpenGLVersionInfo()
{
	std::cout << "Vendor: " << glGetString(GL_VENDOR) << "\n";
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << "\n";
	std::cout << "Version: " << glGetString(GL_VERSION) << "\n";
	std::cout << "Shading language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";
}

/**
 * Function called in the main application loop to handle user input
 *
 * @return void
 */
void Input(ChunkManager &chunkManager)
{
	// Event handler that handles various events in SDL
	// that are related to input and output
	SDL_Event e;

	// Handle events on queue
	while (SDL_PollEvent(&e) != 0)
	{
		// If users posts an event to quit
		// An example is hitting the "x" in the corner of the window.
		if (e.type == SDL_QUIT)
		{
			std::cout << "Goodbye! (Leaving MainApplicationLoop())" << std::endl;
			gQuit = true;
		}
		else if (e.type == SDL_KEYDOWN)
		{
			switch (e.key.keysym.sym)
			{
			case SDLK_t:
				drawType = !drawType;
				break;
			case SDLK_q:
				gQuit = true;
			default:
				// Unused key pressed
				break;
			}
		}
		else if (e.type == SDL_MOUSEWHEEL)
		{
			int scrollY = e.wheel.y;
			gCamera.MoveUp(scrollY * 0.1f);
		}
		else if (e.type == SDL_MOUSEBUTTONDOWN)
		{
			int cameraX = floor(gCamera.GetEyeXPosition());
			int cameraY = floor(gCamera.GetEyeYPosition());
			int cameraZ = floor(gCamera.GetEyeZPosition());

			std::cout << "Camera position " << cameraX << ", " << cameraY << ", " << cameraZ << std::endl;

			chunkManager.UpdateChunks(cameraX, cameraY, cameraZ);
			VertexSpecification(chunkManager);
		}

		// Retrieve keyboard state
		const Uint8 *state = SDL_GetKeyboardState(NULL);
		if (state[SDL_SCANCODE_UP])
		{
			g_uOffset += 0.01f;
		}
		if (state[SDL_SCANCODE_DOWN])
		{
			g_uOffset -= 0.01f;
		}
		if (state[SDL_SCANCODE_LEFT])
		{
			g_uRotate -= 1.0f;
		}
		if (state[SDL_SCANCODE_RIGHT])
		{
			g_uRotate += 1.0f;
		}

		// Camera
		// Update our position of the camera
		if (state[SDL_SCANCODE_W])
		{
			gCamera.MoveForward(0.2f);
		}
		if (state[SDL_SCANCODE_S])
		{
			gCamera.MoveBackward(0.2f);
		}
		if (state[SDL_SCANCODE_A])
		{
			gCamera.MoveLeft(0.2f);
		}
		if (state[SDL_SCANCODE_D])
		{
			gCamera.MoveRight(0.2f);
		}

		// Update the mouse look of the camera
		// Center the mouse in the window
		int mouseX, mouseY;
		SDL_GetGlobalMouseState(&mouseX, &mouseY);
		gCamera.MouseLook(mouseX, mouseY);
	}
}

/**
 * Main Application Loop
 * This is an infinite loop
 *
 * @return void
 */
void MainLoop(ChunkManager &chunkManager, Shader &voxelShader, Shader &sunShader)
{
	SDL_WarpMouseInWindow(gGraphicsApplicationWindow, 640 / 2, 480 / 2);
	// Store the previous time
	uint64_t lastTime = SDL_GetPerformanceCounter();
	float deltaTime = 0.0f;

	// While application is running
	while (!gQuit)
	{
		// Calculate delta time
		uint64_t currentTime = SDL_GetPerformanceCounter();
		deltaTime = (float)(currentTime - lastTime) / SDL_GetPerformanceFrequency();
		lastTime = currentTime;
		// Handle Input
		Input(chunkManager);
		// Setup anything (i.e. OpenGL State) that needs to take
		// place before draw calls
		PreDraw(voxelShader, sunShader, deltaTime);
		// Draw Calls in OpenGL
		Draw(voxelShader, sunShader);
		// Update screen of our specified window
		SDL_GL_SwapWindow(gGraphicsApplicationWindow);
	}
}

/**
 * The last function called in the program
 * This functions responsibility is to destroy any global
 * objects in which we have create dmemory.
 *
 * @return void
 */
void CleanUp()
{
	// Destroy our SDL2 Window
	SDL_DestroyWindow(gGraphicsApplicationWindow);
	gGraphicsApplicationWindow = nullptr;

	// Delete our OpenGL Objects
	glDeleteBuffers(1, &gVoxelVBO);
	glDeleteVertexArrays(1, &gVoxelVAO);

	// Delete our Sun OpenGL Objects
	glDeleteBuffers(1, &gSunVBO);
	glDeleteVertexArrays(1, &gSunVAO);

	// Delete our Graphics pipeline

	// Quit SDL subsystems
	SDL_Quit();
}

/**
 * The main entry point into our C++ programs.
 *
 * @return program status
 */
int main(int argc, char **argv)
{
	ChunkManager chunkManager = ChunkManager();

	// 2. Setup the graphics program
	InitializeProgram();

	// 3. Setup our geometry
	VertexSpecification(chunkManager);

	// 4. Load the texture
	LoadTexture();

	// 5. Create our graphics pipeline
	Shader voxelShader("./shaders/voxel.vs", "./shaders/voxel.fs");
	Shader sunShader("./shaders/sun.vs", "./shaders/sun.fs");

	// 5. Call the main application loop
	MainLoop(chunkManager, voxelShader, sunShader);

	// 6. Call the cleanup function when our program terminates
	CleanUp();

	return 0;
}
