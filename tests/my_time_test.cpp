#include "doctest.h"
#include "utility.hpp"

TEST_CASE("test my_time struct") {
    my_time time(0, 0);
    CHECK_EQ("00:00", time.to_string());

    time += my_time(0, 1);
    CHECK_EQ("00:01", time.to_string());

    time += my_time(0, 59);
    CHECK_EQ("01:00", time.to_string());

    time = time - my_time(1, 0);
    CHECK_EQ("00:00", time.to_string());

    my_time other_time(0, 1);
    CHECK(time < other_time);

    other_time = my_time(0, 0);
    CHECK_FALSE(time < other_time);
    CHECK_FALSE(other_time < time);

    other_time = my_time("00:00");
    CHECK_EQ("00:00", other_time.to_string());

    other_time = my_time(1, 60);
    CHECK_EQ("02:00", other_time.to_string());

    other_time = other_time - my_time(2, 1);
    CHECK_THROWS(static_cast<void>(other_time.to_string()));
}
