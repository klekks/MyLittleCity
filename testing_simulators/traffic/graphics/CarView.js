import { Car } from '../core/Car.js';

export class CarView extends Car {
  draw(ctx) {
    if (this.finished) return;

    const road = this.currentLane.road;
    const dx = road.endIntersection.x - road.startIntersection.x;
    const dy = road.endIntersection.y - road.startIntersection.y;
    const fullLength = Math.hypot(dx, dy);
    const segmentLength = fullLength * Math.abs(this.endRatio - this.startRatio);
    const ratioOffset = this.progress / segmentLength;

    const t = this.startRatio + (this.endRatio - this.startRatio) * ratioOffset;
    const { x, y } = road.getPointAtRatio(t);

    const perpX = -dy / fullLength;
    const perpY = dx / fullLength;
    const offsetX = perpX * (this.currentLane.direction === 1 ? 5 : -5);
    const offsetY = perpY * (this.currentLane.direction === 1 ? 5 : -5);

    ctx.fillStyle = "blue";
    ctx.save();
    ctx.translate(x + offsetX, y + offsetY);
    ctx.rotate(Math.atan2(dy, dx));
    ctx.fillStyle = "blue";
    ctx.fillRect(-4, -2, 8, 4);  // прямоугольник вместо точки
    ctx.restore();

  }
}
