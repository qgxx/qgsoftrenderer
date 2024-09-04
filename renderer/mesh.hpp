#pragma once

#include <vector>
#include <memory>
#include <string>

#include <glm/glm.hpp>

#include "context.hpp"

namespace sr {

struct Vertex {
	glm::vec3 m_vpositions = glm::vec3(0, 0, 0);
	glm::vec2 m_vtexcoords = glm::vec2(0, 0);
	glm::vec3 m_vnormals   = glm::vec3(0, 1, 0);
	glm::vec3 m_vtangent;
	glm::vec3 m_vbitangent;
};

using VertexBuffer = std::vector<Vertex>;
using IndexBuffer  = std::vector<unsigned int>;

class Mesh {
public:
	typedef std::shared_ptr<Mesh> ptr;

	Mesh() = default;
	~Mesh() = default;
	
	Mesh(const Mesh& mesh) : m_vertices(mesh.m_vertices), m_indices(mesh.m_indices), 
	m_drawingMaterial(mesh.m_drawingMaterial) {}

	Mesh& Mesh::operator=(const Mesh& mesh) {
		if (&mesh == this) return *this;
		m_vertices = mesh.m_vertices;
		m_indices = mesh.m_indices;
		m_drawingMaterial = mesh.m_drawingMaterial;
		return *this;
	}

	void setVertices(const std::vector<Vertex> &vertices) { m_vertices = vertices; }
	void setIndices(const std::vector<unsigned int> &indices) { m_indices = indices; }

	void setDiffuseMapTexId(const int &id) { m_drawingMaterial.m_diffuseMapTexId = id; }
	void setSpecularMapTexId(const int &id) { m_drawingMaterial.m_specularMapTexId = id; }
	void setNormalMapTexId(const int &id) { m_drawingMaterial.m_normalMapTexId = id; }
	void setGlowMapTexId(const int &id) { m_drawingMaterial.m_glowMapTexId = id; }

	const int& getDiffuseMapTexId() const { return m_drawingMaterial.m_diffuseMapTexId; }
	const int& getSpecularMapTexId() const { return m_drawingMaterial.m_specularMapTexId; }
	const int& getNormalMapTexId() const { return m_drawingMaterial.m_normalMapTexId; }
	const int& getGlowMapTexId() const { return m_drawingMaterial.m_glowMapTexId; }

	VertexBuffer& getVertices() { return m_vertices; }
	IndexBuffer& getIndices() { return m_indices; }
	const std::vector<Vertex>& getVertices() const { return m_vertices; }
	const std::vector<unsigned int>& getIndices() const { return m_indices; }

	void clear() {
		std::vector<Vertex>().swap(m_vertices);
		std::vector<unsigned int>().swap(m_indices);
	}

private:
	VertexBuffer m_vertices;
	IndexBuffer  m_indices;

	struct MaterialTex {
		int m_diffuseMapTexId = -1;
		int m_specularMapTexId = -1;
		int m_normalMapTexId = -1;
		int m_glowMapTexId = -1;
	} m_drawingMaterial;

};

using MeshBuffer = std::vector<Mesh>;

} // namespace sr