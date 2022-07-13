#include <core/config/engine.h>
#include <core/core_string_names.h>
#include <modules/noise/fastnoise_lite.h>
#include <scene/resources/curve.h>
#include <scene\resources\gradient.h>

#include "voxel_generator_world.h"

namespace zylann::voxel {
VoxelGeneratorWorld::VoxelGeneratorWorld() {
    biome_map = BiomeMap();
}

VoxelGeneratorWorld::~VoxelGeneratorWorld() {}

float VoxelGeneratorWorld::normalize_noise_2d(Ref<FastNoiseLite> noise, int x, int y, int offest) {
    return 0.5 + 0.5 * noise->get_noise_2d(x / offest, y / offest);
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
	biome_map.set_temperature_noise(copy);
}
void VoxelGeneratorWorld::_on_temperature_noise_changed() {
	ERR_FAIL_COND(_temperature_noise.is_null());
	RWLockWrite wlock(_parameters_lock);
	_parameters.temperature_noise = _temperature_noise->duplicate();
    biome_map.set_temperature_noise(_temperature_noise->duplicate());
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
	biome_map.set_moisture_noise(copy);
}
void VoxelGeneratorWorld::_on_moisture_noise_changed() {
	ERR_FAIL_COND(_moisture_noise.is_null());
	RWLockWrite wlock(_parameters_lock);
	_parameters.moisture_noise = _moisture_noise->duplicate();
    biome_map.set_moisture_noise(_moisture_noise->duplicate());
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
    ordered_biomes = {};

    for (int i = 0; i < _biomes.size(); ++i) {
		Ref<Biome> biome{ Object::cast_to<Biome>(_biomes[i]) };

		if (biome == nullptr) {
			break;
		}

		Biome::Humidity humidity = biome->get_humidity();
		Biome::Temperature temperature = biome->get_temperature();

        if(!ordered_biomes.has(humidity)) {
            ordered_biomes[humidity] = {};
        }

        if(!ordered_biomes[humidity].has(temperature)) {
            ordered_biomes[humidity][temperature] = {};
        }

		print_line("[SET BIOMES] humidity: %i, temperature: %i", humidity, temperature);
        ordered_biomes[humidity][temperature].push_back(biome);
    }

    biome_map.set_biomes(ordered_biomes);
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

	VoxelBufferInternal &out_buffer = input.voxel_buffer;
	out_buffer.set_channel_depth(channel, VoxelBufferInternal::DEPTH_16_BIT);

	const Vector3i origin = input.origin_in_voxels;
	const Vector3i bs = out_buffer.get_size();
	const float margin = 1 << input.lod;
	const int lod = input.lod;

	// print_line("================= DEBUG STARTED =================");

	if (origin.y > height + margin) {
		// The bottom of the block is above the highest ground can go (default is air)
		result.max_lod_hint = true;
		return result;
	}
	if (origin.y + (bs.y << lod) < 0) {
		// The top of the block is below the lowest ground can go
		out_buffer.clear_channel(channel, ground);
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
			float continentalness = normalize_noise_2d(params.continentalness_noise, gx, gz, offset);
			float peaks_and_valleys = normalize_noise_2d(params.peaks_and_valleys_noise, gx, gz, offset);
			float erosion = normalize_noise_2d(params.erosion_noise, gx, gz, offset);

            List<WeightedBiomeInstance> weighted_biomes = biome_map.get_closest_biomes(Vector2(gx, gz));
			// print_line("================= DEBUG AFTER WEIGHTED_BIOMES =================");
			// print_line(weighted_biomes.size());
            WeightedBiomeInstance *current_biome = nullptr;

            float h = 0;
            for (WeightedBiomeInstance &weighted_biome : weighted_biomes) {
				// print_line("================= DEBUG NORMALIZED HEIGHT MAP =================");


				//print_line("biome_instance: ", weighted_biome.biome_instance != nullptr);
				//print_line("biome: ", weighted_biome.biome_instance->biome != nullptr);
				//print_line("continentalness: ", weighted_biome.biome_instance->biome->get_continentalness() != nullptr);

                float normalized_height_map = 
                    weighted_biome.biome_instance->biome->get_continentalness()->get_height_at(continentalness) +
					weighted_biome.biome_instance->biome->get_peaks_and_valleys()->get_height_at(peaks_and_valleys) +
					weighted_biome.biome_instance->biome->get_erosion()->get_height_at(erosion);

				// print_line("================= DEBUG AFTER NORMALIZED HEIGHT MAP =================");
                
                if(current_biome == nullptr || current_biome->weight < weighted_biome.weight)
                    current_biome = &weighted_biome;

				h += normalized_height_map * height * weighted_biome.weight;
            }

			// print_line("================= DEBUG FILLING =================");

			h -= origin.y;

			int ih = int(h) >> lod;
			
			if (water_level > origin.y) {
				const int start_relative_height   = ih < water_level - origin.y     ? ih                         : 0;
				const int fill_level              = origin.y + bs.y > water_level   ? water_level - origin.y - 1 : bs.y;

				out_buffer.fill_area(
                    water, 
                    Vector3i(x, start_relative_height, z), Vector3i(x + 1, fill_level, z + 1), 
                    channel
                );
			}

			if (ih > 0) {
                const int surface_threshold     = current_biome->biome_instance->biome->get_surface_threshold();
                const int fill_height           = math::min(ih, bs.y);
                const int surface_fill          = ih <= bs.y ? surface_threshold : 0;

				out_buffer.fill_area(
                    current_biome->biome_instance->biome->get_underground_color_at(continentalness), 
                    Vector3i(x, 0, z), Vector3i(x + 1, fill_height - surface_fill, z + 1), 
                    channel
                );

                if(surface_fill > 0) {
                    out_buffer.fill_area(
                        current_biome->biome_instance->biome->get_surface_color_at(continentalness), 
                        Vector3i(x, fill_height - surface_fill, z), Vector3i(x + 1, fill_height, z + 1), 
                        channel
                    );
                }
			}
		} // for x
	} // for z

