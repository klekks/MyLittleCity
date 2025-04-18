import { findDirectedRoadPath } from "../utils/Pathfinding.js"

export class Car {
  constructor(fromBuilding, toBuilding) {
    this.from = fromBuilding;
    this.to = toBuilding;
    this.speed = 0.5 + Math.random();

    const path = this.findValidPath(fromBuilding, toBuilding);

    if (!path) {
      console.warn("Маршрут не найден");
      this.finished = true;
      return;
    }

    this.initializeMovement(path, fromBuilding, toBuilding);
  }

  findValidPath(fromBuilding, toBuilding) {
    const path = findDirectedRoadPath(
      fromBuilding.road,
      fromBuilding.side,
      toBuilding.road,
      toBuilding.side,
      { allowTurnarounds: true }
    );

    if (path.length === 0) {
      console.warn("Маршрут не найден");
      this.finished = true;
      return;
    }


    return path;
  }


  initializeMovement(path, fromBuilding, toBuilding) {
    this.lanePath = path;
    this.lanePathIndex = 0;
    this.currentLane = path[0];

    this.startRatio = fromBuilding.ratio;
    this.endRatio = (path.length === 1)
      ? toBuilding.ratio
      : (this.currentLane.direction === 1 ? 1 : 0);

    const segments = this.currentLane.direction === 1
      ? this.currentLane.road.forwardSegments
      : this.currentLane.road.backwardSegments;

    const segmentCount = segments.length;

    this.segmentIndex = Math.floor(
      (this.currentLane.direction === 1
        ? this.startRatio
        : (1 - this.startRatio)) * segmentCount
    );

    segments[this.segmentIndex].occupy();

    this.progress = 0;
    this.finished = false;

    const roadIds = path.map(lane => `${lane.direction === 1 ? "→" : "←"}R${lane.road.id}`);
    console.log(`Маршрут машины ${fromBuilding.id} → ${toBuilding.id}:`, roadIds.join(" "));
  }



  update() {
    if (this.finished) return;

    const road = this.currentLane.road;
    const segments = this.currentLane.direction === 1
      ? this.currentLane.road.forwardSegments
      : this.currentLane.road.backwardSegments;

    const segmentCount = segments.length;

    const movingForward = this.endRatio > this.startRatio;
    const fullLength = Math.hypot(
      road.endIntersection.x - road.startIntersection.x,
      road.endIntersection.y - road.startIntersection.y
    );
    const segmentLength = Math.abs(this.endRatio - this.startRatio) * fullLength;

    // Продвигаем машину
    this.progress += this.speed;

    // Рассчитываем текущее положение вдоль линии
    const currentRatio = this.startRatio + (this.endRatio - this.startRatio) * (this.progress / segmentLength);
    const clampedRatio = Math.max(0, Math.min(1, currentRatio));
    let segmentIndex;
    if (this.currentLane.direction === 1) {
      segmentIndex = Math.min(
        segmentCount - 1,
        Math.floor(clampedRatio * segmentCount)
      );
    } else {
      segmentIndex = Math.min(
        segmentCount - 1,
        Math.floor((1 - clampedRatio) * segmentCount)
      );
    }


    if (segmentIndex !== this.segmentIndex) {
      // Перешли в новый сегмент → обновляем занятость
      const previousSegments = this.currentLane.direction === 1
        ? road.forwardSegments
        : road.backwardSegments;
      previousSegments[this.segmentIndex]?.release();

      const currentSegment = segments[segmentIndex];
      if (currentSegment.isOccupied()) {
        // Новый сегмент занят — не двигаемся дальше
        this.progress -= this.speed;
        previousSegments[this.segmentIndex]?.occupy(); // возвращаем занятость
        return;
      }

      currentSegment.occupy();
      this.segmentIndex = segmentIndex;
    }

    // Дошли до конца текущего сегмента
    if (this.progress >= segmentLength) {
      // Освобождаем текущий сегмент
      segments[this.segmentIndex]?.release();

      this.lanePathIndex = this.lanePath.indexOf(this.currentLane) + 1;
      if (this.lanePathIndex >= this.lanePath.length) {
        this.finished = true;
        return;
      }

      const nextLane = this.lanePath[this.lanePathIndex];
      const nextRoad = nextLane.road;
      const nextSegments = nextLane.direction === 1
        ? nextRoad.forwardSegments
        : nextRoad.backwardSegments;

      let nextSegmentIndex = Math.floor((nextLane.direction === 1 ? 0 : 1) * nextSegments.length);
      nextSegmentIndex -= (nextLane.direction === 1 ? 0 : 1);

      if (nextSegments[nextSegmentIndex].isOccupied()) {
        // Ждём освобождения входа в следующую дорогу
        this.progress = segmentLength;
        segments[this.segmentIndex]?.occupy(); // держим текущий
        return;
      }

      // Переход на новую полосу
      nextSegments[nextSegmentIndex].occupy();
      this.currentLane = nextLane;
      this.segmentIndex = nextSegmentIndex;
      this.startRatio = (nextLane.direction === 1) ? 0 : 1;
      this.endRatio = (this.lanePathIndex === this.lanePath.length - 1)
        ? this.to.ratio
        : (nextLane.direction === 1 ? 1 : 0);
      this.progress = 0;
    }
  }


  isFinished() {
    return this.finished;
  }
}
