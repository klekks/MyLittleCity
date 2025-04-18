
const MAX_CONNECTIONS_PER_INTERSECTION = 4;

export class Intersection {
    constructor(x, y) {
        this.x = x;
        this.y = y;
        this.connectedRoads = [];
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
        }
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

    // TODO: get out Lanes
    // TODO: get in Lanes
}