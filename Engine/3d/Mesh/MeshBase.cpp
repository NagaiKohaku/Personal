#include "MeshBase.h"

void MeshBase::CopyMeshData(std::vector<uint32_t> indices, std::vector<VertexData> vertices) {

	std::memcpy(vertexData_, vertices.data(), sizeof(VertexData) * vertices.size());
	std::memcpy(indexData_, indices.data(), sizeof(uint32_t) * indices.size());
}