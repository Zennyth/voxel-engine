#include "voxel_instancer_world.h"

namespace zylann::voxel {

void VoxelInstancerWorld::init_library() {
	generator = Object::cast_to<VoxelGeneratorWorld>(*(_parent->get_generator()))->duplicate();

	if (generator == nullptr)
		return;

	int index = 0;

	VoxelInstanceLibrary librairy = VoxelInstanceLibrary();
	map_index_biomes = {};

	for (const Ref<Biome> &biome : generator->get_casted_biomes()) {
		if (biome->get_library() != nullptr && biome->get_library().is_valid()) {
			biome->get_library()->for_each_item([this, &index, &librairy, &biome](int id, VoxelInstanceLibraryItem &item) {
				librairy.add_item(index, &item);
				map_index_biomes[index] = biome;
				index++;
			});
		}
	}

	set_library(librairy.duplicate());
	
}
bool VoxelInstancerWorld::is_item_spawnable(int index, Vector3i position) {
	if (generator == nullptr)
		return false;

	Vector2 grid_position = Vector2(position.x, position.z);
	return generator->biome_map.get_biome_by_position(grid_position)->get_biome_name() == map_index_biomes[index]->get_biome_name();
}

}
