package jpacman.game;
import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.*;
import jpacman.Launcher;
import jpacman.board.Direction;
import jpacman.board.Square;
import jpacman.board.Unit;
import jpacman.level.*;
import jpacman.npc.Ghost;
import jpacman.npc.ghost.Navigation;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import static org.assertj.core.api.Assertions.assertThat;

public class GameStateAndTransistionTest {
    private Game game;
    private Launcher launcher;
    private Player player;

    /*
        Using the transition tree located in the docs folder I have derived 5 test cases one for each leaf node.
        CREATED_0 -> PLAYING_0 is completed in the setup before each test.
        1.CREATED_0 -> PLAYING_0 -> WON_0
            The last pellet has been eaten while the player is still alive this means that the level has been won.
        2.CREATED_0 -> PLAYING_0 -> LOST_0
            Pacman is eaten by a ghost, and the level has been lost.
        3.CREATED_0 -> PLAYING_0 -> PLAYING_1
            Player moves to an empty square and the game should still be in progress.
        4.CREATED_0 -> PLAYING_0 -> HALTED_0 -> PLAYING_3
            The player suspends the game then restarts the game so the game should be in progress again.
        5.CREATED_0 -> PLAYING_0 -> PLAYING_2
            Player eats a pellet with one or more pellets left on the board. The game should still be in progress.

        No additional tests were needed.
     */

    // Create game before each test
    @BeforeEach
    public void setup(){
        launcher = new Launcher();
        launcher.launch();
        game = launcher.getGame();
        game.start();
        player = game.getPlayers().get(0);
    }

    @AfterEach
    void teardown(){
        launcher.dispose();
    }

    // Map helper function
    private void setUpCustomMap(String path){
        launcher.dispose();
        launcher = new Launcher();
        launcher = launcher.withMapFile(path);
        launcher.launch();
        game = launcher.getGame();
        game.start();
        player = game.getPlayers().get(0);
    }

    @Test
    void PlayerWinsTheGame(){
        // Create the game
        setUpCustomMap("/onePellet.txt");
        // Check that the game is in a playing state
        assertTrue(game.isInProgress());
        // Pacman is next to a square containing a pellet and move pacman to that square
        assertTrue(player.getSquare().getSquareAt(Direction.EAST).getOccupants().stream().anyMatch(c -> c instanceof Pellet));
        Square pelletSquare = player.getSquare().getSquareAt(Direction.EAST);
        game.move(player, Direction.EAST);
        assertEquals(pelletSquare, player.getSquare());
        // Check that the points have been earned
        assertEquals(10, player.getScore());
        // Check that the pellet disappears from that square
        Unit pellet = Navigation.findUnit(Pellet.class, player.getSquare());
        assertEquals(null, pellet);
        // Check the game is in a won state as no pellets are remaining
        assertFalse(game.isInProgress());
    }

    @Test
    void PlayerEatenByGhost(){
        // Create the game
        setUpCustomMap("/killedByAGhost.txt");
        // Check that the game is in a playing state
        assertTrue(game.isInProgress());
        // Pacman is next to a cell containing a ghost
        assertTrue(player.getSquare().getSquareAt(Direction.NORTH).getOccupants().stream().anyMatch(c -> c instanceof Ghost));
        // Move player to the ghost
        game.move(player, Direction.NORTH);
        // Check that pacman is dead
        assertFalse(player.isAlive());
        // The game should be finished as the player has lost
        assertFalse(game.isInProgress());
    }

    @Test
    void PlayerMovesToEmptySquare(){
        // Create the game
        setUpCustomMap("/pellets.txt");
        // Check that the game is in a playing state
        assertTrue(game.isInProgress());
        // Pacman is next to a square that is empty and move pacman to that square
        assertTrue(player.getSquare().getSquareAt(Direction.WEST).getOccupants().isEmpty());
        Square emptySquare = player.getSquare().getSquareAt(Direction.WEST);
        game.move(player, Direction.WEST);
        assertEquals(emptySquare, player.getSquare());
        // Check that no points have been earned as the square was empty
        assertEquals(0, player.getScore());
        // Check the game is still in a playing state
        assertTrue(game.isInProgress());
    }

    @Test
    void GameIsResumedAfterSuspension(){
        // Check that the game is in a playing state
        assertTrue(game.isInProgress());
        // Suspend the game, check that the game is in the halted state
        game.stop();
        assertFalse(game.isInProgress());
        // Restart the game, and check it returns to the playing state
        game.start();
        assertTrue(game.isInProgress());
    }

    @Test
    void PlayerEatsAPellet(){
        // Create the game
        setUpCustomMap("/pellets.txt");
        // Check that the game is in a playing state
        assertTrue(game.isInProgress());
        // Pacman is next to a square containing a pellet and move pacman to that square
        assertTrue(player.getSquare().getSquareAt(Direction.EAST).getOccupants().stream().anyMatch(c -> c instanceof Pellet));
        Square pelletSquare = player.getSquare().getSquareAt(Direction.EAST);
        game.move(player, Direction.EAST);
        assertEquals(pelletSquare, player.getSquare());
        // Check that the points have been earned
        assertEquals(10, player.getScore());
        // Check that the pellet disappears from that square
        Unit pellet = Navigation.findUnit(Pellet.class, player.getSquare());
        assertEquals(null, pellet);
        // Check the game is still in a playing state
        assertTrue(game.isInProgress());
    }
}