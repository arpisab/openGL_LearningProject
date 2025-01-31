///////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	GLSL Linker Errors a
//	Shows how to get linker errors from OpenGL after compiling and 
//  linking shader objects to a shader program.
//


#include <GL\glew.h>
#include <iostream>
#include <MeGlWindow.h>
using namespace std;


extern const char* vertexShaderCode;
extern const char* fragmentShaderCode;

void sendDataToOpenGL()
{
	GLfloat verts[] =
	{
		+0.0f, +1.0f,
		+1.0f, +0.0f, +0.0f,
		-1.0f, -1.0f,
		+0.0f, +1.0f, +0.0f,
		+1.0f, -1.0f,
		+0.0f, +0.0f, +1.0f,

	};
	GLuint vertexBufferID;
	glGenBuffers(1, &vertexBufferID); // generates the Buffer Objects and stores the ID that reprensents the BO to myBufferID. Thinks about a penGL pointer
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID); // creates a binding point for the bufferID
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts),
		verts, GL_STATIC_DRAW);  // send the data that is bound to the GL_ARRAY_BUFFER binding point, down to openGL

	glEnableVertexAttribArray(0); // enable the data that we copied down to buffer (verts array)to go to graphic process pipeline	
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0); // here we describe the data (verts array) to openGL

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (char*)(sizeof(float) * 2));


	GLushort indices[] = { 0, 1, 2, }; // GLushort saves more memory
	GLuint indexBufferID;
	glGenBuffers(1, &indexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),
		indices, GL_STATIC_DRAW);
}

bool chekShaderStatus(GLuint shaderID)
{
	/// Shader compiler checker - version 01
	GLint compileStatus;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileStatus);  //GLGetShaderiv - iv stands for integer vector
	if (compileStatus != GL_TRUE)
	{
		GLint infoLogLenght;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLenght);
		GLchar* buffer = new GLchar[infoLogLenght];

		GLsizei bufferSize;
		glGetShaderInfoLog(shaderID, infoLogLenght, &bufferSize, buffer); // glGetShaderInfoLog will write the error message out of the buffer
		cout << buffer << endl;

		delete[] buffer;
		return false;
	}
	return true;
	/// - end
}

bool chekProgramStatus(GLuint programID)
{
	/// Shader linker checker - version 01
	GLint linkStatus;
	glGetProgramiv(programID, GL_LINK_STATUS, &linkStatus);  //GLGetShaderiv - iv stands for integer vector
	if (linkStatus != GL_TRUE)
	{
		GLint infoLogLenght;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLenght);
		GLchar* buffer = new GLchar[infoLogLenght];

		GLsizei bufferSize;
		glGetProgramInfoLog(programID, infoLogLenght, &bufferSize, buffer); // glGetShaderInfoLog will write the error message out of the buffer
		cout << buffer << endl;

		delete[] buffer;
		return false;
	}
	return true;
	/// - end
}

void installShaders()
{
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	const char* adapter[1];
	adapter[0] = vertexShaderCode;
	glShaderSource(vertexShaderID, 1, adapter, 0);  // here we send the shader down to the graphic card 
	adapter[0] = fragmentShaderCode;
	glShaderSource(fragmentShaderID, 1, adapter, 0);

	glCompileShader(vertexShaderID);   // and here we ask opneGL to compile the shaders
	glCompileShader(fragmentShaderID);

	if (!chekShaderStatus(vertexShaderID) || !chekShaderStatus(fragmentShaderID))
		return;

	GLuint programID = glCreateProgram();  // create the programID that will link the shaders

	//glAttachShader(programID, vertexShaderID);    // attach the shader to the programID
	//glAttachShader(programID, fragmentShaderID);

	glLinkProgram(programID); // link the programID

	if (!chekProgramStatus(programID))
		return;

	glUseProgram(programID); // link the programID


}

void MeGlWindow::initializeGL()
{
	glewInit();
	sendDataToOpenGL();
	installShaders();

}

void MeGlWindow::paintGL()
{
	glViewport(0, 0, width(), height()); // the triangle reshape according with the size of the window

	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, 0); // what do we want to render: Triangles; 
	// how many verteces do we whant to draw? : 6;
	// What data type are they? : GLushort (we're passing the indices buffer now);
	// Is there any offset? : no (0);
}