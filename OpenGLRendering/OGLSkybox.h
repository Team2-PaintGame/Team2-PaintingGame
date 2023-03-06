#pragma once
#ifdef _WIN32
#include <vector>
#include "OGLMesh.h"
#include "OGLShader.h"
#include "Matrix4.h"
#include "Maths.h"

namespace NCL {
	namespace Rendering {
		class OGLSkybox {
		public:
			OGLSkybox();
			~OGLSkybox();
			OGLMesh* GetMesh() { return skyboxMesh; }
			GLuint& GetFBO() { return skyboxFBO; }
			GLuint& GetDayTexID() { return skyboxTexDay; }
			GLuint& GetNightTexID() { return skyboxTexNight; }
			GLuint& GetFinalTexID() { return skyboxTexFinal; }

			OGLShader* GetShader() { return skyboxShader; }

			float GetDayNightRatio() { return dayNightRatio; }

			void Update(float dt);
			Matrix4 GetTransformationMatrix();
		protected:
			void LoadTextures(std::string* filenames, GLuint& texID);

			OGLMesh* skyboxMesh;

			GLuint skyboxTexDay = 0;
			GLuint skyboxTexNight = 0;
			GLuint skyboxFBO = 0;
			GLuint skyboxTexFinal = 0;

			OGLShader* skyboxShader;

			float currentRotation = 0;
			float dayNightRatio = 0;
			
			const float ROTATE_SPEED = 5.0f;
			const float phaseShift = -Maths::PI / 2;		//so that the sin function is zero @ t = 0
			const float frequency = Maths::PI / (12 * 1);	//number of seconds to transition from day to night
		};
	}
}
#endif
