
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
#include "ChunkManager.hpp"
#include "Camera.hpp"

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
GLuint gVertexArrayObject = 0;
// Vertex Buffer Object (VBO)
// Vertex Buffer Objects store information relating to vertices (e.g. positions, normals, textures)
// VBOs are our mechanism for arranging geometry on the GPU.
GLuint gVertexBufferObject = 0;
GLuint gIndexBufferObject = 0;

// Solid Fill = true, Wireframe = false
bool drawType = true;
// Model to draw
int modelId = 0;
// True if a model should be rendered to screen
bool loadedModel = false;

// Shaders
// Here we setup two shaders, a vertex shader and a fragment shader.
// At a minimum, every Modern OpenGL program needs a vertex and a fragment
// shader.
float g_uOffset = -2.0f;
float g_uRotate = 0.0f;

// Camera
Camera gCamera;

// ^^^^^^^^^^^^^^^^^^^^^^^^ Globals ^^^^^^^^^^^^^^^^^^^^^^^^^^^

// vvvvvvvvvvvvvvvvvvv Error Handling Routines vvvvvvvvvvvvvvv
static void GLClearAllErrors()
{
	while (glGetError() != GL_NO_ERROR)
	{
	}
}

// Returns true if we have an error
static bool GLCheckErrorStatus(const char *function, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "OpenGL Error:" << error
				  << "\tLine: " << line
				  << "\tfunction: " << function << std::endl;
		return true;
	}
	return false;
}

#define GLCheck(x)      \
	GLClearAllErrors(); \
	x;                  \
	GLCheckErrorStatus(#x, __LINE__);
// ^^^^^^^^^^^^^^^^^^^ Error Handling Routines ^^^^^^^^^^^^^^^

/**
 * LoadShaderAsString takes a filepath as an argument and will read line by line a file and return a string that is meant to be compiled at runtime for a vertex, fragment, geometry, tesselation, or compute shader.
 * e.g.
 *       LoadShaderAsString("./shaders/filepath");
 *
 * @param filename Path to the shader file
 * @return Entire file stored as a single string
 */
std::string LoadShaderAsString(const std::string &filename)
{
	// Resulting shader program loaded as a single string
	std::string result = "";

	std::string line = "";
	std::ifstream myFile(filename.c_str());

	if (myFile.is_open())
	{
		while (std::getline(myFile, line))
		{
			result += line + '\n';
		}
		myFile.close();
	}

	return result;
}

/**
 * CompileShader will compile any valid vertex, fragment, geometry, tesselation, or compute shader.
 * e.g.
 *	    Compile a vertex shader: 	CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
 *       Compile a fragment shader: 	CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
 *
 * @param type We use the 'type' field to determine which shader we are going to compile.
 * @param source : The shader source code.
 * @return id of the shaderObject
 */
GLuint CompileShader(GLuint type, const std::string &source)
{
	// Compile our shaders
	GLuint shaderObject;

	// Based on the type passed in, we create a shader object specifically for that
	// type.
	if (type == GL_VERTEX_SHADER)
	{
		shaderObject = glCreateShader(GL_VERTEX_SHADER);
	}
	else if (type == GL_FRAGMENT_SHADER)
	{
		shaderObject = glCreateShader(GL_FRAGMENT_SHADER);
	}

	const char *src = source.c_str();
	// The source of our shader
	glShaderSource(shaderObject, 1, &src, nullptr);
	// Now compile our shader
	glCompileShader(shaderObject);

	// Retrieve the result of our compilation
	int result;
	// Our goal with glGetShaderiv is to retrieve the compilation status
	glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &length);
		char *errorMessages = new char[length]; // Could also use alloca here.
		glGetShaderInfoLog(shaderObject, length, &length, errorMessages);

		if (type == GL_VERTEX_SHADER)
		{
			std::cout << "ERROR: GL_VERTEX_SHADER compilation failed!\n"
					  << errorMessages << "\n";
		}
		else if (type == GL_FRAGMENT_SHADER)
		{
			std::cout << "ERROR: GL_FRAGMENT_SHADER compilation failed!\n"
					  << errorMessages << "\n";
		}
		// Reclaim our memory
		delete[] errorMessages;

		// Delete our broken shader
		glDeleteShader(shaderObject);

		return 0;
	}

	return shaderObject;
}

/**
 * Creates a graphics program object (i.e. graphics pipeline) with a Vertex Shader and a Fragment Shader
 *
 * @param vertexShaderSource Vertex source code as a string
 * @param fragmentShaderSource Fragment shader source code as a string
 * @return id of the program Object
 */
