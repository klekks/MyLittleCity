
const MAX_CONNECTIONS_PER_INTERSECTION = 4;

export class Intersection {
  constructor(x, y) {
      this.x = x;
      this.y = y;
      this.connectedRoads = [];
  }

  canConnectMore() {
      return this.connectedRoads.length < MAX_CONNECTIONS_PER_INTERSECTION;
  }

  connectRoad(road) {
      if (this.connectedRoads.includes(road)) return;
      if (this.canConnectMore()) {
          this.connectedRoads.push(road);
      } else {
          throw new Error("Intersection at (" + this.x + ", " + this.y + ") has reached max connections.");
      }
  }

  disconnectRoad(road) {
      const index = this.connectedRoads.indexOf(road);
      if (index !== -1) {
          this.connectedRoads.splice(index, 1);
      }
  }
}