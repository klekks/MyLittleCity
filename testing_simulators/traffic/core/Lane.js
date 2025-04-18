export class Lane {
    constructor(road, direction) {
        this.road = road;
        this.direction = direction; // 1 = start→end, -1 = end→start
    }

    getStartIntersection() {
        return this.direction === 1
            ? this.road.startIntersection
            : this.road.endIntersection;
    }

    getEndIntersection() {
        return this.direction === 1
            ? this.road.endIntersection
            : this.road.startIntersection;
    }

    getId() {
        return `${this.road.id}_${this.direction}`;
    }

    equals(other) {
        return this.road === other.road && this.direction === other.direction;
    }

    canTurnAround() {
        return this.getEndIntersection().canTurnAround();
    }

    isOutgoingFrom(intersection) {
        return this.getStartIntersection().equals(intersection);
    }

    getLength() {
        return this.road.getLength();
    }

    getPointAtDistance(distance) {
        const ratio = distance / this.getLength();
        return this.road.getPointAtRatio(this.direction === 1 ? ratio : 1 - ratio);
    }
}