GLuint CreateShaderProgram(const std::string &vertexShaderSource, const std::string &fragmentShaderSource)
{
	// Create a new program object
	GLuint programObject = glCreateProgram();

	// Compile our shaders
	GLuint myVertexShader = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
	GLuint myFragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

	// Link our two shader programs together.
	// Consider this the equivalent of taking two .cpp files, and linking them into
	// one executable file.
	glAttachShader(programObject, myVertexShader);
	glAttachShader(programObject, myFragmentShader);
	glLinkProgram(programObject);

	// Validate our program
	glValidateProgram(programObject);

	// Once our final program Object has been created, we can
	// detach and then delete our individual shaders.
	glDetachShader(programObject, myVertexShader);
	glDetachShader(programObject, myFragmentShader);
	// Delete the individual shaders once we are done
	glDeleteShader(myVertexShader);
	glDeleteShader(myFragmentShader);

	return programObject;
}

/**
 * Create the graphics pipeline
 *
 * @return void
 */
void CreateGraphicsPipeline()
{
	std::string vertexShaderSource = LoadShaderAsString("./shaders/vert.glsl");
	std::string fragmentShaderSource = LoadShaderAsString("./shaders/frag.glsl");

	gGraphicsPipelineShaderProgram = CreateShaderProgram(vertexShaderSource, fragmentShaderSource);
}

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
	std::vector<GLfloat> vertexData;

	// Load the diffuse texture
	// gTexture.LoadTexture(chunkManager.at(modelId).get_diffuse_texture());

	// Get the vertex data for the model
	vertexData = chunkManager.get_vertex_data();

	//std::cout << "vertex data size " << vertexData.size() << std::endl;
	// Vertex Arrays Object (VAO) Setup
	glGenVertexArrays(1, &gVertexArrayObject);
	// We bind (i.e. select) to the Vertex Array Object (VAO) that we want to work withn.
	glBindVertexArray(gVertexArrayObject);

	// Vertex Buffer Object (VBO) creation
	// Create a new vertex buffer object
	glGenBuffers(1, &gVertexBufferObject);

	// Bind is equivalent to 'selecting the active buffer object' that we want to
	// work with in OpenGL.
	glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObject);
	// Now, in our currently binded buffer, we populate the data from our
	// 'vertexPositions' (which is on the CPU), onto a buffer that will live
	// on the GPU.
	glBufferData(GL_ARRAY_BUFFER,					  // Kind of buffer we are working with
													  // (e.g. GL_ARRAY_BUFFER or GL_ELEMENT_ARRAY_BUFFER)
				 vertexData.size() * sizeof(GLfloat), // Size of data in bytes
				 vertexData.data(),					  // Raw array of data
				 GL_STATIC_DRAW);					  // How we intend to use the data

	// Index buffer data for a quad
	std::vector<GLuint> indexBufferData;

	indexBufferData = chunkManager.get_index_data();
	//std::cout << indexBufferData.size() << std::endl;

	glGenBuffers(1, &gIndexBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIndexBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBufferData.size() * sizeof(GLuint),
				 indexBufferData.data(), GL_STATIC_DRAW);

	// For our Given Vertex Array Object, we need to tell OpenGL
	// 'how' the information in our buffer will be used.
	glEnableVertexAttribArray(0);
	// For the specific attribute in our vertex specification, we use
	// 'glVertexAttribPointer' to figure out how we are going to move
	// through the data.
	glVertexAttribPointer(0, // Attribute 0 corresponds to the enabled glEnableVertexAttribArray
							 // In the future, you'll see in our vertex shader this also correspond
							 // to (layout=0) which selects these attributes.
						  3,					// The number of components (e.g. x,y,z = 3 components)
						  GL_FLOAT,				// Type
						  GL_FALSE,				// Is the data normalized
						  sizeof(GL_FLOAT) * 6, // Stride
						  (void *)0				// Offset
	);

	// Now linking up the attributes in our VAO
	// Color information
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,
						  3, // r,g,b
						  GL_FLOAT,
						  GL_FALSE,
						  sizeof(GL_FLOAT) * 6,
						  (GLvoid *)(sizeof(GL_FLOAT) * 3));

	// Unbind our currently bound Vertex Array Object
	glBindVertexArray(0);

	// Disable any attributes we opened in our Vertex Attribute Arrray,
	// as we do not want to leave them open.
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

/**
 * PreDraw
 * Typically we will use this for setting some sort of 'state'
 * Note: some of the calls may take place at different stages (post-processing) of the
 * 		 pipeline.
 * @return void
 */
