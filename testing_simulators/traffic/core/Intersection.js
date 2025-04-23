import { ObserverSubject } from "../observers/Observer";
import { INTERSECTION_SNAP_RADIUS, MIN_ANGLE_DEGREES } from "../utils/constants";

const MAX_CONNECTIONS_PER_INTERSECTION = 4;
const INTERSETION_IS_CREATED = 0x01;
const INTERSETION_IS_DELETED = 0x02;

export class Intersection extends ObserverSubject {

    static intersections = new Set();
    constructor(x, y) {
        this.x = x;
        this.y = y;
        this.connectedRoads = [];

        Intersection.intersections.add(this);
        this.notify(this, INTERSETION_IS_CREATED);
    }

    radius()
    {
        return this.connectedRoads.length <= 2 ? 10 : 14;
    }

    canConnectMore() 
    {
        return this.connectedRoads.length < MAX_CONNECTIONS_PER_INTERSECTION;
    }

    connectRoad(road) 
    {
        if (this.connectedRoads.includes(road)) 
            return;
        if (this.canConnectMore()) 
        {
            this.connectedRoads.push(road);
        } 
        else 
        {
            throw new Error("Intersection at (" + this.x + ", " + this.y + ") has reached max connections.");
        }
    }

    disconnectRoad(road) 
    {
        const index = this.connectedRoads.indexOf(road);
        if (index !== -1) 
        {
            this.connectedRoads.splice(index, 1);
            if (this.connectedRoads.length == 0)
                this.delete();
        }
    }

    delete()
    {
        Intersection.intersections.delete(this);
        this.notify(this, INTERSETION_IS_DELETED);
    }

    getConnectedLanes()
    {
        let lanes = [];
        for (let road of this.connectedRoads)
        {
            lanes = [...lanes, ...road.getLanes()];
        }
        return lanes;
    }

    canTurnAround() 
    {
        return this.connectedRoads.length <= 1;
    }

    getLanesAvailableFrom(incoming_lane)
    {
        if (incoming_lane.isOutgoingFrom(this))
            return [];

        if (incoming_lane.road.startIntersection != this && incoming_lane.road.endIntersection != this)
            return [];

        let available_lanes = [];

        for (let lane of this.getConnectedLanes())
        {
            if (lane.isOutgoingFrom(this) && lane.road != incoming_lane.road)
                available_lanes.push(lane);

            if (lane.isOutgoingFrom(this) && this.canTurnAround())
                available_lanes.push(lane);
        }

        return available_lanes;
    }

    equals(other)
    {
        return this == other || Math.abs(other.x - this.x) < 0.1 && Math.abs(other.y - this.y) < 0.1;
    }

    isAngleAllowed(start_point, end_point) {
        const newVector = [end_point.x - start_point.x, end_point.y - start_point.y];
        for (const road of this.connectedRoads) {
            const other = road.startIntersection === this ? road.endIntersection : road.startIntersection;
            const existingVector = [other.x - this.x, other.y - this.y];
            const dot = newVector[0] * existingVector[0] + newVector[1] * existingVector[1];
            const len1 = Math.hypot(...newVector);
            const len2 = Math.hypot(...existingVector);
            const angle = Math.acos(dot / (len1 * len2)) * 180 / Math.PI;
            if (angle < MIN_ANGLE_DEGREES) {
                return false;
            }
        }
        return true;
    }

    static findIntersectionAtPoint(x, y) {
        for (let i of Intersection.intersections)
        {
            const dx = i.x - x;
            const dy = i.y - y;
            if (Math.hypot(dx, dy) <= INTERSECTION_SNAP_RADIUS)
                return i;
        }
        return null;
    }

    
    // TODO: get out Lanes
    // TODO: get in Lanes
}