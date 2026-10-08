#include "imgui.h"
#include "lab_work_4.hpp"
#include "utils/read_file.hpp"
#include <iostream>
#include "glm/gtc/type_ptr.hpp"
#include <vector>
#include "utils/random.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace M3D_ISICG
{
	const std::string LabWork4::_shaderFolder = "src/lab_works/lab_work_4/shaders/";

	LabWork4::~LabWork4() { 
		glDeleteProgram( _program );
	}

	void LabWork4::render() { glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );
		_updateMvpMatrix();
		bunny.render(_program);
	}

	bool LabWork4::init() {
		std::cout << "Initializing lab work 4..." << std::endl;

		glClearColor( _bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w );
		glEnable( GL_DEPTH_TEST );

		// Compile shaders.
		const std::string vertexShaderStr	= readFile( _shaderFolder + "mesh.vert" );
		const std::string fragmentShaderStr = readFile( _shaderFolder + "mesh.frag" );
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

		// Camera
		_initCamera();

		// Load bunny
		bunny = TriangleMeshModel();
		bunny.load( "bunny.obj", "data/models/bunny.obj" );

		std::cout << "Done!" << std::endl;
		return true; 
	}

	void LabWork4::displayUI() {
		ImGui::Begin( "Settings lab work 3" );
		
		if ( ImGui::SliderFloat( "FOV", &_fovy, 0.f, 180.f ) )
		{
			_camera.setFovy( _fovy );
		}

		ImGui::End();
	}

	void LabWork4::handleEvents( const SDL_Event & p_event )
	{
		if ( p_event.type == SDL_KEYDOWN )
		{
			switch ( p_event.key.keysym.scancode )
			{
			case SDL_SCANCODE_W: // Front
				_camera.moveFront( _cameraSpeed );
				_updateMvpMatrix();
				break;
			case SDL_SCANCODE_S: // Back
				_camera.moveFront( -_cameraSpeed );
				_updateMvpMatrix();
				break;
			case SDL_SCANCODE_A: // Left
				_camera.moveRight( -_cameraSpeed );
				_updateMvpMatrix();
				break;
			case SDL_SCANCODE_D: // Right
				_camera.moveRight( _cameraSpeed );
				_updateMvpMatrix();
				break;
			case SDL_SCANCODE_R: // Up
				_camera.moveUp( _cameraSpeed );
				_updateMvpMatrix();
				break;
			case SDL_SCANCODE_F: // Bottom
				_camera.moveUp( -_cameraSpeed );
				_updateMvpMatrix();
				break;
			default: break;
			}
		}

		// Rotate when left click + motion (if not on Imgui widget).
		if ( p_event.type == SDL_MOUSEMOTION && p_event.motion.state & SDL_BUTTON_LMASK
			 && !ImGui::GetIO().WantCaptureMouse )
		{
			_camera.rotate( p_event.motion.xrel * _cameraSensitivity, p_event.motion.yrel * _cameraSensitivity );
			_updateMvpMatrix();
		}
	}

	void LabWork4::animate( const float p_deltaTime ) { }

	void LabWork4::_initBuffers() {}

	void LabWork4::_initCamera() {
		_camera					  = Camera();
		_camera.setPosition( Vec3f( 0, 1, 3 ) );
		_camera.setScreenSize(getWindowWidth(), getWindowHeight());
		_camera.setFovy( _fovy );

		_mvpMatrixLocation		  = glGetUniformLocation( _program, "uMVPMatrix" );
	}

	void LabWork4::_updateMvpMatrix() { 
		glProgramUniformMatrix4fv( _program, _mvpMatrixLocation, 1, false, glm::value_ptr(_camera.getMvpMatrix()));
	}
} // namespace M3D_ISICG