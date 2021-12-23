package jpacman.integration;
import jpacman.Launcher;
import jpacman.board.Direction;
import jpacman.board.Square;
import jpacman.board.Unit;
import jpacman.game.Game;
import jpacman.level.Level;
import jpacman.level.MapParser;
import jpacman.level.Pellet;
import jpacman.level.Player;
import jpacman.npc.Ghost;
import jpacman.npc.ghost.Navigation;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;

public class endingConditions {
    private Game game;
    private Launcher launcher;
    private Player player;

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

    // Helper map function
    private void setUpCustomMap(String path){
        launcher.dispose();
        launcher = new Launcher();
        launcher = launcher.withMapFile(path);
        launcher.launch();
        game = launcher.getGame();
        game.start();
        player = game.getPlayers().get(0);
    }

    // EX 4
    // Scenario 2.4
    @Test
    void pacManDies(){
        setUpCustomMap("/killedByAGhost.txt");
        // Given the game has started
        assertTrue(game.isInProgress());
        // and my Pacman is next to a cell containing a ghost
        assertTrue(player.getSquare().getSquareAt(Direction.NORTH).getOccupants().stream().anyMatch(c -> c instanceof Ghost));
        // when I press an arrow key towards that square
        game.move(player, Direction.NORTH);
        // then my Pacman dies
        assertFalse(player.isAlive());
        // and the game is over.
        assertFalse(game.isInProgress());
    }

    // EX 4
    // Scenario 2.5, extends Scenario 2.1
    @Test
    void pacManWins(){
        setUpCustomMap("/onePellet.txt");
        // Given the game has started
        assertTrue(game.isInProgress());
        // and my Pacman is next to a square containing a pellet
        assertTrue(player.getSquare().getSquareAt(Direction.EAST).getOccupants().stream().anyMatch(c -> c instanceof Pellet));
        // when I press an arrow key towards that square
        Square pelletSquare = player.getSquare().getSquareAt(Direction.EAST);
        game.move(player, Direction.EAST);
        // then my Pacman can move to that square
        assertEquals(pelletSquare, player.getSquare());
        // and I earn the points for the pellet
        assertEquals(10, player.getScore());
        // and the pellet disappears from that square
        Unit pellet = Navigation.findUnit(Pellet.class, player.getSquare());
        assertEquals(null, pellet);

        // Since I have eaten the last pellet then I win the game.
        assertFalse(game.isInProgress());
    }
}