#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

using std::vector;
using std::map;
using std::string;
using std::ifstream;
using std::stringstream;

class ShaderManager
{

public:

    static string vertexShaderType;
    static string FragmentShaderType;
    static string GeometryShaderType;
    static string ShaderProgramType;

    GLuint Program;
    map<string, GLuint> mVertexShaders; 
    map<string, GLuint> mFragmentShaders; 

    int addVertexShader(string shaderPath);
    int addFragmentShader(string shaderPath);
    int linkShaderProgram(GLuint vShaderId, GLuint fShaderId, GLuint gShaderId = 0);

    // Constructor generates the shader on the fly
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar* geometryPath = nullptr)
    {
        // 1. Retrieve the vertex/fragment source code from filePath
        std::string vertexCode;
        std::string fragmentCode;
        std::string geometryCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        std::ifstream gShaderFile;

        // ensures ifstream objects can throw exceptions:
        vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        gShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

        try 
        {
            // Open files
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;

            // Read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();		

            // close file handlers
            vShaderFile.close();
            fShaderFile.close();

            // Convert stream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();			

			// If geometry shader path is present, also load a geometry shader
			if(geometryPath != nullptr)
			{
                gShaderFile.open(geometryPath);
                std::stringstream gShaderStream;
				gShaderStream << gShaderFile.rdbuf();
				gShaderFile.close();
				geometryCode = gShaderStream.str();
			}
        }
        catch (std::ifstream::failure e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
            std::cout << vertexPath << std::endl;
            std::cout << fragmentPath << std::endl;
        }

        const GLchar *vShaderCode = vertexCode.c_str();
        const GLchar *fShaderCode = fragmentCode.c_str();

        // 2. Compile shaders
        GLuint vertex, fragment;
        GLint  success;
        GLchar infoLog[512];

        // Vertex Shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");

        // Fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
		checkCompileErrors(fragment, "FRAGMENT");

		// If geometry shader is given, compile geometry shader
		GLuint geometry;
		if(geometryPath != nullptr)
		{
			const GLchar * gShaderCode = geometryCode.c_str();
			geometry = glCreateShader(GL_GEOMETRY_SHADER);
			glShaderSource(geometry, 1, &gShaderCode, NULL);
			glCompileShader(geometry);
			checkCompileErrors(geometry, "GEOMETRY");
		}

        // Shader Program
        this->Program = glCreateProgram();
        glAttachShader(this->Program, vertex);
        glAttachShader(this->Program, fragment);
		if(geometryPath != nullptr)
			glAttachShader(this->Program, geometry);
        glLinkProgram(this->Program);
        checkCompileErrors(this->Program, "PROGRAM");

        // Delete the shaders as they're linked into our program now and no longer necessery
        glDeleteShader(vertex);
        glDeleteShader(fragment);
		if(geometryPath != nullptr)
			glDeleteShader(geometry);

    }

    // Uses the current shader
    void Use() { glUseProgram(this->Program); }

    int attachProgram(string shaderName, string programName);
    int buildProgram(string programName); 
    int useProgram(string programeName);

private:
    void checkState(GLuint objectId, std::string type);
};

string ShaderManager::vertexShaderType      = "Vertex";
string ShaderManager::FragmentShaderType    = "Fragment";
string ShaderManager::GeometryShaderType    = "Geometry";
string ShaderManager::ShaderProgramType     = "Program";

int ShaderManager::attachProgram(string shaderName, string programName)
{
}

int ShaderManager::buildProgram(string programName) 
{

}

int ShaderManager::useProgram(string programeName)
{

}

void ShaderManager::checkState(GLuint objectId, std::string type)
{
		GLint success;
		GLchar infoLog[1024] = { 0 };
		if(type == ShaderManager::ShaderProgramType)
		{
			glGetProgramiv(objectId, GL_LINK_STATUS, &success);
			if(!success)
			{
				glGetProgramInfoLog(objectId, 1024, NULL, infoLog);
                stringstream logStream;
                logStream << "- ERROR: Shader Link Error." << "\n"
                          << "- Detail Info";
                          << "\n -- --------------------------------------------------- --\n";
                          << infoLog
                          << "\n -- --------------------------------------------------- --\n";
                std::cout << logStream.str();
			}
		}
		else
		{
			glGetShaderiv(objectId, GL_COMPILE_STATUS, &success);
			if(!success)
			{
				glGetShaderInfoLog(ObjectId, 1024, NULL, infoLog);
                stringstream logStream;
                logStream << "- ERROR: Shader Compile Error. Shader Type: " << type << "\n"
                          << "- Detail Info";
                          << "\n -- --------------------------------------------------- --\n";
                          << infoLog
                          << "\n -- --------------------------------------------------- --\n";
                std::cout << logStream.str();
			}
		}
}

int ShaderManager::compileShader(string shaderPath, int type)
{
        string sourceCode;
        ifstream shaderFile;

        //set exception mask. 
        shaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

        try 
        {
            shaderFile.open(shaderPath);
            stringstream shaderStream;
            shaderStream << shaderFile.rdbuf();
            shaderFile.close();
            sourceCode = ShaderStream.str();
        }
        catch (std::ifstream::failure e)
        {
            stringstream errorStream;
            errorStream << "- ERROR: Read shader File error. file path: "
                        << shaderPath << std::endl;
            std::cout << errorStream.str();
        }

        const GLchar *srcCode = sourceCode.c_str();
        GLuint shaderName = glCreateShader(type);
        glShaderSource(shaderName, 1, &srcCode, NULL);
        glCompileShader(shaderName);
        // need translate GL type into string here.
        checkState(shaderName, "Vertex");

        return shaderName; 
}

int ShaderManager::addVertexShader(string shaderPath, string name)
{
    GLuint shaderId = compileShader(shaderPath, GL_VERTEX_SHADER);
    mVertexShaders[name] = shaderId;
}

int ShaderManager::addFragmentShader(string shaderPath, string name)
{
    GLuint shaderId = compileShader(shaderPath, GL_FRAGMENT_SHADER);
    mFragmentShaders[name] = shaderId;
}

#endif