	out_buffer.compress_uniform_channels();
	return result;
}

void VoxelGeneratorWorld::_bind_methods() {
    ADD_GROUP("Biomes", "");
	ClassDB::bind_method(D_METHOD("set_biomes", "biomes"), &VoxelGeneratorWorld::set_biomes);
	ClassDB::bind_method(D_METHOD("get_biomes"), &VoxelGeneratorWorld::get_biomes);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "biomes", PROPERTY_HINT_TYPE_STRING, "17/17:Biome"), "set_biomes", "get_biomes");



	ADD_GROUP("Biome Map", "");
	ClassDB::bind_method(D_METHOD("set_temperature_noise", "temperature_noise"), &VoxelGeneratorWorld::set_temperature_noise);
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
    
    ClassDB::bind_method(D_METHOD("set_map_offset", "new_offset"), &VoxelGeneratorWorld::set_map_offset);
	ClassDB::bind_method(D_METHOD("get_map_offset"), &VoxelGeneratorWorld::get_map_offset);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "map_offset", PROPERTY_HINT_RANGE, String("{0},{1},1").format(varray(MIN_OFFSET, MAX_OFFEST))), "set_map_offset", "get_map_offset");
    ClassDB::bind_method(D_METHOD("set_closest_biome_threshold", "new_closest_biome_threshold"), &VoxelGeneratorWorld::set_closest_biome_threshold);
	ClassDB::bind_method(D_METHOD("get_closest_biome_threshold"), &VoxelGeneratorWorld::get_closest_biome_threshold);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "closest_biome_threshold", PROPERTY_HINT_RANGE, String("{0},{1},1").format(varray(0, 10000000))), "set_closest_biome_threshold", "get_closest_biome_threshold");
    ClassDB::bind_method(D_METHOD("set_scale", "new_scale"), &VoxelGeneratorWorld::set_scale);
	ClassDB::bind_method(D_METHOD("get_scale"), &VoxelGeneratorWorld::get_scale);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "scale", PROPERTY_HINT_RANGE, String("{0},{1},1").format(varray(1, 100000))), "set_scale", "get_scale");


	ADD_GROUP("Terrain", "");
    ClassDB::bind_method(D_METHOD("set_height", "new_height"), &VoxelGeneratorWorld::set_height);
	ClassDB::bind_method(D_METHOD("get_height"), &VoxelGeneratorWorld::get_height);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "height", PROPERTY_HINT_RANGE, String("{0},{1},1").format(varray(MIN_HEIGHT, MAX_HEIGHT))), "set_height", "get_height");
    ClassDB::bind_method(D_METHOD("set_water_level", "new_water_level"), &VoxelGeneratorWorld::set_water_level);
	ClassDB::bind_method(D_METHOD("get_water_level"), &VoxelGeneratorWorld::get_water_level);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "water_level", PROPERTY_HINT_RANGE, String("{0},{1},1").format(varray(MIN_WATER_LEVEL, MAX_WATER_LEVEL))), "set_water_level", "get_water_level");
    ClassDB::bind_method(D_METHOD("set_offset", "new_offset"), &VoxelGeneratorWorld::set_offset);
	ClassDB::bind_method(D_METHOD("get_offset"), &VoxelGeneratorWorld::get_offset);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "offset", PROPERTY_HINT_RANGE, String("{0},{1},1").format(varray(MIN_OFFSET, MAX_OFFEST))), "set_offset", "get_offset");
    
	ClassDB::bind_method(D_METHOD("set_continentalness_noise", "continentalness_noise"), &VoxelGeneratorWorld::set_continentalness_noise);
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

int VoxelGeneratorWorld::get_height() const {
	return height;
}
void VoxelGeneratorWorld::set_height(int new_height) {
	height = math::clamp(new_height, MIN_HEIGHT, MAX_HEIGHT);
}
int VoxelGeneratorWorld::get_water_level() const {
	return water_level;
}
void VoxelGeneratorWorld::set_water_level(int new_water_level) {
	water_level = math::clamp(new_water_level, MIN_WATER_LEVEL, MAX_WATER_LEVEL);
}
int VoxelGeneratorWorld::get_offset() const {
	return offset;
}
void VoxelGeneratorWorld::set_offset(int new_offset) {
	offset = math::clamp(new_offset, MIN_OFFSET, MAX_OFFEST);
}

int VoxelGeneratorWorld::get_map_offset() const {
    return biome_map.get_offset();
}
void VoxelGeneratorWorld::set_map_offset(int new_offset) {
    biome_map.set_offset(new_offset);
}
int VoxelGeneratorWorld::get_closest_biome_threshold() const {
    return biome_map.get_closest_biome_threshold();
}
void VoxelGeneratorWorld::set_closest_biome_threshold(int new_closest_biome_threshold) {
    biome_map.set_closest_biome_threshold(new_closest_biome_threshold);
}
int VoxelGeneratorWorld::get_scale() const {
    return biome_map.get_scale();
}
void VoxelGeneratorWorld::set_scale(int new_scale) {
    biome_map.set_scale(new_scale);
}

} // namespace zylann::voxel
