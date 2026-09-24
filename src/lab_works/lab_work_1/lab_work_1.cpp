#include "lab_work_1.hpp"
#include "imgui.h"
#include "utils/read_file.hpp"
#include <iostream>

namespace M3D_ISICG
{
	const std::string LabWork1::_shaderFolder = "src/lab_works/lab_work_1/shaders/";
	
	LabWork1::~LabWork1() { 
		glDeleteProgram( _program );
		glDeleteBuffers( 1,&VBO );
		glDisableVertexArrayAttrib( VAO, 0 );
		glDeleteVertexArrays( 1, &VAO );
	}

	bool LabWork1::init()
	{
		std::cout << "Initializing lab work 1..." << std::endl;
		// Set the color used by glClear to clear the color buffer (in render()).
		glClearColor( _bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w );

		// Compile shaders.
		const std::string vertexShaderStr = readFile( _shaderFolder + "lw1.vert" );
		const std::string fragmentShaderStr = readFile( _shaderFolder + "lw1.frag" );
		GLuint			  vertexShader		= glCreateShader( GL_VERTEX_SHADER );
		GLuint			  fragmentShader	= glCreateShader( GL_FRAGMENT_SHADER );
		const GLchar *	  vSrc				= vertexShaderStr.c_str();
		const GLchar *	  fSrc				= fragmentShaderStr.c_str();
		glShaderSource( vertexShader, 1, &vSrc, NULL);
		glShaderSource( fragmentShader, 1, &fSrc, NULL );

		// Check if shader compilation is ok .
		GLint compiled;
		glCompileShader( vertexShader );
		glGetShaderiv( vertexShader, GL_COMPILE_STATUS, &compiled );
		if ( !compiled )
		{
			GLchar log[ 1024 ];
			glGetShaderInfoLog( vertexShader, sizeof( log ), NULL, log );
			glDeleteShader( vertexShader );
			glDeleteShader( fragmentShader );
			std ::cerr << " Error compiling vertex shader : " << log << std ::endl;
			return false;
		}
		compiled = NULL;
		glCompileShader( fragmentShader );
		glGetShaderiv( fragmentShader, GL_COMPILE_STATUS, &compiled );
		if ( !compiled )
		{
			GLchar log[ 1024 ];
			glGetShaderInfoLog( fragmentShader, sizeof( log ), NULL, log );
			glDeleteShader( vertexShader );
			glDeleteShader( fragmentShader );
			std ::cerr << " Error compiling fragment shader : " << log << std ::endl;
			return false;
		}

		// Attach the shaders to the program.
		_program = glCreateProgram();
		glAttachShader( _program, vertexShader );
		glAttachShader( _program, fragmentShader );
		glLinkProgram( _program );
		// Check if link is ok .
		GLint linked;
		glGetProgramiv( _program, GL_LINK_STATUS, &linked );
		if ( !linked )
		{
			GLchar log[ 1024 ];
			glGetProgramInfoLog( _program, sizeof( log ), NULL, log );
			std ::cerr << " Error linking program : " << log << std ::endl;
			return false;
		}
		glDeleteShader( vertexShader );
		glDeleteShader( fragmentShader );

		// Objects data.
		vertices = { Vec2f( -0.5, 0.5 ), Vec2f( 0.5, 0.5 ), Vec2f( 0.5, -0.5 ) };

		// Initialize and load the VBO and VAO.
		glCreateBuffers( 1, &VBO );
		glNamedBufferData( VBO, vertices.size() * sizeof( Vec2f ), vertices.data(), GL_STATIC_DRAW );

		glCreateVertexArrays( 1, &VAO );
		glEnableVertexArrayAttrib( VAO, 0 );
		glVertexArrayAttribFormat( VAO, 0, 2, GL_FLOAT, GL_FALSE, 0 );
		glVertexArrayVertexBuffer( VAO, 0, VBO, 0, sizeof( Vec2f ) );
		glVertexArrayAttribBinding( VAO, 0, 0 );

		glUseProgram( _program );

		std::cout << "Done!" << std::endl;

		return true;
	}

	void LabWork1::animate( const float p_deltaTime ) {}

	void LabWork1::render() { 
		glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );
		glBindVertexArray( VAO );
		glDrawArrays( GL_TRIANGLES, 0, vertices.size() );
		glBindVertexArray( 0 );
	}

	void LabWork1::handleEvents( const SDL_Event & p_event )
	{}

	void LabWork1::displayUI()
	{
		ImGui::Begin( "Settings lab work 1" );
		ImGui::Text( "No setting available!" );
		ImGui::End();
	}

} // namespace M3D_ISICG