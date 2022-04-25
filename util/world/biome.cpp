#include "biome.h"

#include "height_map.h"

namespace zylann {

Biome::Biome() {}

void Biome::set_continentalness(Ref<HeightMap> continentalness) {
	_continentalness = continentalness;
	RWLockWrite wlock(_parameters_lock);
	_parameters.continentalness = continentalness;
}
Ref<HeightMap> Biome::get_continentalness() const {
	return _continentalness;
}

void Biome::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_continentalness", "continentalness"), &Biome::set_continentalness);
	ClassDB::bind_method(D_METHOD("get_continentalness"), &Biome::get_continentalness);

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "continentalness", PROPERTY_HINT_RESOURCE_TYPE, HeightMap::get_class_static()), "set_continentalness", "get_continentalness");
}

} // namespace zylann
