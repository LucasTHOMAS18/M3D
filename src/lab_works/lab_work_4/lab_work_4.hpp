#ifndef __LAB_WORK_4_HPP__
#define __LAB_WORK_4_HPP__

#include "GL/gl3w.h"
#include "common/base_lab_work.hpp"
#include "define.hpp"
#include <vector>
#include "common/camera.hpp"
#include "common/models/triangle_mesh_model.hpp"

namespace M3D_ISICG
{
	class LabWork4 : public BaseLabWork
	{
	  public:
		LabWork4() : BaseLabWork() {}
		~LabWork4();

		bool init() override;
		void _initBuffers();
		void _initCamera();

		void animate( const float p_deltaTime ) override;
		void render() override;

		void handleEvents( const SDL_Event & p_event ) override;
		void displayUI() override;

		void _updateMvpMatrix();

	  private:
		Vec4f _bgColor = Vec4f( 0.8f, 0.8f, 0.8f, 1.f );
		
		GLuint					 _program;

		static const std::string _shaderFolder;

		GLuint _mvpMatrixLocation;

		Camera _camera;
		float  _fovy = 60.f;

		float _cameraSpeed		 = 0.1f;
		float _cameraSensitivity = 0.1f;

		TriangleMeshModel bunny;
	};
} // namespace M3D_ISICG

#endif // __LAB_WORK_4_HPP__