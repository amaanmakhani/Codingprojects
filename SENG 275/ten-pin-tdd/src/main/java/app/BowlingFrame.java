package app;

import java.util.ArrayList;

public class BowlingFrame {
    private final ArrayList<Integer> rolls;

    public BowlingFrame(ArrayList<Integer> frameRolls) {
        rolls = frameRolls;
    }

    public int score() {
        int total = 0;
        for (int roll: rolls) {
            total += roll;
        }
        return total;
    }
}
