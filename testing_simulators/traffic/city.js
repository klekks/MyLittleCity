import {
    BuildingView
} from './graphics/BuildingView.js';
import {
    CarView
} from './graphics/CarView.js';
import {
    MapRenderer,
    snapToRelativeAngle
} from './utils/MapRenderer.js';

//window.addEventListener("contextmenu", e => e.preventDefault());


function car_queue_driver()
{
    let data = CarView.queue_pop();
    if (data)
    {
        let [object, method, args] = data;  
        object[method](...args);
    }
}


const BUILDING_GRID_STEP_PX = 30;
export let renderer;

document.addEventListener('DOMContentLoaded', function () {

    const canvas = document.getElementById("mapCanvas");
    console.log(canvas);
    const ctx = canvas.getContext("2d");
    canvas.width = window.innerWidth;
    canvas.height = window.innerHeight;

    renderer = new MapRenderer(canvas, ctx);

    let isDrawing = false;
    let startPoint = null;

    canvas.addEventListener("mousedown", (e) => {
        if (e.button === 2) {
            const rect = canvas.getBoundingClientRect();
            const x = e.clientX - rect.left;
            const y = e.clientY - rect.top;
            const road = renderer.findRoadAt(x, y);
            if (road) {
                renderer.removeRoad(road);
                // renderer.render();
            }
            return;
        }

        if (e.shiftKey) {
            const road = renderer.findRoadAt(e.offsetX, e.offsetY);
            if (road) {
                const closest = road.findClosestPointOnRoad(e.offsetX, e.offsetY);

                const roadLength = Math.hypot(
                    road.endIntersection.x - road.startIntersection.x,
                    road.endIntersection.y - road.startIntersection.y
                );

                const closestDist = roadLength * closest.ratio;

                let snappedDist = Math.round(closestDist / BUILDING_GRID_STEP_PX) * BUILDING_GRID_STEP_PX;
                snappedDist = Math.max(15, Math.min(roadLength - 15, snappedDist)); // защита от краёв

                const ratio = snappedDist / roadLength;


                // Вектора: вдоль дороги и от дороги к мыши
                const ax = road.endIntersection.x - road.startIntersection.x;
                const ay = road.endIntersection.y - road.startIntersection.y;
                const bx = e.offsetX - closest.x;
                const by = e.offsetY - closest.y;

                const cross = ax * by - ay * bx;
                const side = cross > 0 ? 1 : -1;

                const building = new BuildingView(road, ratio, side);

                const overlaps = renderer.buildings.some(b => building.intersects(b));
                if (overlaps) {
                    console.warn("Здание не добавлено: пересекается с другим зданием.");
                    return;
                }

                renderer.previewBuilding = null;

                renderer.buildings.push(building);
                BuildingView.place();
                renderer.needRedrawStatic = true;
            }
            return;
        }

        startPoint = {
            x: e.offsetX,
            y: e.offsetY
        };
        isDrawing = true;
    });

    canvas.addEventListener("mouseup", (e) => {
        if (!isDrawing) return;
        const endPoint = {
            x: e.offsetX,
            y: e.offsetY
        };
        renderer.addRoadByCoords(startPoint.x, startPoint.y, endPoint.x, endPoint.y);
        // renderer.render();
        renderer.pop_extra_render_callback("mousemove_road_creating_dash");
        isDrawing = false;
        startPoint = null;
    });

    canvas.addEventListener("mousemove", (e) => {
        if (e.shiftKey) {
            const road = renderer.findRoadAt(e.offsetX, e.offsetY);
            if (road) {
                const closest = road.findClosestPointOnRoad(e.offsetX, e.offsetY);
                const roadLength = Math.hypot(
                    road.endIntersection.x - road.startIntersection.x,
                    road.endIntersection.y - road.startIntersection.y
                );

                const closestDist = roadLength * closest.ratio;

                let snappedDist = Math.round(closestDist / BUILDING_GRID_STEP_PX) * BUILDING_GRID_STEP_PX;
                snappedDist = Math.max(15, Math.min(roadLength - 15, snappedDist)); // защита от краёв

                const ratio = snappedDist / roadLength;


                const ax = road.endIntersection.x - road.startIntersection.x;
                const ay = road.endIntersection.y - road.startIntersection.y;
                const bx = e.offsetX - closest.x;
                const by = e.offsetY - closest.y;
                const cross = ax * by - ay * bx;
                const side = cross > 0 ? 1 : -1;

                const samePreview =
                    renderer.previewBuilding &&
                    renderer.previewBuilding.road === road &&
                    Math.abs(renderer.previewBuilding.ratio - ratio) < 0.001 &&
                    renderer.previewBuilding.side === side;

                if (!samePreview) {
                    const ghost = new BuildingView(road, ratio, side);
                    renderer.previewBuilding = ghost;
                }

                // renderer.render();
                return;
            }
        }
        renderer.previewBuilding = null;

        if (!isDrawing)
        {
            renderer.pop_extra_render_callback("mousemove_road_creating_dash");
            return;
        }
        else
        {
            renderer.push_extra_render_callback("mousemove_road_creating_dash",
                (_save_ctx) => {
                    if (!startPoint) return;

                    let dx = e.offsetX - startPoint.x;
                    let dy = e.offsetY - startPoint.y;

                    let start = renderer.findNearbyIntersection(startPoint.x, startPoint.y);
                    if (!start) start = renderer.findOrSplitRoadAt(startPoint.x, startPoint.y) || renderer.findOrCreateIntersection(startPoint.x, startPoint.y);

                    ({
                        dx,
                        dy
                    } = snapToRelativeAngle(startPoint.x, startPoint.y, startPoint.x + dx, startPoint.y + dy, start.connectedRoads));

                    _save_ctx.strokeStyle = "#00aa00";
                    _save_ctx.lineWidth = 2;
                    _save_ctx.setLineDash([5, 5]);
                    _save_ctx.beginPath();
                    _save_ctx.moveTo(startPoint.x, startPoint.y);
                    _save_ctx.lineTo(startPoint.x + dx, startPoint.y + dy);
                    _save_ctx.stroke();
                    _save_ctx.setLineDash([]);
                }
            );
        }

        
    });

    // renderer.render();

    window.addEventListener("resize", () => {
        canvas.width = window.innerWidth;
        canvas.height = window.innerHeight;
        // renderer.render();
    });

    window.addEventListener("keyup", (e) => {
        if (e.key === "Shift") {
            renderer.previewBuilding = null;
            // renderer.render();
        } else if (e.key === "c") {
            if (renderer.buildings.length >= 2) {
                const from = renderer.buildings[Math.floor(Math.random() * renderer.buildings.length)];
                let to;
                do {
                    to = renderer.buildings[Math.floor(Math.random() * renderer.buildings.length)];
                } while (to === from);
                const car = new CarView(from, to, renderer.roads);
                renderer.cars.push(car);
            }
        }
    });

    function animate() {
        renderer.render();
        requestAnimationFrame(animate);
    }

    requestAnimationFrame(animate);
    setInterval(car_queue_driver, 100);
    

}, false);