import { find_path } from "../utils/Pathfinding.js"
import { ROAD_END_RATIO, ROAD_BEGIN_RATIO } from "../utils/constants.js";

export class Car {
    // TODO: random speed
    // TODO: aceleration
    static taskQueue = [];
    static idx = 0;
    constructor(fromBuilding, toBuilding) {
        this.id = Car.idx++;
        this.from = fromBuilding;
        this.to = toBuilding;
        this.speed = 1;// + Math.random() % 0.2;
        this.displaiable = false;
        this.path = this.findValidPath(fromBuilding, toBuilding);

        if (!this.path) {
            console.warn("Маршрут не найден");
            this.finished = true;
            return;
        }

        this.queue_push("initializeMovement", [fromBuilding, toBuilding]);
    }

    static queue_pop()
    {
        return Car.taskQueue.shift();
    }

    queue_push(name, args)
    {
        Car.taskQueue.push([this, name, args]);
    }

    findValidPath(fromBuilding, toBuilding) {
        const fromLane = fromBuilding.side === 1 ? fromBuilding.road.getLanes()[0] : fromBuilding.road.getLanes()[1];
        const toLane = toBuilding.side === 1 ? toBuilding.road.getLanes()[0] : toBuilding.road.getLanes()[1];
        const path = find_path(fromLane, fromBuilding.ratio, toLane, toBuilding.ratio);

        if (path.length === 0) {
            console.warn("Маршрут не найден");
            this.finished = true;
            return;
        }
        // console.log(path.map((e) => { return e.road.id; }))

        return path;
    }


    initializeMovement(fromBuilding, toBuilding) {
        // console.log(this.from.id, this.to.id);
        this.currentLaneIndex = 0;
        let currentLane = this.path[this.currentLaneIndex];

        this.startAtLaneRatio = currentLane.getLaneRatioAtRoadRatio(fromBuilding.ratio); 
         
        if (this.path.length == 1)
        {
            this.endAtLaneRatio = currentLane.getLaneRatioAtRoadRatio(toBuilding.ratio);
        }
        else
        {
            this.endAtLaneRatio = currentLane.getLaneRatioAtRoadRatio(currentLane.direction == 1 ? ROAD_END_RATIO : ROAD_BEGIN_RATIO);
        }

        let currentRatio = this.startAtLaneRatio;
        let laneSegments = currentLane.getSegments();
        this.currentSegmentIndex = Math.floor(currentRatio * laneSegments.length);

        if (laneSegments[this.currentSegmentIndex].isOccupied())
        {
            this.queue_push("initializeMovement", [fromBuilding, toBuilding]);
            return;
        }
        laneSegments[this.currentSegmentIndex].occupy(this);

        this.progress = currentRatio * currentLane.getLength();

        this.finished = false;
        this.displaiable = true;

        const roadIds = this.path.map(lane => `${lane.direction === 1 ? "→" : "←"}R${lane.road.id}`);
        console.log(`Маршрут машины ${fromBuilding.id} → ${toBuilding.id}:`, roadIds.join(" "));
    }

    getNextPosition()
    {
        let currentLane = this.path[this.currentLaneIndex];
        let newProgress = this.progress + this.speed;
        let newRatio = newProgress / currentLane.getLength();

        let newLane = null;
        let newStartAtLaneRatio = null, newEndAtLaneRatio = null, newSegmentIndex = null, newLaneIndex = null;
        if (newRatio >= this.endAtLaneRatio)
        {
            if (this.currentLaneIndex + 1 >= this.path.length)
            {
                return null;
            }
            newLaneIndex = this.currentLaneIndex + 1;
            newLane = this.path[newLaneIndex];
            newStartAtLaneRatio = ROAD_BEGIN_RATIO;
            newEndAtLaneRatio = (newLaneIndex + 1 == this.path.length) 
                                ? newLane.getLaneRatioAtRoadRatio(this.to.ratio) 
                                : ROAD_END_RATIO;
            newProgress = newLane.getLength() * newStartAtLaneRatio;
            newSegmentIndex = 0; 
        }  
        else
        {
            newLane = currentLane;
            newLaneIndex = this.currentLaneIndex;
            newStartAtLaneRatio = this.startAtLaneRatio;
            newEndAtLaneRatio = this.endAtLaneRatio;
            newSegmentIndex = Math.floor(newRatio * newLane.getSegments().length);
        } 

        return [
            newLaneIndex,
            newStartAtLaneRatio,
            newEndAtLaneRatio,
            newProgress,
            newSegmentIndex
        ];
    }

    update() {
        if (this.finished || !this.displaiable) return;

        let currentLane = this.path[this.currentLaneIndex];
        let next_position = this.getNextPosition();
        if (!next_position)
        {
            this.finished = true;
            currentLane.getSegments()[this.currentSegmentIndex]?.release();
            return;
        }

        let [newLaneIndex, newStartAtLaneRatio, newEndAtLaneRatio, newProgress, newSegmentIndex] = next_position;


        if (newSegmentIndex !== this.currentSegmentIndex || newLaneIndex !== this.currentLaneIndex) 
        {
            const previousSegments = currentLane.getSegments();
            const newSegments = this.path[newLaneIndex].getSegments();

            if (newSegments[newSegmentIndex].isOccupied()) 
                return;

            newSegments[newSegmentIndex].occupy(this);
            previousSegments[this.currentSegmentIndex]?.release();
        }

        this.currentLaneIndex = newLaneIndex;
        this.startAtLaneRatio = newStartAtLaneRatio;
        this.endAtLaneRatio = newEndAtLaneRatio;
        this.progress = newProgress;
        this.currentSegmentIndex = newSegmentIndex;
    }


    isFinished() {
        return this.finished;
    }
}
