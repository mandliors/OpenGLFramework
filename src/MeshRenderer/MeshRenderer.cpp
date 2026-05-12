#include "MeshRenderer.hpp"

#include "SpecialVertexDatas.hpp"

MeshRenderer::MeshRenderer(const PolyMesh &topologyMesh, const Shader &faceShader, const Shader &edgeShader, const Shader &pointShader, const glm::vec4 &faceColor, const glm::vec4 &edgeColor, const glm::vec4 &pointColor)
	: m_mesh(topologyMesh), m_faceShader(faceShader), m_edgeShader(edgeShader), m_pointShader(pointShader), m_faceColor(faceColor), m_edgeColor(edgeColor), m_pointColor(pointColor)
{
	SetDrawMode(true, false, false);
	InitGL();
	UpdateGPU();
}
MeshRenderer::~MeshRenderer()
{
	glDeleteBuffers(1, &m_structuralVbo);
	glDeleteBuffers(1, &m_faceVbo);
	glDeleteBuffers(1, &m_edgeEbo);
	glDeleteBuffers(1, &m_pointEbo);
	glDeleteVertexArrays(1, &m_structuralVao);
	glDeleteVertexArrays(1, &m_faceVao);
}

auto MeshRenderer::UpdateGPU() -> void
{
	// ---------------------- Calculate GPU Data ---------------------
	// Structural VBO
	std::vector<glm::vec3> uniquePositions;
	for (auto v : m_mesh.vertices())
	{
		auto p = m_mesh.point(v);
		uniquePositions.emplace_back(p[0], p[1], p[2]);
	}

	// Render VBO
	std::vector<PosNormVertex> gpuData;
	for (auto f : m_mesh.faces())
	{
		auto fn = m_mesh.normal(f);

		std::vector<PolyMesh::Point> pos;
		for (auto fv : m_mesh.fv_range(f))
			pos.push_back(m_mesh.point(fv));

		if (pos.size() == 3)
		{
			glm::vec3 n{(float)fn[0], (float)fn[1], (float)fn[2]};
			for (int i = 0; i < 3; i++)
			{
				glm::vec3 p{(float)pos[i][0], (float)pos[i][1], (float)pos[i][2]};
				gpuData.emplace_back(p, n);
			}
		}
		else if (pos.size() > 3) // fan triangulation
		{
			for (size_t i = 1; i + 1 < pos.size(); i++)
			{
				glm::vec3 n{(float)fn[0], (float)fn[1], (float)fn[2]};

				glm::vec3 p0{(float)pos[0][0], (float)pos[0][1], (float)pos[0][2]};
				glm::vec3 p1{(float)pos[i][0], (float)pos[i][1], (float)pos[i][2]};
				glm::vec3 p2{(float)pos[i + 1][0], (float)pos[i + 1][1], (float)pos[i + 1][2]};

				gpuData.emplace_back(p0, n);
				gpuData.emplace_back(p1, n);
				gpuData.emplace_back(p2, n);
			}
		}
	}
	m_faceCount = static_cast<uint32_t>(gpuData.size());

	// Edge EBO
	std::vector<uint32_t> edgeIndices;
	for (auto e : m_mesh.edges())
	{
		auto he = m_mesh.halfedge_handle(e, 0);
		edgeIndices.push_back(m_mesh.from_vertex_handle(he).idx());
		edgeIndices.push_back(m_mesh.to_vertex_handle(he).idx());
	}
	m_edgeIndexCount = static_cast<uint32_t>(edgeIndices.size());

	// Point EBO
	std::vector<uint32_t> pointIndices;
	for (auto v : m_mesh.vertices())
		pointIndices.push_back(v.idx());

	m_pointIndexCount = static_cast<uint32_t>(pointIndices.size());

	// -------------------- Upload to GPU -----------------------
	// Structural VBO
	glBindBuffer(GL_ARRAY_BUFFER, m_structuralVbo);
	glBufferData(GL_ARRAY_BUFFER, uniquePositions.size() * sizeof(PosVertex), uniquePositions.data(), GL_STATIC_DRAW);

	// Face VBO
	glBindBuffer(GL_ARRAY_BUFFER, m_faceVbo);
	glBufferData(GL_ARRAY_BUFFER, gpuData.size() * sizeof(PosNormVertex), gpuData.data(), GL_DYNAMIC_DRAW);

	glBindVertexArray(m_structuralVao);

	// Edge EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_edgeEbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, edgeIndices.size() * sizeof(uint32_t), edgeIndices.data(), GL_STATIC_DRAW);

	// Point EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_pointEbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, pointIndices.size() * sizeof(uint32_t), pointIndices.data(), GL_STATIC_DRAW);
}
auto MeshRenderer::Draw(const RenderState &state) const -> void
{
	glBindVertexArray(m_faceVao);
	if (m_drawFaces)
	{
		m_faceShader.Use();
		m_faceShader.Bind(state);
		m_faceShader.SetUniform(m_faceColor, "color");

		glDrawArrays(GL_TRIANGLES, 0, m_faceCount);
	}

	glBindVertexArray(m_structuralVao);
	if (m_drawEdges)
	{
		m_edgeShader.Use();
		m_edgeShader.Bind(state);
		m_edgeShader.SetUniform(m_edgeColor, "color");

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_edgeEbo);
		glDrawElements(GL_LINES, m_edgeIndexCount, GL_UNSIGNED_INT, 0);
	}
	if (m_drawPoints)
	{
		m_pointShader.Use();
		m_pointShader.Bind(state);
		m_pointShader.SetUniform(m_pointColor, "color");

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_pointEbo);
		glDrawElements(GL_POINTS, m_pointIndexCount, GL_UNSIGNED_INT, 0);
	}

	glBindVertexArray(0);
}

auto MeshRenderer::SetDrawMode(bool drawFaces, bool drawEdges, bool drawPoints) -> void
{
	m_drawFaces = drawFaces;
	m_drawEdges = drawEdges;
	m_drawPoints = drawPoints;
}

auto MeshRenderer::InitGL() -> void
{
	glGenVertexArrays(1, &m_structuralVao);
	glGenVertexArrays(1, &m_faceVao);

	glGenBuffers(1, &m_structuralVbo);
	glGenBuffers(1, &m_faceVbo);

	glGenBuffers(1, &m_edgeEbo);
	glGenBuffers(1, &m_pointEbo);

	glBindVertexArray(m_structuralVao);
	glBindBuffer(GL_ARRAY_BUFFER, m_structuralVbo);
	for (auto const &a : PosVertexAttribs)
	{
		glEnableVertexAttribArray(a.location);
		glVertexAttribPointer(
			a.location,
			a.size,
			a.type,
			a.normalized,
			sizeof(PosVertex),
			reinterpret_cast<void *>(static_cast<uintptr_t>(a.offset)));
	}

	glBindVertexArray(m_faceVao);
	glBindBuffer(GL_ARRAY_BUFFER, m_faceVbo);
	for (auto const &a : PosNormVertexAttribs)
	{
		glEnableVertexAttribArray(a.location);
		glVertexAttribPointer(
			a.location,
			a.size,
			a.type,
			a.normalized,
			sizeof(PosNormVertex),
			reinterpret_cast<void *>(static_cast<uintptr_t>(a.offset)));
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_structuralVbo);
	glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_faceVbo);
	glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_STATIC_DRAW);
}