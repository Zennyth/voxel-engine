#include "height_filter.h"

#include <scene/resources/curve.h>
#include <core/core_string_names.h>

namespace zylann {

HeightFilter::HeightFilter() {
}

real_t HeightFilter::get_height_at(real_t noise) {
	return _curve.ptr()->interpolate_baked(noise) * _weight;
}


void HeightFilter::set_curve(Ref<Curve> curve) {
	if (_curve == curve) {
		return;
	}
	if (_curve.is_valid()) {
		_curve->disconnect(CoreStringNames::get_singleton()->changed,
				callable_mp(this, &HeightFilter::_on_curve_changed));
	}
	_curve = curve;
	RWLockWrite wlock(_parameters_lock);
	if (_curve.is_valid()) {
		_curve->connect(CoreStringNames::get_singleton()->changed,
				callable_mp(this, &HeightFilter::_on_curve_changed));
		// The Curve resource is not thread-safe so we make a copy of it for use in threads
		_parameters.curve = _curve->duplicate();
		_parameters.curve->bake();
	} else {
		_parameters.curve.unref();
	}
}
Ref<Curve> HeightFilter::get_curve() const {
	return _curve;
}
void HeightFilter::_on_curve_changed() {
	ERR_FAIL_COND(_curve.is_null());
	RWLockWrite wlock(_parameters_lock);
	_parameters.curve = _curve->duplicate();
	_parameters.curve->bake();
}

void HeightFilter::set_weight(float weight) {
	if (_weight == weight) {
		return;
	}
	_weight = weight;
	emit_changed();
}
float HeightFilter::get_weight() const {
	return _weight;
}


void HeightFilter::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_curve", "curve"), &HeightFilter::set_curve);
	ClassDB::bind_method(D_METHOD("get_curve"), &HeightFilter::get_curve);

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "curve", PROPERTY_HINT_RESOURCE_TYPE, "Curve"), "set_curve", "get_curve");


    ClassDB::bind_method(D_METHOD("set_weight", "weight"), &HeightFilter::set_weight);
	ClassDB::bind_method(D_METHOD("get_weight"), &HeightFilter::get_weight);

    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "weight", PROPERTY_HINT_RANGE, "0.0001,10000.0,0.1,exp"), "set_weight", "get_weight");
}

} // namespace zylann
