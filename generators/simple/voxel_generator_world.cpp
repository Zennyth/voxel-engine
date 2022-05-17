#include "voxel_generator_world.h"
#include <core/config/engine.h>
#include <core/core_string_names.h>
#include <modules/noise/fastnoise_lite.h>
#include <modules/voxel/util/math/color8.h>
#include <scene/resources/curve.h>

#include <scene\resources\gradient.h>

namespace zylann::voxel {
VoxelGeneratorWorld::VoxelGeneratorWorld() {
	border_delimeter = CellNoise();
}

VoxelGeneratorWorld::~VoxelGeneratorWorld() {}

Ref<Biome> VoxelGeneratorWorld::get_biome_by(float temperature, float moisture) {
	// fallback to default if no biome meets the requirements
	Ref<Biome> biome_ref{ Object::cast_to<Biome>(_biomes[0]) };

	for (int i = 0; i < _biomes.size(); ++i) {
		Ref<Biome> biome_new_ref{ Object::cast_to<Biome>(_biomes[i]) };
		if (biome_new_ref.ptr()->is_in_range(temperature, moisture)) {
			biome_ref = biome_new_ref;
			break;
		}
	}

	return biome_ref;
}

void VoxelGeneratorWorld::set_temperature_noise(Ref<FastNoiseLite> temperature_noise) {
	if (_temperature_noise == temperature_noise) {
		return;
	}
	if (_temperature_noise.is_valid()) {
		_temperature_noise->disconnect(CoreStringNames::get_singleton()->changed,
				callable_mp(this, &VoxelGeneratorWorld::_on_temperature_noise_changed));
	}
	_temperature_noise = temperature_noise;
	Ref<FastNoiseLite> copy;
	if (_temperature_noise.is_valid()) {
		_temperature_noise->connect(CoreStringNames::get_singleton()->changed,
				callable_mp(this, &VoxelGeneratorWorld::_on_temperature_noise_changed));
		// The OpenSimplexNoise resource is not thread-safe so we make a copy of it for use in threads
		copy = _temperature_noise->duplicate();
	}
	// The OpenSimplexNoise resource is not thread-safe so we make a copy of it for use in threads
	RWLockWrite wlock(_parameters_lock);
	_parameters.temperature_noise = copy;
}
void VoxelGeneratorWorld::_on_temperature_noise_changed() {
	ERR_FAIL_COND(_temperature_noise.is_null());
	RWLockWrite wlock(_parameters_lock);
	_parameters.temperature_noise = _temperature_noise->duplicate();
}
Ref<FastNoiseLite> VoxelGeneratorWorld::get_temperature_noise() const {
	return _temperature_noise;
}

void VoxelGeneratorWorld::set_moisture_noise(Ref<FastNoiseLite> moisture_noise) {
	if (_moisture_noise == moisture_noise) {
		return;
	}
	if (_moisture_noise.is_valid()) {
		_moisture_noise->disconnect(CoreStringNames::get_singleton()->changed,
				callable_mp(this, &VoxelGeneratorWorld::_on_moisture_noise_changed));
	}
	_moisture_noise = moisture_noise;
	Ref<FastNoiseLite> copy;
	if (_moisture_noise.is_valid()) {
		_moisture_noise->connect(CoreStringNames::get_singleton()->changed,
				callable_mp(this, &VoxelGeneratorWorld::_on_moisture_noise_changed));
		// The OpenSimplexNoise resource is not thread-safe so we make a copy of it for use in threads
		copy = _moisture_noise->duplicate();
	}
	// The OpenSimplexNoise resource is not thread-safe so we make a copy of it for use in threads
	RWLockWrite wlock(_parameters_lock);
	_parameters.moisture_noise = copy;
}
void VoxelGeneratorWorld::_on_moisture_noise_changed() {
	ERR_FAIL_COND(_moisture_noise.is_null());
	RWLockWrite wlock(_parameters_lock);
	_parameters.moisture_noise = _moisture_noise->duplicate();
}
Ref<FastNoiseLite> VoxelGeneratorWorld::get_moisture_noise() const {
	return _moisture_noise;
}

void VoxelGeneratorWorld::set_continentalness_noise(Ref<FastNoiseLite> continentalness_noise) {
	if (_continentalness_noise == continentalness_noise) {
		return;
	}
	if (_continentalness_noise.is_valid()) {
		_continentalness_noise->disconnect(CoreStringNames::get_singleton()->changed,
				callable_mp(this, &VoxelGeneratorWorld::_on_continentalness_noise_changed));
	}
	_continentalness_noise = continentalness_noise;
	Ref<FastNoiseLite> copy;
	if (_continentalness_noise.is_valid()) {
		_continentalness_noise->connect(CoreStringNames::get_singleton()->changed,
				callable_mp(this, &VoxelGeneratorWorld::_on_continentalness_noise_changed));
		// The OpenSimplexNoise resource is not thread-safe so we make a copy of it for use in threads
		copy = _continentalness_noise->duplicate();
	}
	// The OpenSimplexNoise resource is not thread-safe so we make a copy of it for use in threads
	RWLockWrite wlock(_parameters_lock);
	_parameters.continentalness_noise = copy;
}
void VoxelGeneratorWorld::_on_continentalness_noise_changed() {
	ERR_FAIL_COND(_continentalness_noise.is_null());
	RWLockWrite wlock(_parameters_lock);
	_parameters.continentalness_noise = _continentalness_noise->duplicate();
}
Ref<FastNoiseLite> VoxelGeneratorWorld::get_continentalness_noise() const {
	return _continentalness_noise;
}

void VoxelGeneratorWorld::set_peaks_and_valleys_noise(Ref<FastNoiseLite> peaks_and_valleys_noise) {
	if (_peaks_and_valleys_noise == peaks_and_valleys_noise) {
		return;
	}
	if (_peaks_and_valleys_noise.is_valid()) {
		_peaks_and_valleys_noise->disconnect(CoreStringNames::get_singleton()->changed,
				callable_mp(this, &VoxelGeneratorWorld::_on_peaks_and_valleys_noise_changed));
	}
	_peaks_and_valleys_noise = peaks_and_valleys_noise;
	Ref<FastNoiseLite> copy;
	if (_peaks_and_valleys_noise.is_valid()) {
		_peaks_and_valleys_noise->connect(CoreStringNames::get_singleton()->changed,
				callable_mp(this, &VoxelGeneratorWorld::_on_peaks_and_valleys_noise_changed));
		// The OpenSimplexNoise resource is not thread-safe so we make a copy of it for use in threads
		copy = _peaks_and_valleys_noise->duplicate();
	}
	// The OpenSimplexNoise resource is not thread-safe so we make a copy of it for use in threads
	RWLockWrite wlock(_parameters_lock);
	_parameters.peaks_and_valleys_noise = copy;
}
void VoxelGeneratorWorld::_on_peaks_and_valleys_noise_changed() {
	ERR_FAIL_COND(_peaks_and_valleys_noise.is_null());
	RWLockWrite wlock(_parameters_lock);
	_parameters.peaks_and_valleys_noise = _peaks_and_valleys_noise->duplicate();
}
Ref<FastNoiseLite> VoxelGeneratorWorld::get_peaks_and_valleys_noise() const {
	return _peaks_and_valleys_noise;
}

void VoxelGeneratorWorld::set_erosion_noise(Ref<FastNoiseLite> erosion_noise) {
	if (_erosion_noise == erosion_noise) {
		return;
	}
	if (_erosion_noise.is_valid()) {
		_erosion_noise->disconnect(CoreStringNames::get_singleton()->changed,
				callable_mp(this, &VoxelGeneratorWorld::_on_erosion_noise_changed));
	}
	_erosion_noise = erosion_noise;
	Ref<FastNoiseLite> copy;
	if (_erosion_noise.is_valid()) {
		_erosion_noise->connect(CoreStringNames::get_singleton()->changed,
				callable_mp(this, &VoxelGeneratorWorld::_on_erosion_noise_changed));
		// The OpenSimplexNoise resource is not thread-safe so we make a copy of it for use in threads
		copy = _erosion_noise->duplicate();
	}
	// The OpenSimplexNoise resource is not thread-safe so we make a copy of it for use in threads
	RWLockWrite wlock(_parameters_lock);
	_parameters.erosion_noise = copy;
}
void VoxelGeneratorWorld::_on_erosion_noise_changed() {
	ERR_FAIL_COND(_erosion_noise.is_null());
	RWLockWrite wlock(_parameters_lock);
	_parameters.erosion_noise = _erosion_noise->duplicate();
}
Ref<FastNoiseLite> VoxelGeneratorWorld::get_erosion_noise() const {
	return _erosion_noise;
}

void VoxelGeneratorWorld::set_biomes(Array biomes) {
	_biomes = biomes;
	emit_changed();
}
Array VoxelGeneratorWorld::get_biomes() const {
	return _biomes;
}

VoxelGenerator::Result VoxelGeneratorWorld::generate_block(VoxelGenerator::VoxelQueryData &input) {
	Parameters params;
	{
		RWLockRead rlock(_parameters_lock);
		params = _parameters;
	}

	Result result;

	ERR_FAIL_COND_V(_biomes.is_empty(), result);
	ERR_FAIL_COND_V(params.temperature_noise.is_null(), result);
	ERR_FAIL_COND_V(params.moisture_noise.is_null(), result);
	ERR_FAIL_COND_V(params.continentalness_noise.is_null(), result);
	ERR_FAIL_COND_V(params.peaks_and_valleys_noise.is_null(), result);
	ERR_FAIL_COND_V(params.erosion_noise.is_null(), result);

	FastNoiseLite &temperature_noise = **params.temperature_noise;
	FastNoiseLite &moisture_noise = **params.moisture_noise;
	FastNoiseLite &continentalness_noise = **params.continentalness_noise;
	FastNoiseLite &peaks_and_valleys_noise = **params.peaks_and_valleys_noise;
	FastNoiseLite &erosion_noise = **params.erosion_noise;

	VoxelBufferInternal &out_buffer = input.voxel_buffer;
	out_buffer.set_channel_depth(channel, VoxelBufferInternal::DEPTH_16_BIT);

	const Vector3i origin = input.origin_in_voxels;
	const Vector3i bs = out_buffer.get_size();
	const float margin = 1 << input.lod;
	const int lod = input.lod;
	const int offest = 1;

	const int height = 200;
	const Color8 ground = Color8(255, 255, 255, 255);

	const int water_level = 50;
	const Color8 water = Color8(255, 255, 255, 150);

	if (origin.y > height + margin) {
		// The bottom of the block is above the highest ground can go (default is air)
		result.max_lod_hint = true;
		return result;
	}
	if (origin.y + (bs.y << lod) < 0) {
		// The top of the block is below the lowest ground can go
		out_buffer.clear_channel(channel, ground.to_u16());
		result.max_lod_hint = true;
		return result;
	}

	// Blocky
	int gz = origin.z;
	const int stride = 1;

	for (int z = 0; z < bs.z; ++z, gz += stride) {
		int gx = origin.x;

		for (int x = 0; x < bs.x; ++x, gx += stride) {
			// terrain shape
			float noise_continentalness = 0.5 + 0.5 * continentalness_noise.get_noise_2d(gx / offest, gz / offest);
			float noise_peaks_and_valleys = 0.5 + 0.5 * peaks_and_valleys_noise.get_noise_2d(gx / offest, gz / offest);
			float noise_erosion = 0.5 + 0.5 * erosion_noise.get_noise_2d(gx / offest, gz / offest);

			ClosestPointsResult cell_noise = border_delimeter.get_closest_result(Vector2(gx, gz));
			Vector2 current_biome_center = cell_noise.closest.location;
			// biome selection
			float current_noise_temperature =
					0.5 + 0.5 * temperature_noise.get_noise_2d(current_biome_center.x, current_biome_center.y);
			float current_noise_moisture =
					0.5 + 0.5 * moisture_noise.get_noise_2d(current_biome_center.x, current_biome_center.y);

			// fallback to default if no biome meets the requirements
			Ref<Biome> current_biome_ref{ Object::cast_to<Biome>(_biomes[0]) };
			for (int i = 0; i < _biomes.size(); ++i) {
				Ref<Biome> current_biome_new_ref{ Object::cast_to<Biome>(_biomes[i]) };
				if (current_biome_new_ref->is_in_range(current_noise_temperature, current_noise_moisture)) {
					current_biome_ref = current_biome_new_ref;
					break;
				}
			}
			Biome &current_biome = **current_biome_ref;


			float h = 0;

			if (cell_noise.under_threshold.size() > 1) {
				for (PointResult const &biome_center : cell_noise.under_threshold) {
					Vector2 closest_biome_center = biome_center.location;
					// biome selection
					float noise_temperature =
							0.5 + 0.5 * temperature_noise.get_noise_2d(closest_biome_center.x, closest_biome_center.y);
					float noise_moisture =
							0.5 + 0.5 * moisture_noise.get_noise_2d(closest_biome_center.x, closest_biome_center.y);

					// fallback to default if no biome meets the requirements
					Ref<Biome> biome_ref{ Object::cast_to<Biome>(_biomes[0]) };
					for (int i = 0; i < _biomes.size(); ++i) {
						Ref<Biome> biome_new_ref{ Object::cast_to<Biome>(_biomes[i]) };
						if (biome_new_ref->is_in_range(noise_temperature, noise_moisture)) {
							biome_ref = biome_new_ref;
							break;
						}
					}
					Biome &biome = **biome_ref;

					// Output is blocky, so we can go for just one sample
					float normalized_height_map = biome.get_continentalness()->get_height_at(noise_continentalness) +
							biome.get_peaks_and_valleys()->get_height_at(noise_peaks_and_valleys) +
							biome.get_erosion()->get_height_at(noise_erosion);

					h += normalized_height_map * height * biome_center.weight;
				}
			} else {
				// Output is blocky, so we can go for just one sample
				float normalized_height_map = current_biome.get_continentalness()->get_height_at(noise_continentalness) +
						current_biome.get_peaks_and_valleys()->get_height_at(noise_peaks_and_valleys) +
						current_biome.get_erosion()->get_height_at(noise_erosion);

				h = normalized_height_map * height;
			}

			h -= origin.y;
			int ih = int(h) >> lod;
			
			if (water_level > origin.y) {
				int start_relative_height = 0;
				if(ih < water_level - origin.y) {
					start_relative_height = ih;
				}

				int fill_level = bs.y;
				if(origin.y + bs.y > water_level) {
					fill_level = water_level - origin.y - 1;
				}

				// int fill_level = bs.y;
				out_buffer.fill_area(water.to_u16(), Vector3i(x, start_relative_height, z),
						Vector3i(x + 1, fill_level, z + 1), channel);
			}

			if (ih > 0) {
				if (ih > bs.y) {
					ih = bs.y;
				}

				out_buffer.fill_area(current_biome.get_color_at(noise_continentalness), Vector3i(x, 0, z),
						Vector3i(x + 1, ih, z + 1), channel);
			}

		} // for x
	} // for z

	out_buffer.compress_uniform_channels();
	return result;
}

void VoxelGeneratorWorld::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_biomes", "biomes"), &VoxelGeneratorWorld::set_biomes);
	ClassDB::bind_method(D_METHOD("get_biomes"), &VoxelGeneratorWorld::get_biomes);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "biomes", PROPERTY_HINT_ARRAY_TYPE, "RESOURCE"), "set_biomes", "get_biomes");

	ADD_GROUP("Temperature and Moisture", "");
	ClassDB::bind_method(
			D_METHOD("set_temperature_noise", "temperature_noise"), &VoxelGeneratorWorld::set_temperature_noise);
	ClassDB::bind_method(D_METHOD("get_temperature_noise"), &VoxelGeneratorWorld::get_temperature_noise);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "temperature_noise", PROPERTY_HINT_RESOURCE_TYPE,
						 FastNoiseLite::get_class_static(),
						 PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_EDITOR_INSTANTIATE_OBJECT),
			"set_temperature_noise", "get_temperature_noise");
	ClassDB::bind_method(D_METHOD("set_moisture_noise", "moisture_noise"), &VoxelGeneratorWorld::set_moisture_noise);
	ClassDB::bind_method(D_METHOD("get_moisture_noise"), &VoxelGeneratorWorld::get_moisture_noise);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "moisture_noise", PROPERTY_HINT_RESOURCE_TYPE,
						 FastNoiseLite::get_class_static(),
						 PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_EDITOR_INSTANTIATE_OBJECT),
			"set_moisture_noise", "get_moisture_noise");

	ADD_GROUP("Height", "");
	ClassDB::bind_method(D_METHOD("set_continentalness_noise", "continentalness_noise"),
			&VoxelGeneratorWorld::set_continentalness_noise);
	ClassDB::bind_method(D_METHOD("get_continentalness_noise"), &VoxelGeneratorWorld::get_continentalness_noise);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "continentalness_noise", PROPERTY_HINT_RESOURCE_TYPE,
						 FastNoiseLite::get_class_static(),
						 PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_EDITOR_INSTANTIATE_OBJECT),
			"set_continentalness_noise", "get_continentalness_noise");
	ClassDB::bind_method(D_METHOD("set_peaks_and_valleys_noise", "peaks_and_valleys_noise"),
			&VoxelGeneratorWorld::set_peaks_and_valleys_noise);
	ClassDB::bind_method(D_METHOD("get_peaks_and_valleys_noise"), &VoxelGeneratorWorld::get_peaks_and_valleys_noise);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "peaks_and_valleys_noise", PROPERTY_HINT_RESOURCE_TYPE,
						 FastNoiseLite::get_class_static(),
						 PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_EDITOR_INSTANTIATE_OBJECT),
			"set_peaks_and_valleys_noise", "get_peaks_and_valleys_noise");
	ClassDB::bind_method(D_METHOD("set_erosion_noise", "erosion_noise"), &VoxelGeneratorWorld::set_erosion_noise);
	ClassDB::bind_method(D_METHOD("get_erosion_noise"), &VoxelGeneratorWorld::get_erosion_noise);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "erosion_noise", PROPERTY_HINT_RESOURCE_TYPE,
						 FastNoiseLite::get_class_static(),
						 PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_EDITOR_INSTANTIATE_OBJECT),
			"set_erosion_noise", "get_erosion_noise");
}

VoxelBufferInternal::ChannelId VoxelGeneratorWorld::get_channel() const {
	return channel;
}

} // namespace zylann::voxel
