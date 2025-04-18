import { Lane } from "./Lane.js"


class Segment {
    constructor() {
        this.occupied = false;
    }

    isOccupied() {
        return this.occupied;
    }

    occupy() {
        this.occupied = true;
    }

    release() {
        this.occupied = false;
    }
}


export class Road {
    static idCounter = 0;
    constructor(startIntersection, endIntersection, segmentCount = 10) {
        this.id = Road.idCounter++;

        this.startIntersection = startIntersection;
        this.endIntersection = endIntersection;
        this.segmentCount = segmentCount;

        this.forwardSegments = Array.from({ length: segmentCount }, () => new Segment());
        this.backwardSegments = Array.from({ length: segmentCount }, () => new Segment());

        this.startIntersection.connectRoad(this);
        this.endIntersection.connectRoad(this);
    }

    getLanes() {
        return [new Lane(this, 1), new Lane(this, -1)];
    }

    getPointAtRatio(ratio) {
        const x = this.startIntersection.x + (this.endIntersection.x - this.startIntersection.x) * ratio;
        const y = this.startIntersection.y + (this.endIntersection.y - this.startIntersection.y) * ratio;
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
}