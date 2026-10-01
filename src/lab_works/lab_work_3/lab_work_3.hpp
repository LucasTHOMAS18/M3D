#ifndef __LAB_WORK_3_HPP__
#define __LAB_WORK_3_HPP__

#include "GL/gl3w.h"
#include "common/base_lab_work.hpp"
#include "define.hpp"
#include <vector>

namespace M3D_ISICG
{
	class LabWork3 : public BaseLabWork
	{
	  public:
		LabWork3() : BaseLabWork() {}
		~LabWork3();

		bool init() override;
		void _initBuffers();

		void animate( const float p_deltaTime ) override;
		void render() override;

		void handleEvents( const SDL_Event & p_event ) override;
		void displayUI() override;


		struct Mesh
		{
			std::vector<Vec3f> posSommets;
			std::vector<Vec3f> colSommets;

			std::vector<unsigned int> indicesSommets;

			Mat4f transform;

			GLuint VBO;
			GLuint VAO;
			GLuint EBO;
			GLuint ColorsVBO;
		};

	  private:
		Vec4f _bgColor = Vec4f( 0.8f, 0.8f, 0.8f, 1.f );
		
		GLuint					 _program;

		static const std::string _shaderFolder;

		Mesh createCube();
		Mesh _cube;
		GLuint _cubeTransformLocation;
	};
} // namespace M3D_ISICG

#endif // __LAB_WORK_2_HPP__