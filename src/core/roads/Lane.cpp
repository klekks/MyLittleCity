#include "Lane.hpp"
#include <mutex>

static std::mutex lane_segments_mutex;

template <size_t number_of_segments>
Lane<number_of_segments>::Lane(const Road& road, LaneDirection direction)
: road(road), direction(direction)
{}

template <size_t number_of_segments>
bool Lane<number_of_segments>::try_acquire_segment(size_t segment_idx)
{
    std::lock_guard guard(lane_segments_mutex);
    if (lane_segments[segment_idx])
        return false;
    
    lane_segments.set(segment_idx);
    return true;
}


template <size_t number_of_segments>
void Lane<number_of_segments>::release_segment(size_t segment_idx)
{
    std::lock_guard guard(lane_segments_mutex);
    lane_segments.reset(segment_idx);
}


template <size_t number_of_segments>
float Lane<number_of_segments>::length() const
{
    return road.length();
}


