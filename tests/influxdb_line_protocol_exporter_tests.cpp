#include "doctest.h"
#include "influxdblpexporter.h"
#include "influxdblptool.h"
#include "simple_instruments.h"

namespace ift = influxdblptool;
namespace cie = crosscode::influxdblpexporter;
namespace csi = crosscode::simple_instruments;

using namespace std::literals;

std::chrono::system_clock::time_point fake_now() {
    return std::chrono::system_clock::time_point{1s};
}

class line_writer {
private:
    std::ostream* ostream_;
public:
    explicit line_writer(std::ostream* os) : ostream_{os} {}
    template <typename Tline>
    void write(Tline&& line) {
        (*ostream_) << std::forward<Tline>(line) << "\n";
    }
};

TEST_SUITE("influxdb_line_protocol_exporter_tests") {
    TEST_CASE("Can create instrument_factory with influxdb_line_protocol_exporter, default timestamp settings enabled") {
        using fake_now_metadata = cie::metadata_template<ift::point_custom_timestamp<fake_now>>;
        using fake_now_influxdb_line_protocol_exporter = cie::influxdb_line_protocol_exporter_template<line_writer, fake_now_metadata, cie::emitter<fake_now_metadata>>;
        std::stringstream ss;
        csi::instrument_factory factory(fake_now_influxdb_line_protocol_exporter{line_writer{&ss}});
        SUBCASE("Can create monotonic int64_t counter with measurement name: test with field_name test_field.") {
            auto mono_counter = factory.make_atomic_bidirectional_counter<int64_t>({"test","test_field"});
            SUBCASE("Output is send to ostream correctly.") {
                REQUIRE("test test_field=0i 1000000000\n"==ss.str());
            }
        }
    }
    TEST_CASE("Can create instrument_factory with sync_influxdb_line_protocol_exporter and enabled INSERT prefix.") {
        std::stringstream ss;
        csi::instrument_factory factory{cie::sync_influxdb_line_protocol_exporter<line_writer>(line_writer{&ss}, ift::timestamp_resolution::none, true)};
        SUBCASE("Can create monotonic int64_t counter with measurement name: test with field_name test_field.") {
            auto mono_counter = factory.make_atomic_bidirectional_counter<int64_t>({"test","test_field"});
            SUBCASE("Output is send to ostream correctly.") {
                REQUIRE("INSERT test test_field=0i\n"==ss.str());
            }
        }
    }
    TEST_CASE("Can create instrument_factory with influxdb_line_protocol_exporter and enabled INSERT prefix.") {
        std::stringstream ss;
        csi::instrument_factory factory{cie::influxdb_line_protocol_exporter<line_writer>{line_writer{&ss}, ift::timestamp_resolution::none, true}};
        SUBCASE("Can create monotonic int64_t counter with measurement name: test with field_name test_field.") {
            auto mono_counter = factory.make_atomic_bidirectional_counter<int64_t>({"test","test_field"});
            SUBCASE("Output is send to ostream correctly.") {
                REQUIRE("INSERT test test_field=0i\n"==ss.str());
            }
        }
    }
    TEST_CASE("Can create instrument_factory with influxdb_line_protocol_exporter.") {
        std::stringstream ss;
        csi::instrument_factory factory{cie::influxdb_line_protocol_exporter<line_writer>{line_writer{&ss}, ift::timestamp_resolution::none}};
        SUBCASE("Can create monotonic int64_t counter with measurement name: test with field_name test_field.") {
            auto mono_counter = factory.make_atomic_bidirectional_counter<int64_t>({"test","test_field"});
            REQUIRE(0==mono_counter.value());
            SUBCASE("Output is send to ostream correctly.") {
                REQUIRE("test test_field=0i\n"==ss.str());
            }
            SUBCASE("Add increments value") {
                mono_counter.add();
                REQUIRE(1==mono_counter.value());
                SUBCASE("Output is send to ostream correctly.") {
                    REQUIRE("test test_field=0i\ntest test_field=1i\n" == ss.str());
                }
            }
        }
        SUBCASE("Can create bidirectional counter uint16_t counter with measurement name: test with field_name test_field and tags tag1=tagv1,tag2=tagv2.") {
            factory.make_atomic_bidirectional_counter<uint16_t>(
            {"test","test_field",
               ift::tags_map{ift::tag{"tag1","tagv1"},ift::tag{"tag2","tagv2"}}}
            );
            SUBCASE("Output is send to ostream correctly.") {
                REQUIRE("test,tag1=tagv1,tag2=tagv2 test_field=0u\n" == ss.str());
            }
        }
        SUBCASE("Can create bidirectional counter uint16_t counter with measurement name: test with field_name test_field and custom fields.") {
            factory.make_atomic_bidirectional_counter<uint16_t>(
        {"test","test_field",ift::tags_map {},ift::fields_map{ift::field{"field1","fieldv1"},ift::field{"field2",true}}}
            );
            SUBCASE("Output is send to ostream correctly.") {
                REQUIRE("test field1=\"fieldv1\",field2=t,test_field=0u\n" == ss.str());
            }
        }
        SUBCASE("Can create bidirectional counter uint16_t counter with measurement name: test with field_name test_field and custom fields, and emit initial disabled") {
            auto bi_counter = factory.make_atomic_bidirectional_counter<uint16_t>(
                    {"test","test_field",ift::tags_map{},ift::fields_map{},false}
            );
            REQUIRE(0==bi_counter.value());
            SUBCASE("ostream status empty.") {
                REQUIRE(""==ss.str());
                SUBCASE("Add increments value") {
                    bi_counter.add();
                    REQUIRE(1==bi_counter.value());
                    SUBCASE("Output is send to ostream correctly.") {
                        REQUIRE("test test_field=1u\n" == ss.str());
                    }
                }
            }
        }
    }
}
