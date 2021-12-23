package jpacman.level;
import jpacman.PacmanConfigurationException;
import jpacman.board.BoardFactory;
import jpacman.board.Square;
import jpacman.npc.Ghost;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import static org.assertj.core.api.Assertions.assertThatThrownBy;
import static org.mockito.Mockito.*;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

import static org.assertj.core.api.Assertions.assertThat;

public class MapParserTest {
    private MapParser testObj;
    private LevelFactory levelMock;
    private BoardFactory boardMock;

    @BeforeEach
    void setUp() {
        levelMock = mock(LevelFactory.class);
        boardMock = mock(BoardFactory.class);
        testObj = new MapParser(levelMock, boardMock);
    }

    // Good weather test cases
    @Test
    void parseGround() {
        testObj.parseMap(Collections.singletonList(" "));
        Square square = mock(Square.class);
        when(boardMock.createGround()).thenReturn(square);
        verify(boardMock).createGround();
        verify(levelMock).createLevel(any(), anyList(), anyList() );
    }

    @Test
    void parsePacman () {
        testObj.parseMap(Collections.singletonList("P"));
        Square square = mock(Square.class);
        when(boardMock.createGround()).thenReturn(square);
        verify(boardMock).createGround();
        //verify(List).add(any());
        verify(levelMock).createLevel(any(), anyList(), anyList() );
    }

    @Test
    void parseWall () {
        testObj.parseMap(Collections.singletonList("#"));
        verify(boardMock).createWall();
        verify(levelMock).createLevel(any(), anyList(), anyList() );
    }

    @Test
     void parseGhost () {
        Square square = mock(Square.class);
        Ghost ghostMock = mock(Ghost.class);
        when(levelMock.createGhost()).thenReturn(ghostMock);
        when(boardMock.createGround()).thenReturn(square);
        testObj.parseMap(Collections.singletonList("G"));
        verify(levelMock).createLevel(any(), anyList(), anyList() );
    }

    @Test
    void parsePelletSquare () {
        Square square = mock(Square.class);
        when(boardMock.createGround()).thenReturn(square);
        Pellet pellet = mock(Pellet.class);
        when(levelMock.createPellet()).thenReturn(pellet);
        testObj.parseMap(Collections.singletonList("."));
        verify(boardMock).createGround();
        verify(levelMock).createPellet();
        verify(pellet).occupy(square);
        verify(levelMock).createLevel(any(), anyList(), anyList() );
    }

    // Bad weather test cases
    @Test
    void nonEXistingFile () {
        assertThatThrownBy(()-> testObj.parseMap("no file")).isInstanceOf(PacmanConfigurationException.class);
    }

    @Test
    void invalidChar () {
        assertThatThrownBy(()-> testObj.parseMap(Collections.singletonList("F"))).isInstanceOf(PacmanConfigurationException.class);
    }

    @Test
    void invlaidFormat () {
        List<String> emList = Collections.emptyList();
        assertThatThrownBy(()-> testObj.parseMap(emList)).isInstanceOf(PacmanConfigurationException.class);
    }
}
