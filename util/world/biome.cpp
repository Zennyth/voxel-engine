#include "biome.h"

#include "height_filter.h"
#include <modules/voxel/util/math/color8.h>

namespace zylann::voxel {

Biome::Biome() {}

uint16_t Biome::get_color_at(Ref<Gradient> gradient, float offset) {
	return Color8(gradient->get_color_at_offset(offset)).to_u16();
}

uint16_t Biome::get_underground_color_at(float offset) {
    return get_color_at(_underground_gradient, offset);
}
uint16_t Biome::get_surface_color_at(float offset) {
    return get_color_at(_surface_gradient, offset);
}

void Biome::set_continentalness(Ref<HeightFilter> continentalness) {
	_continentalness = continentalness;
	RWLockWrite wlock(_parameters_lock);
	_parameters.continentalness = continentalness;
}
Ref<HeightFilter> Biome::get_continentalness() const {
	return _continentalness;
}
void Biome::set_erosion(Ref<HeightFilter> erosion) {
	_erosion = erosion;
	RWLockWrite wlock(_parameters_lock);
	_parameters.erosion = erosion;
}
Ref<HeightFilter> Biome::get_erosion() const {
	return _erosion;
}
void Biome::set_peaks_and_valleys(Ref<HeightFilter> peaks_and_valleys) {
	_peaks_and_valleys = peaks_and_valleys;
	RWLockWrite wlock(_parameters_lock);
	_parameters.peaks_and_valleys = peaks_and_valleys;
}
Ref<HeightFilter> Biome::get_peaks_and_valleys() const {
	return _peaks_and_valleys;
}


int Biome::get_surface_threshold() const {
    return surface_threshold;
}
void Biome::set_surface_threshold(int _surface_threshold) {
    surface_threshold = _surface_threshold;
}
void Biome::set_underground_gradient(Ref<Gradient> gradient) {
    _underground_gradient = gradient;
	RWLockWrite wlock(_parameters_lock);
	_parameters.underground_gradient = gradient;
}
Ref<Gradient> Biome::get_underground_gradient() const {
    return _underground_gradient;
}
void Biome::set_surface_gradient(Ref<Gradient> gradient) {
    _surface_gradient = gradient;
	RWLockWrite wlock(_parameters_lock);
	_parameters.surface_gradient = gradient;
}
Ref<Gradient> Biome::get_surface_gradient() const {
    return _surface_gradient;
}

Biome::Humidity Biome::get_humidity() const {
	return humidity;
}
void Biome::set_humidity(Humidity _humidity) {
	ERR_FAIL_INDEX(_humidity, HUMIDITY_COUNT);
	humidity = _humidity;
}
Biome::Temperature Biome::get_temperature() const {
	return temperature;
}
void Biome::set_temperature(Temperature _temperature) {
	ERR_FAIL_INDEX(_temperature, TEMPERATURE_COUNT);
	temperature = _temperature;
}

String Biome::get_biome_name() const {
	return _biome_name;
}
void Biome::set_biome_name(String biome_name) {
	_biome_name = biome_name;
}

void Biome::set_library(Ref<VoxelInstanceLibrary> library) {
	if (library == _library) {
		return;
	}

	_library = library;
}
Ref<VoxelInstanceLibrary> Biome::get_library() const {
	return _library;
}


void Biome::_bind_methods() {

	ClassDB::bind_method(D_METHOD("set_biome_name", "set_biome_name"), &Biome::set_biome_name);
	ClassDB::bind_method(D_METHOD("get_biome_name"), &Biome::get_biome_name);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "biome_name"), "set_biome_name", "get_biome_name");


	ADD_GROUP("Spawnable objects", "");

	ClassDB::bind_method(D_METHOD("set_library", "library"), &Biome::set_library);
	ClassDB::bind_method(D_METHOD("get_library"), &Biome::get_library);

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "library", PROPERTY_HINT_RESOURCE_TYPE, VoxelInstanceLibrary::get_class_static()), "set_library", "get_library");


	ADD_GROUP("Height filters", "");

    ClassDB::bind_method(D_METHOD("set_continentalness", "continentalness"), &Biome::set_continentalness);
	ClassDB::bind_method(D_METHOD("get_continentalness"), &Biome::get_continentalness);
	ClassDB::bind_method(D_METHOD("set_erosion", "erosion"), &Biome::set_erosion);
	ClassDB::bind_method(D_METHOD("get_erosion"), &Biome::get_erosion);
	ClassDB::bind_method(D_METHOD("set_peaks_and_valleys", "peaks_and_valleys"), &Biome::set_peaks_and_valleys);
	ClassDB::bind_method(D_METHOD("get_peaks_and_valleys"), &Biome::get_peaks_and_valleys);

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "continentalness", PROPERTY_HINT_RESOURCE_TYPE, HeightFilter::get_class_static()), "set_continentalness", "get_continentalness");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "erosion", PROPERTY_HINT_RESOURCE_TYPE, HeightFilter::get_class_static()), "set_erosion", "get_erosion");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "peaks_and_valleys", PROPERTY_HINT_RESOURCE_TYPE, HeightFilter::get_class_static()), "set_peaks_and_valleys", "get_peaks_and_valleys");


	ADD_GROUP("Colors", "");

    ClassDB::bind_method(D_METHOD("set_surface_threshold", "new_surface_threshold"), &Biome::set_surface_threshold);
	ClassDB::bind_method(D_METHOD("get_surface_threshold"), &Biome::get_surface_threshold);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "surface_threshold", PROPERTY_HINT_RANGE, String("{0},{1},1").format(varray(0, 100))), "set_surface_threshold", "get_surface_threshold");
	ClassDB::bind_method(D_METHOD("set_surface_gradient", "gradient"), &Biome::set_surface_gradient);
	ClassDB::bind_method(D_METHOD("get_surface_gradient"), &Biome::get_surface_gradient);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "surface_gradient", PROPERTY_HINT_RESOURCE_TYPE, Gradient::get_class_static()), "set_surface_gradient", "get_surface_gradient");
	ClassDB::bind_method(D_METHOD("set_underground_gradient", "gradient"), &Biome::set_underground_gradient);
	ClassDB::bind_method(D_METHOD("get_underground_gradient"), &Biome::get_underground_gradient);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "underground_gradient", PROPERTY_HINT_RESOURCE_TYPE, Gradient::get_class_static()), "set_underground_gradient", "get_underground_gradient");


	ADD_GROUP("Conditions of appearance", "");

