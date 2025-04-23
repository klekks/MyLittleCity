import { RoadView } from '../graphics/RoadView.js';
import { IntersectionView } from '../graphics/IntersectionView.js';
import { isIntersectingExistingRoads, split_road_at } from './road_utils.js';


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


export class Renderer
{
    constructor(canvas, ctx)
    {
        this.canvas = canvas;
        this.ctx = ctx;

        this.static_objects = new Set();
        this.dynamic_objects = new Set();

        this.preview_objects = new Set();

        this.staticCanvas = document.createElement('canvas');
        this.staticCanvas.width = canvas.width;
        this.staticCanvas.height = canvas.height;
        this.staticCtx = this.staticCanvas.getContext('2d');

        this.needRedrawStatic = true;
    }

    add_static(object)
    {
        this.static_objects.add(object);
        this.needRedrawStatic = true;
    }

    add_dynamic(object)
    {
        this.dynamic_objects.add(object);
    }

    add_preview(object)
    {
        this.preview_objects.add(object);
    }

    remove_static(object)
    {
        this.static_objects.delete(object);
    }

    remove_dynamic(object)
    {
        this.dynamic_objects.delete(object);
    }

    remove_preview(object)
    {
        this.preview_objects.delete(object);
    }


    static draw_objects(ctx, canvas, objects)
    {
        ctx.clearRect(0, 0, canvas.width, canvas.height);
        objects.forEach(object => object.draw(ctx));
    }

    draw_static()
    {
        Renderer.draw_objects(this.staticCtx, this.staticCanvas, this.static_objects);
        this.needRedrawStatic = false;
    }

    draw_dynamic()
    {
        this.ctx.clearRect(0, 0, canvas.width, canvas.height);
        this.ctx.drawImage(this.staticCanvas, 0, 0);
        Renderer.draw_objects(this.ctx, this.canvas, this.dynamic_objects);
    }

    draw_preview()
    {
        Renderer.draw_objects(this.ctx, this.canvas, this.preview_objects);
    }

    draw()
    {
        if (this.needRedrawStatic)
            this.draw_static();
        this.draw_dynamic();
        this.draw_preview();
    }
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

    // findOrSplitRoadAt(x, y) {
    //     for (const road of this.roads) {
    //         const closest = road.findClosestPointOnRoad(x, y);
    //         if (!closest) continue;
    //         const dist = Math.hypot(closest.x - x, closest.y - y);
    //         if (dist < INTERSECTION_SNAP_RADIUS) {
    //             const newIntersection = new IntersectionView(closest.x, closest.y);
    //             this.intersections.push(newIntersection);

    //             const segmentCount1 = Math.max(1, Math.round(road.segmentCount * closest.ratio));
    //             const segmentCount2 = Math.max(1, road.segmentCount - segmentCount1);

    //             road.startIntersection.disconnectRoad(road);
    //             road.endIntersection.disconnectRoad(road);

    //             const road1 = new RoadView(road.startIntersection, newIntersection, segmentCount1);
    //             const road2 = new RoadView(newIntersection, road.endIntersection, segmentCount2);

    //             this.roads = this.roads.filter(r => r !== road);
    //             this.roads.push(road1, road2);
    //             return newIntersection;
    //         }
    //     }
    //     return null;
    // }


    

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
