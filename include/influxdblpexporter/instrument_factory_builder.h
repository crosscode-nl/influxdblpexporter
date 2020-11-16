#ifndef INFLUXDBLPEXPORTER_INSTRUMENT_FACTORY_BUILDER_H
#define INFLUXDBLPEXPORTER_INSTRUMENT_FACTORY_BUILDER_H

#include "influxdblpexporter.h"
#include "line_based_writers.h"
#include "simple_instruments.h"
#include "influxdblptool.h"

namespace crosscode::influxdblpexporter {

    class instrument_factory_with_file_based_influxdb_line_protocol_exporter_builder;

    class instrument_factory_builder {
    protected:
        friend class instrument_factory_with_file_based_influxdb_line_protocol_exporter_builder;
        crosscode::influxdblpexporter::settings settings_;
        std::size_t buffer_size_ = 500;
    public:
        instrument_factory_builder &with_buffer_size(std::size_t buffer_size);
        instrument_factory_builder &with_timestamp_resolution(influxdblptool::timestamp_resolution tr);
        instrument_factory_builder &with_insert_prefix(bool enabled);
        instrument_factory_with_file_based_influxdb_line_protocol_exporter_builder with_file_template(std::string tpl);
    };

    class instrument_factory_with_file_based_influxdb_line_protocol_exporter_builder {
    protected:
        instrument_factory_builder parent_;
        std::string file_template_;
        std::size_t counter_;
    public:
        instrument_factory_with_file_based_influxdb_line_protocol_exporter_builder(instrument_factory_builder parent, std::string tpl);
        instrument_factory_with_file_based_influxdb_line_protocol_exporter_builder &with_buffer_size(std::size_t buffer_size);
        instrument_factory_with_file_based_influxdb_line_protocol_exporter_builder &with_timestamp_resolution(influxdblptool::timestamp_resolution tr);
        instrument_factory_with_file_based_influxdb_line_protocol_exporter_builder &with_insert_prefix(bool enabled);
        instrument_factory_with_file_based_influxdb_line_protocol_exporter_builder &with_initial_counter(std::size_t counter);
        crosscode::simple_instruments::instrument_factory<crosscode::influxdblpexporter::influxdb_line_protocol_exporter<crosscode::line_based_writers::segmented_line_based_file_writer>> build();
        crosscode::simple_instruments::instrument_factory<crosscode::influxdblpexporter::sync_influxdb_line_protocol_exporter<crosscode::line_based_writers::segmented_line_based_file_writer>> build_sync();
    };


}


#endif //INFLUXDBLPEXPORTER_INSTRUMENT_FACTORY_BUILDER_H
