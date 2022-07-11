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

    BIND_ENUM_CONSTANT(HUMIDITY_STICKY);
	BIND_ENUM_CONSTANT(HUMIDITY_HUMID);
	BIND_ENUM_CONSTANT(HUMIDITY_PLEASANT);
	BIND_ENUM_CONSTANT(HUMIDITY_DRY);
	BIND_ENUM_CONSTANT(HUMIDITY_COUNT);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "humidity", PROPERTY_HINT_ENUM, "Sticky,Humid,Pleasant,Dry"), "set_humidity", "get_humidity");
    
    BIND_ENUM_CONSTANT(TEMPERATURE_FREEZING);
	BIND_ENUM_CONSTANT(TEMPERATURE_COLD);
	BIND_ENUM_CONSTANT(TEMPERATURE_TEMPERATE);
	BIND_ENUM_CONSTANT(TEMPERATURE_HOT);
	BIND_ENUM_CONSTANT(TEMPERATURE_BURNING);
	BIND_ENUM_CONSTANT(TEMPERATURE_COUNT);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "temperature", PROPERTY_HINT_ENUM, "Freezing,Cold,Temperate,Hot,Burning"), "set_temperature", "get_temperature");

}

} // namespace zylann