//  BIND_ENUM_CONSTANT(HUMIDITY_STICKY);
	BIND_ENUM_CONSTANT(HUMIDITY_HUMID);
//	BIND_ENUM_CONSTANT(HUMIDITY_PLEASANT);
	BIND_ENUM_CONSTANT(HUMIDITY_DRY);
	BIND_ENUM_CONSTANT(HUMIDITY_COUNT);
	ClassDB::bind_method(D_METHOD("set_humidity", "_humidity"), &Biome::set_humidity);
	ClassDB::bind_method(D_METHOD("get_humidity"), &Biome::get_humidity);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "humidity", PROPERTY_HINT_ENUM, "Humid,Dry"), "set_humidity", "get_humidity");
    
    BIND_ENUM_CONSTANT(TEMPERATURE_FREEZING);
//	BIND_ENUM_CONSTANT(TEMPERATURE_COLD);
//	BIND_ENUM_CONSTANT(TEMPERATURE_TEMPERATE);
	BIND_ENUM_CONSTANT(TEMPERATURE_HOT);
//	BIND_ENUM_CONSTANT(TEMPERATURE_BURNING);
	BIND_ENUM_CONSTANT(TEMPERATURE_COUNT);
	ClassDB::bind_method(D_METHOD("set_temperature", "temperature"), &Biome::set_temperature);
	ClassDB::bind_method(D_METHOD("get_temperature"), &Biome::get_temperature);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "temperature", PROPERTY_HINT_ENUM, "Freezing,Hot"), "set_temperature", "get_temperature");

}

} // namespace zylann
