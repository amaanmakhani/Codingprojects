package jpacman.level;

import jpacman.points.DefaultPointCalculator;

public class DefaultPlayerInteractionMapTest extends PlayerCollisionTest {
    public DefaultPlayerInteractionMapTest(){
        setCmap(new DefaultPlayerInteractionMap(new DefaultPointCalculator()));
    }
}
