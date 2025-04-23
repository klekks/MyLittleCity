import { ObserverSubject } from "../observers/Observer";
// ax, ay, bx, by - vector, px, py - point
function pointSideIsLeft(ax, ay, bx, by, px, py) {
    const cross = (bx - ax) * (py - ay) - (by - ay) * (px - ax);
    if (cross > 0) return 1;
    return 0;
}

const BUILDING_IS_CREATED = 0x01;

export class Building extends ObserverSubject {
    static random_size_seed = Math.random();
    static idCounter = 0;
    constructor(road, ratio, side = 1, offset = 20, width = null, height = null) {
        this.id = Building.idCounter;
        const { width: w, height: h } = Building.randomSize();
        // this.direction = pointSideIsLeft(road.startIntersection.x, road.startIntersection.y,
        //                                 road.endIntersection.x, road.endIntersection.y,

        // )
        this.width = width || w;
        this.height = height || h;
        this.road = road;
        this.ratio = ratio;
        this.side = side;
        this.offset = offset;
        this.color = Building.randomColor();
        console.log(`Building ${this.id} is ${this.side}`);
        this.notify(this, BUILDING_IS_CREATED);
    }

    static randomSize() {
        const sizes = [
            { width: 10, height: 10 },
            { width: 14, height: 12 },
            { width: 20, height: 16 },
            { width: 12, height: 18 },
        ];
        return sizes[Math.floor(this.random_size_seed * sizes.length)];
    }

    static place() {
        Building.random_size_seed = Math.random();
        Building.idCounter++;
    }

    static randomColor() {
        const colors = ['#0077cc', '#cc7700', '#00aa55', '#aa00aa', '#555555'];
        return colors[Math.floor(Math.random() * colors.length)];
    }

    getBoundingBox() {
        const { x, y } = this.road.getPointAtRatio(this.ratio);
        const dx = this.road.endIntersection.x - this.road.startIntersection.x;
        const dy = this.road.endIntersection.y - this.road.startIntersection.y;
        const length = Math.hypot(dx, dy);
        const angle = Math.atan2(dy, dx);

        const perpX = -dy / length;
        const perpY = dx / length;

        const baseX = x + perpX * this.offset * this.side;
        const baseY = y + perpY * this.offset * this.side;

        const cos = Math.cos(angle);
        const sin = Math.sin(angle);

        // Повернутый прямоугольник, но просто вернём AABB (ось-выравненный bounding box)
        return {
            x: baseX - this.width / 2,
            y: baseY - this.height / 2,
            width: this.width,
            height: this.height,
        };
    }

    intersects(other) {
        const a = this.getBoundingBox();
        const b = other.getBoundingBox();
        return !(
            a.x + a.width < b.x ||
            a.x > b.x + b.width ||
            a.y + a.height < b.y ||
            a.y > b.y + b.height
        );
    }
}
