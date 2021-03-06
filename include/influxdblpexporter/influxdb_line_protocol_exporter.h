#ifndef INFLUXDBLPEXPORTER_INFLUXDBLINEPROTOCOLEXPORTER_H
#define INFLUXDBLPEXPORTER_INFLUXDBLINEPROTOCOLEXPORTER_H
#include <ostream>
#include <influxdblptool.h>
#include <algorithm>
#include <utility>
#include <mutex>
#include <memory>

namespace crosscode::influxdblpexporter {

    template <typename Tpoint>
    class metadata_template {
    public:
        using point_type = Tpoint;
    private:
        influxdblptool::measurement measurement_;
        std::string field_key_;
        influxdblptool::tags_map tags_;
        influxdblptool::fields_map fields_;
        bool emit_initial_;
    public:
        template <typename Ttags>
        metadata_template(std::string measurement, std::string field_key, Ttags&& tags)
        : measurement_{std::move(measurement)}, field_key_{std::move(field_key)}, tags_{std::forward<Ttags>(tags)}, fields_{}, emit_initial_{true} {
            influxdblptool::validators::throw_when_field_key_invalid(field_key_);
        }

        template <typename Ttags, typename Tfields>
        metadata_template(std::string measurement, std::string field_key, Ttags&& tags, Tfields&& fields)
        : measurement_{std::move(measurement)}, field_key_{std::move(field_key)}, tags_{std::forward<Ttags>(tags)}, fields_{std::forward<Tfields>(fields)}, emit_initial_{true} {
            influxdblptool::validators::throw_when_field_key_invalid(field_key_);
        }

        template <typename Ttags, typename Tfields>
        metadata_template(std::string measurement, std::string field_key, Ttags&& tags, Tfields&& fields, bool emit_initial)
        : measurement_{std::move(measurement)}, field_key_{std::move(field_key)}, tags_{std::forward<Ttags>(tags)}, fields_{std::forward<Tfields>(fields)}, emit_initial_{emit_initial} {
            influxdblptool::validators::throw_when_field_key_invalid(field_key_);
        }

        metadata_template(std::string measurement, std::string field_key)
                : measurement_{std::move(measurement)}, field_key_{std::move(field_key)}, tags_{}, emit_initial_{true} {
            influxdblptool::validators::throw_when_field_key_invalid(field_key_);
        }

        template<typename Tvalue>
        point_type make_point(Tvalue&& v) const {
            point_type pt{measurement_,influxdblptool::field{field_key_,std::forward<Tvalue>(v)}};
            auto stream_to_point = [&pt](const auto &item){
                pt << item;
            };
            std::for_each(begin(tags_),end(tags_),stream_to_point);
            std::for_each(begin(fields_),end(fields_),stream_to_point);
            return pt;
        }

        [[nodiscard]] bool emit_initial() const {
            return emit_initial_;
        }
    };

    using metadata = metadata_template<influxdblptool::point>;

    struct settings {
        influxdblptool::timestamp_resolution timestamp_resolution = influxdblptool::timestamp_resolution::nanoseconds;
        bool insert_prefix = false;
    };

    template <typename Tmetadata>
    class emitter {
        public:
        using metadata_type = Tmetadata;
        template <typename Tvalue, typename Tline_handler>
        void write(Tvalue&& v, const metadata_type& md, Tline_handler& line_handler, const settings& settings) const {
            auto pt = md.make_point(std::forward<Tvalue>(v));
            pt.set_current_timestamp_resolution(settings.timestamp_resolution);
            if (settings.insert_prefix) {
                pt.set_prefix(influxdblptool::insert_prefix);
            }
            line_handler.write(influxdblptool::to_string(pt));
        }
    };


    template <typename Tmetadata>
    class sync_emitter {
        private:
        emitter<Tmetadata> nts_emit_;
        std::unique_ptr<std::mutex> mutex_;
        public:
        using metadata_type = Tmetadata;
        template <typename Tvalue, typename Tline_handler>
        void write(Tvalue&& v, const metadata_type& md, Tline_handler& lh, const settings& settings) const {
            std::lock_guard<std::mutex> lock(*mutex_);
            nts_emit_.write(std::forward<Tvalue>(v), md, lh, settings);
        }
    };

    template <typename Tline_writer, typename Tmetadata, typename Temitter>
    class influxdb_line_protocol_exporter_template {
    public:
        using metadata_type = Tmetadata;
    private:
        Tline_writer line_writer_;
        Temitter emitter_;
        settings settings_;
    public:
        influxdb_line_protocol_exporter_template(influxdb_line_protocol_exporter_template<Tline_writer,Tmetadata,Temitter>&& rhs) noexcept :
                line_writer_{std::move(rhs.line_writer_)},
                emitter_{std::move(rhs.emitter_)},
                settings_{std::move(rhs.settings_)} {}

        template<typename ...Args>
        explicit influxdb_line_protocol_exporter_template(settings settings, Args... args)
        : line_writer_{std::forward<Args>(args)...},
          settings_{settings} {}

        template <typename Tvalue>
        void emit_init(Tvalue&& v, const metadata_type& md) {
            if (md.emit_initial()) {
                emit(std::forward<Tvalue>(v), md);
            }
        }

        template <typename Tvalue>
        void emit(Tvalue&& v, const metadata_type& md) {
            emitter_.write(std::forward<Tvalue>(v), md, line_writer_, settings_);
        }

    };

    template <typename Tline_handler>
    using influxdb_line_protocol_exporter = influxdb_line_protocol_exporter_template<Tline_handler, metadata, emitter<metadata>>;

    template <typename Tline_handler>
    using sync_influxdb_line_protocol_exporter = influxdb_line_protocol_exporter_template<Tline_handler, metadata, sync_emitter<metadata>>;


}

#endif //INFLUXDBLPEXPORTER_INFLUXDBLINEPROTOCOLEXPORTER_H
