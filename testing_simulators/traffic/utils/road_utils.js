import { IntersectionView } from "../graphics/IntersectionView";

export function findOrCreateIntersection(x, y) 
{
    const nearby = IntersectionView.findIntersectionAtPoint(x, y);
    if (nearby) return nearby;
    const newIntersection = new IntersectionView(x, y);
    return newIntersection;
}

