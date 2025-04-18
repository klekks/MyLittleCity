import { Lane } from '../core/Lane.js';
import { MinHeap } from './minHeap.js';

export function findDirectedRoadPath(fromRoad, fromDir, toRoad, toDir, options = {}) {
    const {
      allowTurnarounds = true,
      preferExact = true
    } = options;
  
    const tryPaths = [
      [fromDir, toDir],
    ];
  
    if (allowTurnarounds) {
      tryPaths.push(
        [-fromDir, toDir],
        [fromDir, -toDir],
        [-fromDir, -toDir]
      );
    }
  
    for (const [fDir, tDir] of tryPaths) {
      const fromLane = new Lane(fromRoad, fDir);
      const toLane = new Lane(toRoad, tDir);
      const path = findDirectedRoadPathRaw(fromLane, toLane);
      if (path.length > 0) {
        return path;
      }
    }
  
    return [];
  }
  

function findDirectedRoadPathRaw(fromLane, toLane) {

  const visited = new Set();
  const queue = [{
      lane: fromLane,
      path: [],
      distance: 0,
  }];

  while (queue.length > 0) {
      // находим кратчайший
      queue.sort((a, b) => a.distance - b.distance);
      const { lane, path, distance } = queue.shift();

      const key = lane.getId();
      if (visited.has(key)) continue;
      visited.add(key);

      const extendedPath = [...path, lane];

      if (lane.equals(toLane)) {
        //console.log(findDirectedRoadPathDijkstra(fromRoad, fromDir, toRoad, toDir).map((e) => {return e.road.id; }));
        return extendedPath;
      }

      const currentIntersection = lane.getEndIntersection();

      for (const nextRoad of currentIntersection.connectedRoads) {
          for (const nextLane of nextRoad.getLanes()) {
              const nextKey = nextLane.getId();
              if (visited.has(nextKey)) continue;

              const sameRoadReverse =
                  nextLane.road === lane.road && nextLane.direction !== lane.direction;

              const isTurningAround = sameRoadReverse;

              // запрещаем разворот, если он НЕ в тупике
              if (isTurningAround && !lane.canTurnAround()) continue;

              // можем двигаться только если выезд совпадает
              if (!nextLane.isOutgoingFrom(currentIntersection)) continue;

              const segmentLength = Math.hypot(
                  nextLane.road.endIntersection.x - nextLane.road.startIntersection.x,
                  nextLane.road.endIntersection.y - nextLane.road.startIntersection.y
              );

              queue.push({
                  lane: nextLane,
                  path: extendedPath,
                  distance: distance + segmentLength,
              });
          }
      }
  }

  return [];
}


function findDirectedRoadPathDijkstra(fromRoad, fromDir, toRoad, toDir)
{
    const fromLane = new Lane(fromRoad, fromDir);
    const toLane = new Lane(toRoad, toDir);

    const visited = new Set();
    
    let INF = 0xFFFFFFFFFFFF;

    let queue = new MinHeap();
    queue.insert({lane: fromLane, dist: 0, path: [fromLane, ]}, 0);
    
    let distances = new Map();
    let paths = new Map();
    const get = (id) => distances.get(id) ?? INF;

    while (!queue.isEmpty())
    {
        // let tmp = queue.remove().element;
        // console.log(tmp);
        let {lane: lane, dist: dist_to_lane, path: path} = queue.remove().element;

        if (dist_to_lane > get(lane.getId()))
            continue;

        const currentIntersection = lane.getEndIntersection();

        for (const nextRoad of currentIntersection.connectedRoads) 
        {
            for (const nextLane of nextRoad.getLanes())
            {
                const isTurningAround =
                    nextLane.road === lane.road && nextLane.direction !== lane.direction;

                if (isTurningAround && !lane.canTurnAround()) continue;

                if (!nextLane.isOutgoingFrom(currentIntersection)) continue;

                let new_dist = nextLane.getLength() + dist_to_lane;
                if (get(nextLane.getId()) > new_dist)
                {
                    path = [...path, nextLane];
                    distances.set(nextLane.getId(), new_dist);
                    queue.insert({lane: nextLane, dist: new_dist, path: path}, new_dist);
                    paths[lane.getId()] = path;
                }
            }
        }
    }

    return paths[toLane.getId()];
}