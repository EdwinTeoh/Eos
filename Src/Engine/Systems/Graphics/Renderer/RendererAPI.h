#pragma once

#include <glm/glm.hpp>

#include "VertexArray.h"

class RendererAPI
{
public:
	enum class API
	{
		None = 0, OpenGL = 1
	};
	virtual ~RendererAPI() = default;

	virtual void Init() = 0;
	virtual void SetViewport(GLuint x, GLuint y, GLuint width, GLuint height) = 0;
	virtual void SetClearColor(const glm::vec4& color) = 0;
	virtual void Clear() = 0;

	virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, GLuint indexCount = 0) = 0;
	virtual void DrawIndexLines(const std::shared_ptr<VertexArray>& vertexArray, GLuint indexCount = 0) = 0;

	static API GetAPI() { return s_API; }
	static std::unique_ptr<RendererAPI> Create();
private:
	static API s_API;
};