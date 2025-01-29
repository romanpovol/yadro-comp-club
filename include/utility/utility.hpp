#ifndef UTILITY_H
#define UTILITY_H

#include <cstdint>
#include <cstdio>
#include <optional>
#include <string>
#include <vector>

struct my_time {
public:
    explicit my_time(const std::string &time);
    explicit my_time(std::int32_t hours, std::int32_t minutes);

    my_time() = default;

    [[nodiscard]] std::string to_string() const;

    [[nodiscard]] std::int32_t minutes() const;
    [[nodiscard]] std::int32_t hours() const;

    [[nodiscard]] std::int32_t rounded_up_number_of_hours() const;

    bool operator<(const my_time &other) const;

    my_time operator-(const my_time &rhs) const;
    my_time operator+(const my_time &rhs) const;

    my_time &operator+=(const my_time &rhs);

private:
    std::int32_t m_minutes;
    std::int32_t m_hours;
};

enum class input_event_type : std::size_t {
    client_arrived = 1,
    client_sat = 2,  // table number is required
    client_waiting = 3,
    client_left = 4,
};

enum class output_event_type : std::size_t {
    client_left_club = 11,
    client_sat_down_at_vacant_table = 12,
    error = 13
};

const std::string ERROR_NOT_OPEN_YET = "NotOpenYet";
const std::string ERROR_YOU_SHALL_NOT_PASS = "YouShallNotPass";
const std::string ERROR_PLACE_IS_BUSY = "PlaceIsBusy";
const std::string ERROR_CLIENT_UNKNOWN = "ClientUnknown";
const std::string ERROR_I_CAN_WAIT_NO_LONGER = "ICanWaitNoLonger!";

#endif  // UTILITY_H
