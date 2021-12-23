package app;

import java.util.ArrayList;

public class BowlingGame {

    public static final int NUMBER_OF_FRAMES = 10;
    private final int[] rolls;

    public BowlingGame(int[] rolls) {
        this.rolls = rolls;
    }

    public int score() {
        int result = 0;
        ArrayList<BowlingFrame> frames = createFrames();
        for (BowlingFrame frame: frames) {
            result += frame.score();
        }
        return result;
    }

    private ArrayList<BowlingFrame> createFrames() {
        ArrayList<BowlingFrame> frames = new ArrayList<>();
        int rollIndex = 0;
        for (int i = 0; i < NUMBER_OF_FRAMES; i++) {
            ArrayList<Integer> frameRolls = new ArrayList<>();
            frameRolls.add(rolls[rollIndex]);
            frameRolls.add(rolls[rollIndex + 1]);
            // Checks if spare
            if (rolls[rollIndex] + rolls[rollIndex + 1] == 10) {
                frameRolls.add(rolls[rollIndex + 2]);
            }
            else if(rolls[rollIndex] == 10) {
                frameRolls.add(rolls[rollIndex + 2]);
                // Increment only one frame because of strike
                rollIndex += 1;
                frames.add(new BowlingFrame(frameRolls));
                continue;
            }
            // Increment rolls to next frame
            rollIndex += 2;
            frames.add(new BowlingFrame(frameRolls));
        }
        return frames;
    }



}
