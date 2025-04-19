import { RoadView } from '../graphics/RoadView.js';
import { IntersectionView } from '../graphics/IntersectionView.js';


const INTERSECTION_SNAP_RADIUS = 10;
const MIN_ANGLE_DEGREES = 20;
const SEGMENTS_PER_PIXEL = 1 / 10;
const MIN_ROAD_LENGTH = 30;
const ANGLE_SNAP_TOLERANCE = 15;



export function snapToRelativeAngle(x0, y0, x1, y1, neighbors) {
    const dx = x1 - x0;
    const dy = y1 - y0;

    for (const road of neighbors) {
        const {
            x: x2,
            y: y2
        } = road.startIntersection === undefined || road.startIntersection.x === x0 && road.startIntersection.y === y0 ? road.endIntersection : road.startIntersection;
        const refDx = x2 - x0;
        const refDy = y2 - y0;

        const baseAngle = Math.atan2(refDy, refDx);
        const targetAngle = Math.atan2(dy, dx);
        const delta = (targetAngle - baseAngle) * 180 / Math.PI;

        const snapped = [0, 90, 180, 270, -90, -180, -270].find(d => Math.abs(delta - d) < ANGLE_SNAP_TOLERANCE);
        if (snapped !== undefined) {
            const snappedRad = (baseAngle + snapped * Math.PI / 180);
            const len = Math.hypot(dx, dy);
            return {
                dx: Math.cos(snappedRad) * len,
                dy: Math.sin(snappedRad) * len
            };
        }
    }

    return {
        dx,
        dy
    };
}




export class MapRenderer {
    constructor(canvas, ctx) {
        this.canvas = canvas;
        this.ctx = ctx;
        this.roads = [];
        this.buildings = [];
        this.intersections = [];
        this.cars = [];
        this.previewBuilding = null;

        // Offscreen canvas для статических объектов
        this.staticCanvas = document.createElement('canvas');
        this.staticCanvas.width = canvas.width;
        this.staticCanvas.height = canvas.height;
        this.staticCtx = this.staticCanvas.getContext('2d');

        // Флаг необходимости обновления offscreen canvas
        this.needRedrawStatic = true;
        this.extra_render_callbacks = {};
    }

    findNearbyIntersection(x, y) {
        return this.intersections.find(i => {
            const dx = i.x - x;
            const dy = i.y - y;
            return Math.hypot(dx, dy) <= INTERSECTION_SNAP_RADIUS;
        });
    }

    push_extra_render_callback(key, callback)
    {
        this.extra_render_callbacks[key] = callback;
    }

    pop_extra_render_callback(key)
    {
        delete this.extra_render_callbacks[key];
    }

    findRoadAt(x, y) {
        return this.roads.find(road => {
            const closest = road.findClosestPointOnRoad(x, y);
            if (!closest) return false;
            const dist = Math.hypot(closest.x - x, closest.y - y);
            return dist < INTERSECTION_SNAP_RADIUS;
        });
    }

    findOrCreateIntersection(x, y) {
        const nearby = this.findNearbyIntersection(x, y);
        if (nearby) return nearby;
        const newIntersection = new IntersectionView(x, y);
        this.intersections.push(newIntersection);
        return newIntersection;
    }

    findOrSplitRoadAt(x, y) {
        for (const road of this.roads) {
            const closest = road.findClosestPointOnRoad(x, y);
            if (!closest) continue;
            const dist = Math.hypot(closest.x - x, closest.y - y);
            if (dist < INTERSECTION_SNAP_RADIUS) {
                const newIntersection = new IntersectionView(closest.x, closest.y);
                this.intersections.push(newIntersection);

                const segmentCount1 = Math.max(1, Math.round(road.segmentCount * closest.ratio));
                const segmentCount2 = Math.max(1, road.segmentCount - segmentCount1);

                road.startIntersection.disconnectRoad(road);
                road.endIntersection.disconnectRoad(road);

                const road1 = new RoadView(road.startIntersection, newIntersection, segmentCount1);
                const road2 = new RoadView(newIntersection, road.endIntersection, segmentCount2);

                this.roads = this.roads.filter(r => r !== road);
                this.roads.push(road1, road2);
                return newIntersection;
            }
        }
        return null;
    }

