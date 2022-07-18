#pragma once

#include <engine/renderer/ResourceManager.h>

class Renderer
{
public:
	static void Draw(const VertexArray *va, const IndexBuffer *ib, const Shader *shader);
	static void DrawTriangles(const VertexArray* va, const Shader* shader);

	static void Clear();
private:

};

