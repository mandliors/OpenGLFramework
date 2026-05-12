#pragma once

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "OpenMesh/Core/IO/MeshIO.hh"
#include "OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh"

#include "Shader/Shader.hpp"
#include "Rendering/RenderState.hpp"

#include <vector>

using PolyMesh = OpenMesh::PolyMesh_ArrayKernelT<>;

class MeshRenderer
{
public:
	MeshRenderer(const PolyMesh &topologyMesh, const Shader &faceShader, const Shader &edgeShader, const Shader &pointShader, const glm::vec4 &faceColor, const glm::vec4 &edgeColor, const glm::vec4 &pointColor);
	~MeshRenderer();

	auto UpdateGPU() -> void;
	auto Draw(const RenderState &state) const -> void;

	auto SetDrawMode(bool drawFaces, bool drawEdges, bool drawPoints) -> void;

private:
	auto InitGL() -> void;

private:
	const PolyMesh &m_mesh;

	const Shader &m_faceShader;
	const Shader &m_edgeShader;
	const Shader &m_pointShader;

	glm::vec4 m_faceColor;
	glm::vec4 m_edgeColor;
	glm::vec4 m_pointColor;

	std::vector<uint32_t> m_edgeIndices;
	std::vector<uint32_t> m_pointIndices;

	GLuint m_faceVao, m_faceVbo;
	GLuint m_structuralVao, m_structuralVbo;
	GLuint m_edgeEbo, m_pointEbo;

	uint32_t m_faceCount;
	uint32_t m_edgeIndexCount;
	uint32_t m_pointIndexCount;

	bool m_drawFaces = true;
	bool m_drawEdges = true;
	bool m_drawPoints = true;
};