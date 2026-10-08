#include "imgui.h"
#include "lab_work_3.hpp"
#include "utils/read_file.hpp"
#include <iostream>
#include "glm/gtc/type_ptr.hpp"
#include <vector>
#include "utils/random.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace M3D_ISICG
{
	const std::string LabWork3::_shaderFolder = "src/lab_works/lab_work_3/shaders/";

	LabWork3::~LabWork3() { 
		glDeleteProgram( _program );
		glDeleteBuffers( 1, &_cube.VBO );
		glDeleteBuffers( 1, &_cube.ColorsVBO );
		glDisableVertexArrayAttrib( _cube.VAO, 0 );
		glDeleteVertexArrays( 1, &_cube.VAO );
	}

	void LabWork3::render() { glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );
		_updateProjectionMatrix();
		_updateViewMatrix();
		
		glProgramUniformMatrix4fv( _program, _cubeTransformLocation, 1, false, glm::value_ptr( _cube.transform ) );
		
		glBindVertexArray( _cube.VAO );
		glDrawElements( GL_TRIANGLES, _cube.indicesSommets.size(), GL_UNSIGNED_INT, 0 );
		glBindVertexArray( 0 );

	}

	bool LabWork3::init() {
		std::cout << "Initializing lab work 3..." << std::endl;

		glClearColor( _bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w );
		glEnable( GL_DEPTH_TEST );

		// Compile shaders.
		const std::string vertexShaderStr	= readFile( _shaderFolder + "lw1.vert" );
		const std::string fragmentShaderStr = readFile( _shaderFolder + "lw1.frag" );
		GLuint			  vertexShader		= glCreateShader( GL_VERTEX_SHADER );
		GLuint			  fragmentShader	= glCreateShader( GL_FRAGMENT_SHADER );
		const GLchar *	  vSrc				= vertexShaderStr.c_str();
		const GLchar *	  fSrc				= fragmentShaderStr.c_str();
		glShaderSource( vertexShader, 1, &vSrc, NULL );
		glShaderSource( fragmentShader, 1, &fSrc, NULL );
		
		// Check if shader compilation is ok .
		GLint compiled;

		glCompileShader( vertexShader );
		glGetShaderiv( vertexShader, GL_COMPILE_STATUS, &compiled );
		if ( !compiled ) {
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
		if ( !compiled ) {
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

		glUseProgram( _program );

		// Create the cube
		_cube = createCube();
		_cubeTransformLocation = glGetUniformLocation( _program, "uTransform" );
		_initBuffers();

		std::cout << "Done!" << std::endl;

		// Camera
		_initCamera();

		return true; 
	}

	void LabWork3::displayUI() {
		ImGui::Begin( "Settings lab work 3" );
		
		if ( ImGui::SliderFloat( "FOV", &_fovy, 0.f, 180.f ) )
		{
			_camera.setFovy( _fovy );
		}

		ImGui::End();
	}

	void LabWork3::handleEvents( const SDL_Event & p_event )
	{
		if ( p_event.type == SDL_KEYDOWN )
		{
			switch ( p_event.key.keysym.scancode )
			{
			case SDL_SCANCODE_W: // Front
				_camera.moveFront( _cameraSpeed );
				_updateViewMatrix();
				break;
			case SDL_SCANCODE_S: // Back
				_camera.moveFront( -_cameraSpeed );
				_updateViewMatrix();
				break;
			case SDL_SCANCODE_A: // Left
				_camera.moveRight( -_cameraSpeed );
				_updateViewMatrix();
				break;
			case SDL_SCANCODE_D: // Right
				_camera.moveRight( _cameraSpeed );
				_updateViewMatrix();
				break;
			case SDL_SCANCODE_R: // Up
				_camera.moveUp( _cameraSpeed );
				_updateViewMatrix();
				break;
			case SDL_SCANCODE_F: // Bottom
				_camera.moveUp( -_cameraSpeed );
				_updateViewMatrix();
				break;
			default: break;
			}
		}

		// Rotate when left click + motion (if not on Imgui widget).
		if ( p_event.type == SDL_MOUSEMOTION && p_event.motion.state & SDL_BUTTON_LMASK
			 && !ImGui::GetIO().WantCaptureMouse )
		{
			_camera.rotate( p_event.motion.xrel * _cameraSensitivity, p_event.motion.yrel * _cameraSensitivity );
			_updateViewMatrix();
		}
	}

	void LabWork3::animate( const float p_deltaTime ) { 
		_cube.transform = glm::rotate( _cube.transform, p_deltaTime, glm::vec3( 0, 1, 1 ) );
	}

	void LabWork3::_initBuffers() {
		// Initialize and load the VBO and VAO.
		glCreateBuffers( 1, &_cube.VBO );
		glNamedBufferData( _cube.VBO, _cube.posSommets.size() * sizeof( Vec3f ), _cube.posSommets.data(), GL_STATIC_DRAW );

		glCreateVertexArrays( 1, &_cube.VAO );
		glEnableVertexArrayAttrib( _cube.VAO, 0 );
		glVertexArrayAttribFormat( _cube.VAO, 0, 3, GL_FLOAT, GL_FALSE, 0 );
		glVertexArrayVertexBuffer( _cube.VAO, 0, _cube.VBO, 0, sizeof( Vec3f ) );
		glVertexArrayAttribBinding( _cube.VAO, 0, 0 );

		// Initialize EBO
		glCreateBuffers( 1, &_cube.EBO );
		glNamedBufferData(_cube.EBO, _cube.indicesSommets.size() * sizeof( GLuint ), _cube.indicesSommets.data(), GL_STATIC_DRAW );
		glVertexArrayElementBuffer( _cube.VAO, _cube.EBO );

		// Initialise Colors VBO
		glCreateBuffers( 1, &_cube.ColorsVBO );
		glNamedBufferData( _cube.ColorsVBO, _cube.colSommets.size() * sizeof( Vec3f ), _cube.colSommets.data(), GL_STATIC_DRAW );

		glEnableVertexArrayAttrib( _cube.VAO, 1 );
		glVertexArrayAttribFormat( _cube.VAO, 1, 3, GL_FLOAT, GL_FALSE, 1 );
		glVertexArrayVertexBuffer( _cube.VAO, 1, _cube.ColorsVBO, 0, sizeof( Vec3f ) );
		glVertexArrayAttribBinding( _cube.VAO, 1, 1 );
	}

	void LabWork3::_initCamera() {
		_camera					  = Camera();
		_camera.setPosition( Vec3f( 0, 1, 3 ) );
		_camera.setScreenSize(getWindowWidth(), getWindowHeight());
		_camera.setFovy( _fovy );

		_viewMatrixLocation		  = glGetUniformLocation( _program, "uViewMatrix" );
		_projectionMatrixLocation = glGetUniformLocation( _program, "uProjectionMatrix" );
	}

	LabWork3::Mesh LabWork3::createCube() {
		Mesh cube = {};
		cube.posSommets = { Vec3f( -.5, -.5, -.5 ), Vec3f( .5, -.5, -.5 ), Vec3f( -.5, -.5, .5 ), Vec3f( .5, -.5, .5 ),
							Vec3f( -.5, .5, -.5 ),	Vec3f( .5, .5, -.5 ),  Vec3f( -.5, .5, .5 ),  Vec3f( .5, .5, .5 ) 
		};

		for ( int _ = 0; _ != 8; _++ )
			cube.colSommets.push_back( getRandomVec3f() );

		cube.indicesSommets = {
			0, 1, 2, 3, 2, 1,
			4, 5, 6, 7, 6, 5,
			0, 1, 4, 1, 4, 5,
			1, 3, 5, 5, 3, 7,
			3, 2, 6, 6, 3, 7,
			4, 0, 2, 4, 2, 6,
		};

		cube.transform = glm::mat4(1.0f);
		cube.transform = glm::scale( cube.transform, glm::vec3( 0.8f, 0.8f, 0.8f ) );

		return cube;
	}

	void LabWork3::_updateViewMatrix() { 
		glProgramUniformMatrix4fv( _program, _viewMatrixLocation, 1, false, glm::value_ptr(_camera.getViewMatrix()));
	}

	void LabWork3::_updateProjectionMatrix() { 
		glProgramUniformMatrix4fv( _program, _projectionMatrixLocation, 1, false, glm::value_ptr( _camera.getProjectionMatrix() ) );
	}

} // namespace M3D_ISICG