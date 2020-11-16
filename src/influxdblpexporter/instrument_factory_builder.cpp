#include "influxdblpexporter/instrument_factory_builder.h"

namespace crosscode::influxdblpexporter {

    instrument_factory_builder& instrument_factory_builder::with_buffer_size(std::size_t buffer_size) {
        buffer_size_ = buffer_size;
        return *this;
    }

    instrument_factory_builder& instrument_factory_builder::with_timestamp_resolution(influxdblptool::timestamp_resolution tr) {
        settings_.timestamp_resolution = tr;
        return *this;
    }

    instrument_factory_builder& instrument_factory_builder::with_insert_prefix(bool enabled) {
        settings_.insert_prefix = enabled;
        return *this;
    }

    instrument_factory_with_file_based_influxdb_line_protocol_exporter_builder instrument_factory_builder::with_file_template(std::string tpl)
    {
        return instrument_factory_with_file_based_influxdb_line_protocol_exporter_builder{*this,std::move(tpl)};
    }

    instrument_factory_with_file_based_influxdb_line_protocol_exporter_builder::instrument_factory_with_file_based_influxdb_line_protocol_exporter_builder(instrument_factory_builder parent, std::string tpl) : parent_{parent}, file_template_{std::move(tpl)}, counter_{0} {}

    instrument_factory_with_file_based_influxdb_line_protocol_exporter_builder& instrument_factory_with_file_based_influxdb_line_protocol_exporter_builder::with_buffer_size(std::size_t buffer_size) {
        parent_.buffer_size_ = buffer_size;
        return *this;
    }

    instrument_factory_with_file_based_influxdb_line_protocol_exporter_builder& instrument_factory_with_file_based_influxdb_line_protocol_exporter_builder::with_timestamp_resolution(influxdblptool::timestamp_resolution tr) {
        parent_.settings_.timestamp_resolution = tr;
        return *this;
    }

    instrument_factory_with_file_based_influxdb_line_protocol_exporter_builder& instrument_factory_with_file_based_influxdb_line_protocol_exporter_builder::with_insert_prefix(bool enabled) {
        parent_.settings_.insert_prefix = enabled;
        return *this;
    }

    instrument_factory_with_file_based_influxdb_line_protocol_exporter_builder& instrument_factory_with_file_based_influxdb_line_protocol_exporter_builder::with_initial_counter(std::size_t counter) {
        counter_ = counter;
        return *this;
    }

    crosscode::simple_instruments::instrument_factory<crosscode::influxdblpexporter::influxdb_line_protocol_exporter<crosscode::line_based_writers::segmented_line_based_file_writer>> instrument_factory_with_file_based_influxdb_line_protocol_exporter_builder::build() {
        return crosscode::simple_instruments::instrument_factory{crosscode::influxdblpexporter::influxdb_line_protocol_exporter<crosscode::line_based_writers::segmented_line_based_file_writer>{parent_.settings_,parent_.buffer_size_,file_template_,counter_}};
    }

    crosscode::simple_instruments::instrument_factory<crosscode::influxdblpexporter::sync_influxdb_line_protocol_exporter<crosscode::line_based_writers::segmented_line_based_file_writer>> instrument_factory_with_file_based_influxdb_line_protocol_exporter_builder::build_sync() {
        return crosscode::simple_instruments::instrument_factory{crosscode::influxdblpexporter::sync_influxdb_line_protocol_exporter<crosscode::line_based_writers::segmented_line_based_file_writer>{parent_.settings_,parent_.buffer_size_,file_template_,counter_}};
    }

}