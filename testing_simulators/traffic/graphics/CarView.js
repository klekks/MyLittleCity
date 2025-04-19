import { Car } from '../core/Car.js';

export class CarView extends Car {
    draw(ctx) {
        if (this.finished || !this.displaiable) return;
        let currentLane = this.path[this.currentLaneIndex];

        const laneRatio = this.progress / currentLane.getLength();

        const dx = currentLane.getEndIntersection().x - currentLane.getStartIntersection().x;
        const dy = currentLane.getEndIntersection().y - currentLane.getStartIntersection().y;
        const fullLength = currentLane.getLength();

        const { x, y } = currentLane.getPointAtRatio(laneRatio);

        const perpX = -dy / fullLength;
        const perpY = dx / fullLength;
        const offsetX = perpX * 5;
        const offsetY = perpY * 5;

        ctx.fillStyle = "blue";
        ctx.save();
        ctx.translate(x + offsetX, y + offsetY);
        ctx.rotate(Math.atan2(dy, dx));
        ctx.fillStyle = "blue";
        ctx.fillRect(-4, -2, 8, 4);  // прямоугольник вместо точки

    
        ctx.fillStyle = "red";
        ctx.font = "12px sans-serif";
        ctx.textAlign = "center";
        ctx.fillText(`${this.id}`, 0, 0);
        ctx.restore();

    }
}
