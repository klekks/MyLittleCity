import { Lane } from "./Lane.js"
import { INTERSECTION_ENTRY_MARGIN, INTERSECTION_SNAP_RADIUS } from "../utils/constants.js"
import { ObserverSubject } from "../observers/Observer.js";

class Segment {
    constructor() {
        this.occupied = false;
        this.occupier = null;
    }

    isOccupied() {
        return this.occupied;
    }

    occupy(occupier) {
        this.occupied_by = occupier;
        this.occupied = true;
    }

    release() {
        this.occupied = false;
    }
}

const ROAD_IS_CREATED = 0x01;
const ROAD_IS_DELETED = 0x02;
const ROAD_IS_ACCEPTED = 0x03;

const ROAD_SNAP_RADIUS = INTERSECTION_SNAP_RADIUS;

export class Road extends ObserverSubject {
    static idCounter = 0;
    static roads = new Set();
    constructor(startIntersection, endIntersection, segmentCount = 10) {
        this.id = Road.idCounter++;

        this.startIntersection = startIntersection;
        this.endIntersection = endIntersection;
        this.segmentCount = segmentCount;

        this.forwardSegments = Array.from({ length: segmentCount }, () => new Segment());
        this.backwardSegments = Array.from({ length: segmentCount }, () => new Segment());

        this.lanes = [new Lane(this, 1), new Lane(this, -1)];

        this.startIntersection.connectRoad(this);
        this.endIntersection.connectRoad(this);

        this.is_deleted = false;
        Road.roads.add(this);
        this.notify(this, ROAD_IS_CREATED);
    }

    getLanes() {
        return this.lanes;
    }

    getPointAtRatio(ratio, clampToEntry = false) {
        let adjustedRatio = ratio;
        if (clampToEntry) {
            const margin = INTERSECTION_ENTRY_MARGIN;
            adjustedRatio = Math.min(1 - margin, Math.max(margin, ratio));
        }

        const x = this.startIntersection.x + (this.endIntersection.x - this.startIntersection.x) * adjustedRatio;
        const y = this.startIntersection.y + (this.endIntersection.y - this.startIntersection.y) * adjustedRatio;
        return { x, y };
    }

    findClosestPointOnRoad(x, y) {
        const x1 = this.startIntersection.x;
        const y1 = this.startIntersection.y;
        const x2 = this.endIntersection.x;
        const y2 = this.endIntersection.y;

        const dx = x2 - x1;
        const dy = y2 - y1;
        const lengthSq = dx * dx + dy * dy;

        if (lengthSq === 0) return null;

        let t = ((x - x1) * dx + (y - y1) * dy) / lengthSq;
        t = Math.max(0, Math.min(1, t));

        const px = x1 + t * dx;
        const py = y1 + t * dy;

        return { x: px, y: py, ratio: t };
    }

    getLength() {
        const dx = this.endIntersection.x - this.startIntersection.x;
        const dy = this.endIntersection.y - this.startIntersection.y;
        return Math.hypot(dx, dy);
    }

    accept()
    {
        // TODO: add acception actions, e. g. split road
        this.notify(this, ROAD_IS_ACCEPTED);
    }

    delete()
    {
        if (!this.is_deleted)
        {
            this.is_deleted = true;
            Road.roads.delete(this);

            this.startIntersection.disconnectRoad(this);
            this.endIntersection.disconnectRoad(this);

            this.startIntersection = null;
            this.endIntersection = null;

            this.forwardSegments = null;
            this.backwardSegments = null;

            this.lanes = null;
            this.notify(this, ROAD_IS_DELETED);
        }
    }

    static getRoadAtPoint(x, y)
    {
        for (let road of Road.roads)
        {
            const closest = road.findClosestPointOnRoad(x, y);
            if (!closest) continue;
            const dist = Math.hypot(closest.x - x, closest.y - y);
            if (dist < ROAD_SNAP_RADIUS)
                return road;
        }
        return null;
    }
}

