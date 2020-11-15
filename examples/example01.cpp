#include <iostream>
#include "influxdblpexporter.h"
#include "line_based_writers.h"
#include "simple_instruments.h"
#include "influxdblptool.h"
#include <thread>

namespace cie = crosscode::influxdblpexporter;
namespace clbw = crosscode::line_based_writers;
namespace csi = crosscode::simple_instruments;
namespace ift = influxdblptool;

using exporter = cie::influxdb_line_protocol_exporter<clbw::segmented_line_based_file_writer>;

using namespace std::literals;

int main() {
    csi::instrument_factory factory{exporter{cie::settings{},std::size_t{5},"/tmp/output_%NUM%.txt"}};
    auto counter = factory.make_atomic_monotonic_counter<std::uint64_t>({"test","test_field",ift::tags_map{ift::tag{"tag1","tagv1"},ift::tag{"tag2","tagv2"}}});
    for (int i = 0; i<20; i++) {
        counter.add();
        std::this_thread::sleep_for(1s);
    }
}
