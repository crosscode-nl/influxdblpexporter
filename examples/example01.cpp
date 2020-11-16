#include <iostream>
#include <thread>
#include <influxdblpexporter/instrument_factory_builder.h>




int main() {

    using namespace crosscode::influxdblpexporter;
    using namespace std::literals;

    auto instrument_factory = instrument_factory_builder()
            .with_buffer_size(5)
            .with_file_template("/tmp/output_%NUM%.txt")
            .build();

    auto metadata = metadata_builder()
            .with_measurement("test")
            .with_field_key("test_field")
            .with_tag("tag1","tagv1")
            .with_tag("tag2","tagv2")
            .with_field("mybooolfield",true)
            .with_field("mytextfield","text")
            .build();

    auto counter = instrument_factory.make_atomic_monotonic_counter<std::uint64_t>(metadata);
    for (int i = 0; i<20; i++) {
        counter.add();
        std::this_thread::sleep_for(1s);
    }
}

