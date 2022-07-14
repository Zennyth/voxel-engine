#ifndef VOXEL_INSTANCER_WORLD_H
#define VOXEL_INSTANCER_WORLD_H

#include <modules/voxel/generators/simple/voxel_generator_world.h>
#include <modules/voxel/terrain/instancing/voxel_instancer.h>
#include <modules/voxel/terrain/voxel_node.h>
#include <map>

namespace zylann::voxel {

class VoxelInstancerWorld : public VoxelInstancer {
	GDCLASS(VoxelInstancerWorld, VoxelInstancer)

protected:
	void init_library() override;
	bool is_item_spawnable(int index, Vector3i grid_position) override;

private:
	HashMap<int, Ref<Biome>> map_index_biomes = {};
	Ref<VoxelGeneratorWorld> generator;
};

} // namespace zylann::voxel

#endif // VOXEL_INSTANCER_WORLD_H
