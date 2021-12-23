package jpacman.level;

import jpacman.points.DefaultPointCalculator;

public class CollisionInteractionMapTest extends PlayerCollisionTest {
    public CollisionInteractionMapTest(){
        setCmap(new PlayerCollisions(new DefaultPointCalculator()));
    }
}