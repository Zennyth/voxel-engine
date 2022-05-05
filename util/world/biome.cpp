#include "biome.h"

#include "height_map.h"
#include <modules/voxel/util/math/color8.h>

namespace zylann {

Biome::Biome() {}

uint16_t Biome::get_color_at(float offset) {
	return Color8((*_gradient)->get_color_at_offset(offset)).to_u16();
}

bool Biome::is_in_range(float temperature, float moisture) {
	return temperature >= _min_temperature && temperature <= _max_temperature && moisture >= _min_moisture && moisture <= _max_moisture;
}


void Biome::set_continentalness(Ref<HeightMap> continentalness) {
	_continentalness = continentalness;
	RWLockWrite wlock(_parameters_lock);
	_parameters.continentalness = continentalness;
}
Ref<HeightMap> Biome::get_continentalness() const {
	return _continentalness;
}
void Biome::set_erosion(Ref<HeightMap> erosion) {
	_erosion = erosion;
	RWLockWrite wlock(_parameters_lock);
	_parameters.erosion = erosion;
}
Ref<HeightMap> Biome::get_erosion() const {
	return _erosion;
}
void Biome::set_peaks_and_valleys(Ref<HeightMap> peaks_and_valleys) {
	_peaks_and_valleys = peaks_and_valleys;
	RWLockWrite wlock(_parameters_lock);
	_parameters.peaks_and_valleys = peaks_and_valleys;
}
Ref<HeightMap> Biome::get_peaks_and_valleys() const {
	return _peaks_and_valleys;
}

void Biome::set_gradient(Ref<Gradient> gradient) {
	_gradient = gradient;
	RWLockWrite wlock(_parameters_lock);
	_parameters.gradient = gradient;
}
Ref<Gradient> Biome::get_gradient() const {
	return _gradient;
}

void Biome::set_min_temperature(float min_temperature) {
	if (_min_temperature == min_temperature) {
		return;
	}
	_min_temperature = min_temperature;
	emit_changed();
}
float Biome::get_min_temperature() const {
	return _min_temperature;
}
void Biome::set_max_temperature(float max_temperature) {
	if (_max_temperature == max_temperature) {
		return;
	}
	_max_temperature = max_temperature;
	emit_changed();
}
float Biome::get_max_temperature() const {
	return _max_temperature;
}
void Biome::set_min_moisture(float min_moisture) {
	if (_min_moisture == min_moisture) {
		return;
	}
	_min_moisture = min_moisture;
	emit_changed();
}
float Biome::get_min_moisture() const {
	return _min_moisture;
}
void Biome::set_max_moisture(float max_moisture) {
	if (_max_moisture == max_moisture) {
		return;
	}
	_max_moisture = max_moisture;
	emit_changed();
}
float Biome::get_max_moisture() const {
	return _max_moisture;
}
void Biome::set_biome_name(String biome_name) {
	if (_biome_name == biome_name) {
		return;
	}
	_biome_name = biome_name;
	emit_changed();
}
String Biome::get_biome_name() const {
	return _biome_name;
}

void Biome::_bind_methods() {

	ClassDB::bind_method(D_METHOD("set_biome_name", "set_biome_name"), &Biome::set_biome_name);
	ClassDB::bind_method(D_METHOD("get_biome_name"), &Biome::get_biome_name);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "biome_name"), "set_biome_name", "get_biome_name");


	ADD_GROUP("Height maps", "");

    ClassDB::bind_method(D_METHOD("set_continentalness", "continentalness"), &Biome::set_continentalness);
	ClassDB::bind_method(D_METHOD("get_continentalness"), &Biome::get_continentalness);
	ClassDB::bind_method(D_METHOD("set_erosion", "erosion"), &Biome::set_erosion);
	ClassDB::bind_method(D_METHOD("get_erosion"), &Biome::get_erosion);
	ClassDB::bind_method(D_METHOD("set_peaks_and_valleys", "peaks_and_valleys"), &Biome::set_peaks_and_valleys);
	ClassDB::bind_method(D_METHOD("get_peaks_and_valleys"), &Biome::get_peaks_and_valleys);

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "continentalness", PROPERTY_HINT_RESOURCE_TYPE, HeightMap::get_class_static()), "set_continentalness", "get_continentalness");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "erosion", PROPERTY_HINT_RESOURCE_TYPE, HeightMap::get_class_static()), "set_erosion", "get_erosion");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "peaks_and_valleys", PROPERTY_HINT_RESOURCE_TYPE, HeightMap::get_class_static()), "set_peaks_and_valleys", "get_peaks_and_valleys");


	ADD_GROUP("Colors", "");

	ClassDB::bind_method(D_METHOD("set_gradient", "gradient"), &Biome::set_gradient);
	ClassDB::bind_method(D_METHOD("get_gradient"), &Biome::get_gradient);

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "gradient", PROPERTY_HINT_RESOURCE_TYPE, Gradient::get_class_static()), "set_gradient", "get_gradient");


	ADD_GROUP("Conditions of appearance", "");

	ClassDB::bind_method(D_METHOD("set_min_temperature", "min_temperature"), &Biome::set_min_temperature);
	ClassDB::bind_method(D_METHOD("get_min_temperature"), &Biome::get_min_temperature);
	ClassDB::bind_method(D_METHOD("set_max_temperature", "min_temperature"), &Biome::set_max_temperature);
	ClassDB::bind_method(D_METHOD("get_max_temperature"), &Biome::get_max_temperature);
	ClassDB::bind_method(D_METHOD("set_min_moisture", "min_moisture"), &Biome::set_min_moisture);
	ClassDB::bind_method(D_METHOD("get_min_moisture"), &Biome::get_min_moisture);
	ClassDB::bind_method(D_METHOD("set_max_moisture", "min_moisture"), &Biome::set_max_moisture);
	ClassDB::bind_method(D_METHOD("get_max_moisture"), &Biome::get_max_moisture);

	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "min_temperature", PROPERTY_HINT_RANGE, "0.0001,10000.0,0.1,exp"), "set_min_temperature", "get_min_temperature");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "max_temperature", PROPERTY_HINT_RANGE, "0.0001,10000.0,0.1,exp"), "set_max_temperature", "get_max_temperature");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "min_moisture", PROPERTY_HINT_RANGE, "0.0001,10000.0,0.1,exp"), "set_min_moisture", "get_min_moisture");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "max_moisture", PROPERTY_HINT_RANGE, "0.0001,10000.0,0.1,exp"), "set_max_moisture", "get_max_moisture");

}

} // namespace zylann
