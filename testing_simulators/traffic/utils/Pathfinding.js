import { Lane } from '../core/Lane.js';
import { MinHeap } from './minHeap.js';


export function find_path(fromLane, fromLaneRatio, toLane, toLaneRatio)
{
    if (fromLane.equals(toLane) && ((toLaneRatio - fromLaneRatio) / toLane.direction > 0))
    {
        return [fromLane, ];
    }
    else if (fromLane.equals(toLane))
    {
        for (const nextLane of fromLane.getEndIntersection().getLanesAvailableFrom(fromLane))
            return [fromLane, ...find_path(nextLane, nextLane.direction === 1 ? 0 : 1, toLane, toLaneRatio)];
        return [];
    }

    const visited = new Set();
    const queue = [{
        lane: fromLane,
        path: [fromLane, ],
        distance: 0,
    }];

    let distances = {};
    let paths = {};
    distances[fromLane.getId()] = 0;
    paths[fromLane.getId()] = [fromLane, ];
    

    while (queue.length > 0) {
        queue.sort((a, b) => a.distance - b.distance);
        const { lane, path, distance } = queue.shift();

        if (!distances.hasOwnProperty(lane.getId()))
            distances[lane.getId] = Infinity;

        if (distance > distances[lane.getId()])
            continue;

        for (const nextLane of lane.getEndIntersection().getLanesAvailableFrom(lane))
        {
            if (!distances.hasOwnProperty(nextLane.getId()))
                distances[nextLane.getId()] = Infinity;

            if (distances[nextLane.getId()] > distances[lane.getId()] + nextLane.getLength())
            {
                distances[nextLane.getId()] = distances[lane.getId()] + nextLane.getLength();
                paths[nextLane.getId()] = [...path, nextLane];
                queue.push({
                    lane: nextLane,
                    path: [...path, nextLane],
                    distance: distance + nextLane.getLength(),
                });
            }
        }
    }

    if (paths[toLane.getId()])
        return paths[toLane.getId()];
    return [];
}
