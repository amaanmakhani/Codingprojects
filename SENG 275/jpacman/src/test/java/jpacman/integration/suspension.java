package jpacman.integration;

import jpacman.Launcher;
import jpacman.game.Game;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;

public class suspension {
    private Game game;
    private Launcher launcher;

    @BeforeEach
    public void setup(){
        launcher = new Launcher();
        launcher.launch();
        game = launcher.getGame();
        game.start();
    }

    @AfterEach
    void teardown(){
        launcher.dispose();
    }

    // EX 1
    // Scenario 4.1
    @Test
    public void SuspendRunningGame(){
        // Given the game has started
        assertTrue(game.isInProgress());
        // when the player clicks the "Stop" button
        game.stop();
        // then all moves from ghosts and the player are suspended.
        assertFalse(game.isInProgress());
    }

    // EX 1
    // Scenario 4.2
    @Test
    public void RestartSuspendedGame(){
        // Given the game is suspended;
        game.stop();
        assertFalse(game.isInProgress());
        // when the player hits the "Start" button
        game.start();
        // then the game is resumed.
        assertTrue(game.isInProgress());
    }
}