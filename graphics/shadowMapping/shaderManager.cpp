#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <map>

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

    GLuint mActiveProgramId;
    string mActiveProgramName;
    map<string, GLuint> mVertexShaders; 
    map<string, GLuint> mFragmentShaders; 
    map<string, GLuint> mPrograms; 

    ShaderManager();
    
    ~ShaderManager();
    
    int addVertexShader(string shaderPath, string name);
    
    int addFragmentShader(string shaderPath, string name);
    
    int linkShaderProgram(GLuint vShaderId, GLuint fShaderId, GLuint gShaderId = 0);

    int attachProgram(string shaderName, string programName);
    
    int buildProgram(string programName); 
    
    int useProgram(string programeName);

private:
    
    int compileShader(string shaderPath, int type);
    void checkState(GLuint objectId, std::string type);
    
};

/* static */ string ShaderManager::vertexShaderType      = "Vertex";
/* static */ string ShaderManager::FragmentShaderType    = "Fragment";
/* static */ string ShaderManager::GeometryShaderType    = "Geometry";
/* static */ string ShaderManager::ShaderProgramType     = "Program";

// Default Constructor
ShaderManager::ShaderManager()
: mActiveProgramId(0)
, mActiveProgramName()
{ }

// Default Destructor 
ShaderManager::~ShaderManager()
{
    // Implement destructor here.
    //glDeleteShader();
}

int ShaderManager::attachProgram(string shaderName, string programName)
{
    GLuint  shaderId, programId;  
    glAttachShader(programId, shaderId);
}

int ShaderManager::buildProgram(string programName) 
{
    GLuint  programId = mPrograms[programName];  
    glLinkProgram(programId);
    mActiveProgramId   = programId;
    mActiveProgramName = programName;
}

int ShaderManager::useProgram(string programName)
{
    GLuint  programId = mPrograms[programName];  
    glUseProgram(programId);
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
                          << "- Detail Info:"
                          << "\n -- --------------------------------------------------- --\n"
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
				glGetShaderInfoLog(objectId, 1024, NULL, infoLog);
                stringstream logStream;
                logStream << "- ERROR: Shader Compile Error. Shader Type: " << type << "\n"
                          << "- Detail Info:"
                          << "\n -- --------------------------------------------------- --\n"
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
            shaderFile.open(shaderPath.c_str());
            stringstream shaderStream;
            shaderStream << shaderFile.rdbuf();
            shaderFile.close();
            sourceCode = shaderStream.str();
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
