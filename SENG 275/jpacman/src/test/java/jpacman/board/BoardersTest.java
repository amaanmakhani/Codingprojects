package jpacman.board;
import jpacman.level.LevelFactory;
import jpacman.level.PlayerFactory;
import jpacman.npc.ghost.GhostFactory;
import jpacman.npc.ghost.GhostMapParser;
import jpacman.points.DefaultPointCalculator;
import jpacman.sprite.PacManSprites;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.CsvSource;

import static org.assertj.core.api.Assertions.assertThat;

public class BoardersTest {
    Board temp;
    int rows = 8;
    int columns = 7;

    @BeforeEach
    void setUp() {
        Square[][] sq = new Square[rows][columns];
        for (int i = 0; i < sq.length; i++){
            for(int j = 0; j < sq[0].length; j++){
                sq[i][j] = new BasicSquare();
            }
        }
        temp = new Board(sq);
    }

    //Decision classes
    //1. One edge within border and one outside the border
    //  i.e (x out and y in) and (x in and y out)
    //2. Both edges within the borders
    //3. Both edges outside the borders
    @ParameterizedTest
    @CsvSource({
            "1, 1",
            "0, 9",
            "9, 7",
            "200, 200"
    })
    void BoarderConstrained( int x, int y) {
        assertThat(temp.withinBorders(x, y)).isEqualTo(( x <= rows && y <=columns));
    }
}
