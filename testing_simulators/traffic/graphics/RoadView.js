import { Road } from '../core/Road.js';
import { Lane } from "../core/Lane.js";

export class RoadView extends Road {
  draw(ctx) {
    const start = this.startIntersection;
    const end = this.endIntersection;

    const width = 20;
    const halfWidth = width / 2;

    const dx = end.x - start.x;
    const dy = end.y - start.y;
    const len = Math.hypot(dx, dy);
    const nx = dx / len;
    const ny = dy / len;
    const px = -ny;
    const py = nx;

    // точки по краям дороги
    const sx1 = start.x + px * halfWidth;
    const sy1 = start.y + py * halfWidth;
    const sx2 = start.x - px * halfWidth;
    const sy2 = start.y - py * halfWidth;
    const ex1 = end.x + px * halfWidth;
    const ey1 = end.y + py * halfWidth;
    const ex2 = end.x - px * halfWidth;
    const ey2 = end.y - py * halfWidth;

    ctx.beginPath();
    ctx.moveTo(sx1, sy1);
    ctx.lineTo(ex1, ey1);
    ctx.lineTo(ex2, ey2);
    ctx.lineTo(sx2, sy2);
    ctx.closePath();
    ctx.fillStyle = "#444";
    ctx.fill();


    const midOffset = 3;
    const length = Math.hypot(dx, dy);
    const normX = dx / length;
    const normY = dy / length;
    const offsetX = -normY * midOffset;
    const offsetY = normX * midOffset;

    ctx.strokeStyle = "#888";
    ctx.lineWidth = 1;
    ctx.beginPath();
    ctx.moveTo(start.x + offsetX, start.y + offsetY);
    ctx.lineTo(end.x + offsetX, end.y + offsetY);
    ctx.stroke();

    ctx.beginPath();
    ctx.moveTo(start.x - offsetX, start.y - offsetY);
    ctx.lineTo(end.x - offsetX, end.y - offsetY);
    ctx.stroke();

    // Центральная пунктирная линия
    ctx.strokeStyle = "#ffffff";
    ctx.lineWidth = 2;
    ctx.setLineDash([10, 10]);

    ctx.beginPath();
    ctx.moveTo(start.x, start.y);
    ctx.lineTo(end.x, end.y);
    ctx.stroke();

    ctx.setLineDash([]);

    const midX = (start.x + end.x) / 2;
    const midY = (start.y + end.y) / 2;

    
    ctx.fillStyle = "black";
    ctx.font = "10px sans-serif";
    ctx.textAlign = "center";
    ctx.fillText(`R${this.id}`, midX, midY - 8);

    // Рисуем занятые сегменты
  const segmentRadius = 3;

  // Направление: start → end
  this.forwardSegments.forEach((segment, index) => {
    if (segment.occupied) {
      const lane = new Lane(this, 1);
      const pos = lane.getPointAtDistance((index + 0.5) * this.getLength() / this.segmentCount);
      ctx.beginPath();
      ctx.arc(pos.x - ny * 5, pos.y + nx * 5, segmentRadius, 0, Math.PI * 2);
      ctx.fillStyle = "red";
      ctx.fill();
    }
  });

  // Направление: end → start
  this.backwardSegments.forEach((segment, index) => {
    if (segment.occupied) {
      const lane = new Lane(this, -1);
      const pos = lane.getPointAtDistance((index + 0.5) * this.getLength() / this.segmentCount);
      ctx.beginPath();
      ctx.arc(pos.x + ny * 5, pos.y - nx * 5, segmentRadius, 0, Math.PI * 2);
      ctx.fillStyle = "red";
      ctx.fill();
    }
  });

    this.startIntersection.draw(ctx);
    this.endIntersection.draw(ctx);

    this.drawArrow(ctx, start.x - normY * 5, start.y + normX * 5, end.x - normY * 5, end.y + normX * 5);
    this.drawArrow(ctx, end.x + normY * 5, end.y - normX * 5, start.x + normY * 5, start.y - normX * 5);
  }

  drawArrow(ctx, fromX, fromY, toX, toY) {
    const dx = toX - fromX;
    const dy = toY - fromY;
    const angle = Math.atan2(dy, dx);

    const midX = fromX + dx * 0.5;
    const midY = fromY + dy * 0.5;

    ctx.save();
    ctx.translate(midX, midY);
    ctx.rotate(angle);
    ctx.fillStyle = "white";
    ctx.beginPath();
    ctx.moveTo(-5, -3);
    ctx.lineTo(5, 0);
    ctx.lineTo(-5, 3);
    ctx.closePath();
    ctx.fill();
    ctx.restore();
  }
}