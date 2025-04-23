import { IntersectionView } from "../graphics/IntersectionView";
import { RoadView } from "../graphics/RoadView";
import { INTERSECTION_SNAP_RADIUS } from "./constants";

export function findOrCreateIntersection(x, y) 
{
    const nearby = IntersectionView.findIntersectionAtPoint(x, y);
    if (nearby) return nearby;
    const newIntersection = new IntersectionView(x, y);
    return newIntersection;
}

function doLinesIntersect(a1, a2, b1, b2) 
{
    function ccw(p1, p2, p3) {
        return (p3.y - p1.y) * (p2.x - p1.x) > (p2.y - p1.y) * (p3.x - p1.x);
    }

    return (
        ccw(a1, b1, b2) !== ccw(a2, b1, b2) &&
        ccw(a1, a2, b1) !== ccw(a1, a2, b2)
    );
}

export function isIntersectingExistingRoads(start_intersection, end_intersection) 
{
    for (const road of RoadView.roads) {
        const a1 = road.startIntersection;
        const a2 = road.endIntersection;
        if ((a1 === start_intersection || a1 === end_intersection || a2 === start_intersection || a2 === end_intersection)) continue;
        if (doLinesIntersect(start_intersection, end_intersection, a1, a2)) {
            return true;
        }
    }
    return false;
}


export function split_road_at(x, y)
{
    let road = RoadView.getRoadAtPoint(x, y);
    if (road)
    {
        const closest_point = road.findClosestPointOnRoad(x, y);
        if (!closest_point) 
            return null;

        const dist = Math.hypot(closest_point.x - x, closest_point.y - y);
        if (dist > INTERSECTION_SNAP_RADIUS)
            return null;

        const newIntersection = new IntersectionView(closest.x, closest.y);

        const segmentCount1 = Math.max(1, Math.round(road.segmentCount * closest_point.ratio));
        const segmentCount2 = Math.max(1, road.segmentCount - segmentCount1);

        const road1 = new RoadView(road.startIntersection, newIntersection, segmentCount1);
        const road2 = new RoadView(newIntersection, road.endIntersection, segmentCount2);

        road.delete();

        return [road1, newIntersection, road2];
    }

    return null;
}


export function addRoadByCoords(startX, startY, endX, endY) {
    let dx = endX - startX;
    let dy = endY - startY;

    let new_objects = null;
    let start = IntersectionView.findIntersectionAtPoint(startX, startY);
    if (!start)
    {
        start = RoadView.getRoadAtPoint(startX, startY);
        if (!start)
        {
            new_objects = split_road_at(startX, startY);
            if (new_objects)
                start = new_objects[1];
            else
            {
                start = findIntersectionAtPoint(startX, startY);
                if (!start)
                {
                    start = IntersectionView(startX, startY);
                    new_objects = [start, ];
                }
            }
            
        }
    }

    const length = Math.hypot(dx, dy);

    if (length < MIN_ROAD_LENGTH) {
        console.warn("Cannot add road: too short.");
        return [];
    }

    let end = IntersectionView.findIntersectionAtPoint(endX, endY);
    let new_objects2 = null;
    if (!end)
    {
        end = RoadView.getRoadAtPoint(endX, endY);
        if (!end)
        {
            new_objects2 = split_road_at(endX, endY);
            if (new_objects2)
                end = new_objects2[1];
            else
            {
                end = findIntersectionAtPoint(endX, endY);
                if (!end)
                {
                    end = IntersectionView(endX, endY);
                    new_objects2 = [end, ];
                }
            }
            
        }
    } 

    if (!start.canConnectMore() || !end.canConnectMore()) {
        console.warn("Cannot add road: one of the intersections has max connections.");
        return [];
    }

    if (!start.isAngleAllowed(start, end) || !end.isAngleAllowed(end, start)) {
        console.warn("Cannot add road: angle too small.");
        return [];
    }

    if (isIntersectingExistingRoads(start, end)) {
        console.warn("Cannot add road: intersects existing road.");
        return [];
    }

    const segmentCount = Math.max(1, Math.round(length * SEGMENTS_PER_PIXEL));

    const road = new RoadView(start, end, segmentCount);
    let result = [];
    if (new_objects)
        result.push(...new_objects);
    if (new_objects2)
        result.push(...new_objects2);
    result.push(road);
    return result;
}

