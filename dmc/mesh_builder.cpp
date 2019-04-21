#include "mesh_builder.h"

namespace dmc {

Ref<ArrayMesh> MeshBuilder::commit(bool wireframe) {

	if (_positions.size() == 0) {
		return Ref<ArrayMesh>();
	}

	ERR_FAIL_COND_V(_indices.size() % 3 != 0, Ref<ArrayMesh>());

	if (wireframe) {

		// Debug purpose, no effort to be fast here
		std::vector<int> wireframe_indices;

		for (int i = 0; i < _indices.size(); i += 3) {

			wireframe_indices.push_back(_indices[i]);
			wireframe_indices.push_back(_indices[i + 1]);

			wireframe_indices.push_back(_indices[i + 1]);
			wireframe_indices.push_back(_indices[i + 2]);

			wireframe_indices.push_back(_indices[i + 2]);
			wireframe_indices.push_back(_indices[i]);
		}

		_indices = wireframe_indices;
	}

	PoolVector3Array positions;
	PoolVector3Array normals;
	PoolIntArray indices;

	raw_copy_to(positions, _positions);
	raw_copy_to(normals, _normals);
	raw_copy_to(indices, _indices);

	clear();

	Array surface;
	surface.resize(Mesh::ARRAY_MAX);
	surface[Mesh::ARRAY_VERTEX] = positions;
	surface[Mesh::ARRAY_NORMAL] = normals;
	surface[Mesh::ARRAY_INDEX] = indices;

	Ref<ArrayMesh> mesh;
	mesh.instance();
	mesh->add_surface_from_arrays(wireframe ? Mesh::PRIMITIVE_LINES : Mesh::PRIMITIVE_TRIANGLES, surface);

	return mesh;
}

void MeshBuilder::clear() {
	_positions.clear();
	_normals.clear();
	_indices.clear();
	_position_to_index.clear();
}

} // namespace dmc
