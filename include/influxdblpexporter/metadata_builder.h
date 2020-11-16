#ifndef INFLUXDBLPEXPORTER_METADATA_BUILDER_H
#define INFLUXDBLPEXPORTER_METADATA_BUILDER_H
#include "influxdblptool.h"
#include "influxdb_line_protocol_exporter.h"

namespace crosscode::influxdblpexporter {

class metadata_with_measurement_builder;
struct metadata_builder {
    metadata_with_measurement_builder with_measurement(std::string measurement);
};

class metadata_with_field_key_builder;
class metadata_with_measurement_builder {
    std::string measurement_;
public:
    explicit metadata_with_measurement_builder(std::string measurement);
    metadata_with_measurement_builder& with_measurement(std::string measurement);
    metadata_with_field_key_builder with_field_key(std::string field_key);
};

class metadata_with_field_key_builder {
    std::string measurement_;
    std::string field_key_;
    influxdblptool::tags_map tags_;
    influxdblptool::fields_map fields_;
    bool emit_initial_;
public:
    metadata_with_field_key_builder(std::string measurement, std::string field_key);
    metadata_with_field_key_builder& with_measurement(std::string measurement);
    metadata_with_field_key_builder& with_field_key(std::string field_key);
    metadata_with_field_key_builder& with_tag(std::string key, std::string value);
    template <typename ...Args>
    metadata_with_field_key_builder& with_field(Args... args){
        fields_.emplace(std::forward<Args>(args)...);
        return *this;
    }
    metadata_with_field_key_builder& with_emit_initial(bool emit_initial);
    metadata build();
};

}

#endif //INFLUXDBLPEXPORTER_METADATA_BUILDER_H
