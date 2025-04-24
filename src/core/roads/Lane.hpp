#pragma once

#include <atomic>
#include <bitset>

#include "Road.hpp"

enum LaneDirection : bool 
{
    FORWARD_DIRECTION,
    BACKWARD_DIRECTION
};

template <size_t number_of_segments>
class Lane
{
    const Road& road;
    LaneDirection direction;

    std::bitset<number_of_segments> lane_segments;

public:
    Lane(const Road& road, LaneDirection direction);

    bool try_acquire_segment(size_t segment_idx);
    void release_segment(size_t segment_idx);

    float length() const;
};