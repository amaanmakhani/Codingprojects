package jpacman.integration;
import jpacman.Launcher;
import jpacman.board.Direction;
import jpacman.board.Square;
import jpacman.board.Unit;
import jpacman.game.Game;
import jpacman.level.Pellet;
import jpacman.level.Player;
import jpacman.npc.ghost.Navigation;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.mockito.internal.matchers.InstanceOf;

import static org.junit.jupiter.api.Assertions.*;

public class playerMovement {
    private Game game;
    private Launcher launcher;
    private Player player;

    // Helper function to create custom map
    private void setUpCustomMap(String path){
        launcher.dispose();
        launcher = new Launcher();
        launcher = launcher.withMapFile(path);
        launcher.launch();
        game = launcher.getGame();
        game.start();
        player = game.getPlayers().get(0);
    }

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

    // EX 2
    // Scenario 2.1
    @Test
    void playerEatsPellet(){
        setUpCustomMap("/pellets.txt");
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
    }

    // EX 2
    // Scenario 2.2
    @Test
    void playerGoesToAnEmptySquare(){
        setUpCustomMap("/nearEmptySquare.txt");
        // Given the game has started
        assertTrue(game.isInProgress());
        // and my Pacman is next to an empty square
        Square emptySpace = player.getSquare().getSquareAt(Direction.EAST);
        assertTrue(emptySpace.getOccupants().isEmpty());
        // when I press an arrow key towards that square
        game.move(player, Direction.EAST);
        // then my Pacman can move to that square
        assertEquals(emptySpace, player.getSquare());
        // and my points remain the same
        assertEquals(0, player.getScore());
    }

    // EX 2
    // Scenario 2.3
    @Test
    void moveFails(){
        setUpCustomMap("/nearWall.txt");
        // Given the game has started
        assertTrue(game.isInProgress());
        // and my Pacman is next to a cell containing a wall
        // wall squares cannot be occupied by a player, so if a player cannot access the square it must be a wall
        assertFalse(player.getSquare().getSquareAt(Direction.WEST).isAccessibleTo(player));
        // when I press an arrow key towards that cell
        Square prev = player.getSquare();
        game.move(player, Direction.WEST);
        // then the move is not conducted
        assertEquals(prev, player.getSquare());
    }
}
