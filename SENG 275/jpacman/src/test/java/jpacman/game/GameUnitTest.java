package jpacman.game;
import static org.mockito.Mockito.*;
import jpacman.board.BoardFactory;
import jpacman.level.*;
import jpacman.points.PointCalculator;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import static org.assertj.core.api.Assertions.assertThat;

import static org.mockito.Mockito.mock;

public class GameUnitTest {

    private Player player;
    private Level level;
    private SinglePlayerGame game;
    private PlayerFactory playerFactory;

    @BeforeEach
    void setUp() {
        level = mock(Level.class);
        player = mock(Player.class);
        game = new SinglePlayerGame(player, level, mock(PointCalculator.class));
    }

    @Test
    // Top statement true, second statement not executed
    void gameInProgress(){
        when(level.isInProgress()).thenReturn(true);
        game.start();
        assertThat(game.isInProgress()).isFalse();
    }

    @Test
    //Bottom statement first cond false, bottom loop second condition true
    void NoPlayersAlive(){
        when(level.isInProgress()).thenReturn(false);
        when(level.isAnyPlayerAlive()).thenReturn(false);
        when(level.remainingPellets()).thenReturn(10);
        game.start();
        assertThat(game.isInProgress()).isFalse();
    }

    @Test
    //Bottom statement true, both conditions true
    void newGameStarting(){
        when(level.isInProgress()).thenReturn(false);
        when(level.isAnyPlayerAlive()).thenReturn(true);
        when(level.remainingPellets()).thenReturn(10);
        game.start();
        verify(level).start();
        verify(level).addObserver(game);
        assertThat(game.isInProgress()).isTrue();
    }

    @Test
    //Bottom statement second cond false, bottom loop first condition true
    void noPellets(){
        when(level.isInProgress()).thenReturn(false);
        when(level.isAnyPlayerAlive()).thenReturn(true);
        when(level.remainingPellets()).thenReturn(0);
        game.start();
        assertThat(game.isInProgress()).isFalse();
    }
}