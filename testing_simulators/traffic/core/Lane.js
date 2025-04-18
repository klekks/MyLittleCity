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
    const intersection = this.getEndIntersection();
    return intersection.connectedRoads.length === 1;
  }

  isOutgoingFrom(intersection) {
    const start = this.getStartIntersection();
    return Math.abs(start.x - intersection.x) < 0.1 &&
           Math.abs(start.y - intersection.y) < 0.1;
  }

  getLength() {
    const dx = this.road.endIntersection.x - this.road.startIntersection.x;
    const dy = this.road.endIntersection.y - this.road.startIntersection.y;
    return Math.hypot(dx, dy);
  }

  getPointAtDistance(distance) {
    const ratio = distance / this.getLength();
    return this.road.getPointAtRatio(this.direction === 1 ? ratio : 1 - ratio);
  }
}
