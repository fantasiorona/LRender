#pragma once

#include "glad/glad.h"

#include <string>

namespace LRender {
	class Shader final {
	public:
		Shader(const std::string &vertex, const std::string &fragment);
		~Shader();
		void use() const;

	private:
		GLuint program;

		static const std::string VERSION;
		static const std::string FILE_UNIFORM_BLOCK;
		static GLuint createShader(const GLenum type, const GLchar *code);

		std::string readFile(const std::string &file);
	};
};