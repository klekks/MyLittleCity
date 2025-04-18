import { Building } from '../core/Building.js';

export class BuildingView extends Building {
  draw(ctx) {
    const { x, y } = this.road.getPointAtRatio(this.ratio);
    const dx = this.road.endIntersection.x - this.road.startIntersection.x;
    const dy = this.road.endIntersection.y - this.road.startIntersection.y;
    const length = Math.hypot(dx, dy);
    const angle = Math.atan2(dy, dx);

    const normX = dx / length;
    const normY = dy / length;
    const perpX = -normY;
    const perpY = normX;

    const baseX = x + perpX * this.offset * this.side;
    const baseY = y + perpY * this.offset * this.side;

    ctx.save();
    ctx.translate(baseX, baseY);
    ctx.rotate(angle);
    ctx.fillStyle = this.color;
    ctx.fillRect(-this.width / 2, -this.height / 2, this.width, this.height);
    ctx.restore();

    ctx.save();
    ctx.translate(baseX, baseY);
    ctx.rotate(angle);
    ctx.fillStyle = "black";
    ctx.font = "10px sans-serif";
    ctx.textAlign = "center";
    ctx.fillText(`B${this.id}`, 0, this.height / 2 + 10);
    ctx.restore();

  }

  drawGhost(ctx, canPlace = true) {
    const { x, y } = this.road.getPointAtRatio(this.ratio);
    const dx = this.road.endIntersection.x - this.road.startIntersection.x;
    const dy = this.road.endIntersection.y - this.road.startIntersection.y;
    const length = Math.hypot(dx, dy);
    const angle = Math.atan2(dy, dx);

    const normX = dx / length;
    const normY = dy / length;
    const perpX = -normY;
    const perpY = normX;

    const baseX = x + perpX * this.offset * this.side;
    const baseY = y + perpY * this.offset * this.side;

    ctx.save();
    ctx.translate(baseX, baseY);
    ctx.rotate(angle);
    ctx.fillStyle = canPlace ? "rgba(0, 200, 0, 0.4)" : "rgba(200, 0, 0, 0.4)";
    ctx.fillRect(-this.width / 2, -this.height / 2, this.width, this.height);
    ctx.restore();
  }
}