void PreDraw()
{
	glEnable(GL_DEPTH_TEST);
	// glEnable(GL_CULL_FACE);
	// glCullFace(GL_FRONT);

	// Initialize clear color
	// This is the background of the screen.
	glViewport(0, 0, gScreenWidth, gScreenHeight);
	glClearColor(0.529f, 0.808f, 0.922f, 1.0f);

	// Clear color buffer and Depth Buffer
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	// Use our shader
	glUseProgram(gGraphicsPipelineShaderProgram);

	// Model transformation by translating our object into world spaceu
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, g_uOffset));

	// Update our model matrix by applying a rotation after our translation
	model = glm::rotate(model, glm::radians(g_uRotate), glm::vec3(0.0f, 1.0f, 0.0f));

	// Retrieve our location of our Model Matrix
	GLint u_ModelMatrixLocation = glGetUniformLocation(gGraphicsPipelineShaderProgram, "u_ModelMatrix");
	if (u_ModelMatrixLocation >= 0)
	{
		glUniformMatrix4fv(u_ModelMatrixLocation, 1, GL_FALSE, &model[0][0]);
	}
	else
	{
		std::cout << "Could not find u_ModelMatrix, maybe a mispelling?\n";
		exit(EXIT_FAILURE);
	}

	// Update the View Matrix
	GLint u_ViewMatrixLocation = glGetUniformLocation(gGraphicsPipelineShaderProgram, "u_ViewMatrix");
	if (u_ViewMatrixLocation >= 0)
	{
		glm::mat4 viewMatrix = gCamera.GetViewMatrix();
		glUniformMatrix4fv(u_ViewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
	}
	else
	{
		std::cout << "Could not find u_ModelMatrix, maybe a mispelling?\n";
		exit(EXIT_FAILURE);
	}

	// Projection matrix (in perspective)
	float farPlane = 100.0f; // Set the desired view distance (far plane)
	glm::mat4 perspective = glm::perspective(glm::radians(45.0f),
											 (float)gScreenWidth / (float)gScreenHeight,
											 0.1f,
											 farPlane);

	// Retrieve our location of our perspective matrix uniform
	GLint u_ProjectionLocation = glGetUniformLocation(gGraphicsPipelineShaderProgram, "u_Projection");
	if (u_ProjectionLocation >= 0)
	{
		glUniformMatrix4fv(u_ProjectionLocation, 1, GL_FALSE, &perspective[0][0]);
	}
	else
	{
		std::cout << "Could not find u_Perspective, maybe a mispelling?\n";
		exit(EXIT_FAILURE);
	}

	// Update the light direction uniform
	GLint u_LightDirectionLocation = glGetUniformLocation(gGraphicsPipelineShaderProgram, "u_LightDirection");
	if (u_LightDirectionLocation >= 0)
	{
		glm::vec3 lightDirection(0.0f, -1.0f, 0.0f);
		glUniform3fv(u_LightDirectionLocation, 1, glm::value_ptr(lightDirection));
	}
	else
	{
		std::cout << "Could not find u_LightDirection, maybe a misspelling?\n";
		exit(EXIT_FAILURE);
	}
}

/**
 * Draw
 * The render function gets called once per loop.
 * Typically this includes 'glDraw' related calls, and the relevant setup of buffers
 * for those calls.
 *
 * @return void
 */
void Draw(ChunkManager &chunkManager)
{
	if (drawType)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	// Enable our attributes
	glBindVertexArray(gVertexArrayObject);

	// Select the vertex buffer object we want to enable
	glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObject);

	int elements = chunkManager.get_index_data().size();

	glDrawElements(GL_TRIANGLES, elements, GL_UNSIGNED_INT, nullptr);

	// Stop using our current graphics pipeline
	// Note: This is not necessary if we only have one graphics pipeline.
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
			gCamera.MoveForward(0.1f);
		}
		if (state[SDL_SCANCODE_S])
		{
			gCamera.MoveBackward(0.1f);
		}
		if (state[SDL_SCANCODE_A])
		{
			gCamera.MoveLeft(0.1f);
		}
		if (state[SDL_SCANCODE_D])
		{
			gCamera.MoveRight(0.1f);
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
void MainLoop(ChunkManager &chunkManager)
{
	SDL_WarpMouseInWindow(gGraphicsApplicationWindow, 640 / 2, 480 / 2);

	// While application is running
	while (!gQuit)
	{
		// Handle Input
		Input(chunkManager);
		// Setup anything (i.e. OpenGL State) that needs to take
		// place before draw calls
		PreDraw();
		// Draw Calls in OpenGL
		Draw(chunkManager);
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
	glDeleteBuffers(1, &gVertexBufferObject);
	glDeleteVertexArrays(1, &gVertexArrayObject);

	// Delete our Graphics pipeline
	glDeleteProgram(gGraphicsPipelineShaderProgram);

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

	// 4. Create our graphics pipeline
	// 	- At a minimum, this means the vertex and fragment shader
	CreateGraphicsPipeline();

	// 5. Call the main application loop
	MainLoop(chunkManager);

	// 6. Call the cleanup function when our program terminates
	CleanUp();

	return 0;
}
