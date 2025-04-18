import { Intersection } from '../core/Intersection.js';

export class IntersectionView extends Intersection {
  draw(ctx) {
    const numRoads = this.connectedRoads.length;
    const size = numRoads <= 2 ? 10 : 14;

    ctx.beginPath();
    ctx.arc(this.x, this.y, size, 0, Math.PI * 2);
    ctx.fillStyle = "#444";
    ctx.fill();

    // разметка: белые лучи в направлении каждой дороги
    for (const road of this.connectedRoads) {
      const other =
        road.startIntersection === this ? road.endIntersection : road.startIntersection;
      const dx = other.x - this.x;
      const dy = other.y - this.y;
      const len = Math.hypot(dx, dy);
      const normX = dx / len;
      const normY = dy / len;

      const lineLength = 16;
      const startX = this.x + normX * 4;
      const startY = this.y + normY * 4;
      const endX = this.x + normX * lineLength;
      const endY = this.y + normY * lineLength;

      ctx.beginPath();
      ctx.moveTo(startX, startY);
      ctx.lineTo(endX, endY);
      ctx.strokeStyle = "#fff";
      ctx.lineWidth = 2;
      ctx.stroke();
    }
  }
}