    isAngleAllowed(newStart, newEnd, intersection) {
        const newVector = [newEnd.x - newStart.x, newEnd.y - newStart.y];
        for (const road of intersection.connectedRoads) {
            const other = road.startIntersection === intersection ? road.endIntersection : road.startIntersection;
            const existingVector = [other.x - intersection.x, other.y - intersection.y];
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
    doLinesIntersect(a1, a2, b1, b2) {
        function ccw(p1, p2, p3) {
            return (p3.y - p1.y) * (p2.x - p1.x) > (p2.y - p1.y) * (p3.x - p1.x);
        }
        return (
            ccw(a1, b1, b2) !== ccw(a2, b1, b2) &&
            ccw(a1, a2, b1) !== ccw(a1, a2, b2)
        );
    }
    isIntersectingExistingRoads(start, end) {
        for (const road of this.roads) {
            const a1 = road.startIntersection;
            const a2 = road.endIntersection;
            if ((a1 === start || a1 === end || a2 === start || a2 === end)) continue;
            if (this.doLinesIntersect(start, end, a1, a2)) {
                return true;
            }
        }
        return false;
    }

    addRoadByCoords(startX, startY, endX, endY) {
        let dx = endX - startX;
        let dy = endY - startY;

        let start = this.findNearbyIntersection(startX, startY);
        if (!start) start = this.findOrSplitRoadAt(startX, startY) || this.findOrCreateIntersection(startX, startY);

        ({ dx, dy } = snapToRelativeAngle(startX, startY, startX + dx, startY + dy, start.connectedRoads));
        endX = startX + dx;
        endY = startY + dy;


        const length = Math.hypot(dx, dy);

        if (length < MIN_ROAD_LENGTH) {
            console.warn("Cannot add road: too short.");
            return;
        }

        let end = this.findNearbyIntersection(endX, endY);
        if (!end) end = this.findOrSplitRoadAt(endX, endY) || this.findOrCreateIntersection(endX, endY);

        if (!start.canConnectMore() || !end.canConnectMore()) {
            console.warn("Cannot add road: one of the intersections has max connections.");
            return;
        }

        if (!this.isAngleAllowed(start, end, start) || !this.isAngleAllowed(end, start, end)) {
            console.warn("Cannot add road: angle too small.");
            return;
        }

        if (this.isIntersectingExistingRoads(start, end)) {
            console.warn("Cannot add road: intersects existing road.");
            return;
        }

        const segmentCount = Math.max(1, Math.round(length * SEGMENTS_PER_PIXEL));

        const road = new RoadView(start, end, segmentCount);
        this.roads.push(road);

        this.needRedrawStatic = true;
    }

    drawStaticObjects() {
        this.staticCtx.clearRect(0, 0, this.staticCanvas.width, this.staticCanvas.height);
      
        this.roads.forEach(road => road.draw(this.staticCtx));
        this.buildings.forEach(building => building.draw(this.staticCtx));
        //this.intersections.forEach(intersection => intersection.draw(this.staticCtx));
      
        this.needRedrawStatic = false;
      }

    removeRoad(road) {
        console.log(road.startIntersection.connectedRoads);
        console.log(road.endIntersection.connectedRoads);
        road.startIntersection.connectedRoads = road.startIntersection.connectedRoads.filter(r => r !== road);
        road.endIntersection.connectedRoads = road.endIntersection.connectedRoads.filter(r => r !== road);
        console.log(road.startIntersection.connectedRoads);
        console.log(road.endIntersection.connectedRoads);
        this.roads = this.roads.filter(r => r !== road);

        this.needRedrawStatic = true;
    }

    render() 
    {
        //if (this.needRedrawStatic) 
        {
            this.drawStaticObjects();
        }

        this.ctx.clearRect(0, 0, this.canvas.width, this.canvas.height);
        this.ctx.drawImage(this.staticCanvas, 0, 0);

        if (this.previewBuilding) {
            const canPlace = !this.buildings.some(b => this.previewBuilding.intersects(b));
            this.previewBuilding.drawGhost(this.ctx, canPlace);
        }

        this.cars.forEach(car => {
            car.update();
            car.draw(this.ctx);
        });
        this.cars = this.cars.filter(car => !car.isFinished());

        for (let callback in this.extra_render_callbacks)
        {
            this.extra_render_callbacks[callback](this.ctx);
        }
    }
}
