#include "influxdblpexporter/metadata_builder.h"

namespace crosscode::influxdblpexporter {

    metadata_with_measurement_builder metadata_builder::with_measurement(std::string measurement) {
        return metadata_with_measurement_builder{std::move(measurement)};
    }

    metadata_with_measurement_builder::metadata_with_measurement_builder(std::string measurement) : measurement_{std::move(measurement)} {}

    metadata_with_measurement_builder& metadata_with_measurement_builder::with_measurement(std::string measurement) {
        measurement_ = std::move(measurement);
        return *this;
    }

    metadata_with_field_key_builder metadata_with_measurement_builder::with_field_key(std::string field_key) {
        return metadata_with_field_key_builder{measurement_,std::move(field_key)};
    }

    metadata_with_field_key_builder::metadata_with_field_key_builder(std::string measurement, std::string field_key) : measurement_{std::move(measurement)}, field_key_{std::move(field_key)} , emit_initial_{true} {}

    metadata_with_field_key_builder& metadata_with_field_key_builder::with_measurement(std::string measurement) {
        measurement_ = std::move(measurement);
        return *this;
    }

    metadata_with_field_key_builder& metadata_with_field_key_builder::with_field_key(std::string field_key) {
        field_key_ = std::move(field_key);
        return *this;
    }

    metadata_with_field_key_builder& metadata_with_field_key_builder::with_tag(std::string key, std::string value) {
        tags_.emplace(std::move(key),std::move(value));
        return *this;
    }

    metadata_with_field_key_builder& metadata_with_field_key_builder::with_emit_initial(bool emit_initial) {
        emit_initial_ = emit_initial;
        return *this;
    }

    metadata metadata_with_field_key_builder::build() {
        return metadata{measurement_,field_key_, tags_, fields_, emit_initial_};
    }
}