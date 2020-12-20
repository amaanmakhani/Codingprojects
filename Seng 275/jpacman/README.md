# JPacman


## About

Pacman-like game used for teaching software testing.
It exposes students to the use of git, Gradle, JUnit, and mockito.

Parts of the code are well tested, whereas others are left untested intentionally. As a student in software testing, you can extend the test suite, or use the codebase to build extensions in a test-driven way. As a teacher, you can use JPacman to create your own testing exercises.

## Getting Started

### IntelliJ
1. Git clone the project
2. Open IntelliJ and create new project "from existing sources"
3. Select 'Gradle' in the following screen as external model, and click 'Next'
4. In the next screen, optionally adjust the Gradle options and click 'Finish'
5. To see JPacman in action: run `Launcher`
5. To run the test suite in IntelliJ: right click on a test or directory -> `Run` or `Run ...Test`

### Command line
1. Git clone the project
2. To see JPacman in action: `./gradlew run`
3. To run the test suite and static analysis tools: `./gradlew check`
    1. For tests only run `./gradlew test`
    2. For static analysis tools only run `./gradlew staticAnalysis`
	 
# Answers for Lab 0
- Ex 3: Why can’t we exhaustively test our entire software project? What should we do instead?  
    We cannot exhaustively test our software because in reality we do not have unlimited resources. Often there are to many components to test so it is impossible to test all combinations of the components. Instead we should write test cases to guide us in development and give us confidence in our changes. We should also prioritize test cases as bugs are not uniformly distributed. Testing is great but since we cannot guarantee a software system has no bugs, we should also focus on varying the type of tests too.

- Ex 4: What is the pesticide paradox about and what does it imply to software testers?  
    The pesticide paradox is when testers apply the same testing techniques over and over and they lose their efficiency. For software testers no single testing strategy can guarantee that the software under test is bug-free. Testers have to use different testing strategies to minimize the number of bugs that are remaining.

- Ex 5: Why should we automate, as much as possible, the test execution?  
    To modify code and ensure it still behaves as expected automated tests are a vital part of software development. A few advantages of automated tests are they run faster than manual tests. Automated tests also bring confidence when refactoring code as they can be run to ensure no changes have broken other components. Automated tests also have less mistakes then when executing manual tests as no human error should be involved. When writing automated tests developers who write tests also spend less time debugging their software. These are all reasons to automate testing.
    
# Answers for Lab 3, Part 2
- Ex 4: CollisionInteractionMap is one class that has 0/9 methods tested, 0/2 classes tested, and 0/51 lines tested. This class was not tested well becuase the smoke test only covers basic game interactions and a collision is not consisted within these actions.
The JPacmanFuzzer class also is poorly tested as 0/1 classes are tested, 0/6 methods are tested, and 0/37 lines are tested. The fuzzer tries to figure out random moves and writes logs in output files. The number of runs is a tunable parameter. Each run initiates the game and is played until PacMan dies. This simulation feature wouldn't be required in the smoke test as this is not part of the basic functionality.

- Ex 5: Move is fully covered as all lines are tested. The error that occurred once move was removed was the assertation failing as it expected a value of 10 and the actual value returned was 0. This would be helpful as before implementing new features you can see the smoke test fails, therefore you can start a real game and see what happens. This will allow you to locate the problem based on your knowledge of the expected behaivour. You can also see that a score of 10 was expected and only 0 points were earned.

- Ex 6: I could not tell where the problem was the only difference I saw is the board disappeared faster than the normal smoke test.

- Ex 7: Game and level classes are related because game implements level. So this means that each game has a level which could be assumed to set the difficulty of the game.
The level class also has an object of the board class, so the level class controls the layout of the board.
The level class also has instances of unit to customize the occupants of the board based on the level.
The unit class occupies a square if possible, this is the equivalent to moving the object to assigned board spots if possible.

# Answers for Lab 4
What can we do to avoid code repetition during the Arrange part of the unit test?

When we are setting up our tests we might need to reuse a large amount of test code. This could be to create new instances or reinitialize variables. To reduce this we can use a before each block. This will allow certain lines of code to be run every time without repetition. We could also use a before all block for any code that is required prior to any fresh instances. Another important item is to design your code with testability in mind. This means using TDD too setup your classes to be testing friendly and easy to arrange(set up).

Note that our tests always make use of “clean instances” of the class under test. See BoardFactoryTest as an example: the setUp() always instantiates a new BoardFactory instance.
What are the advantages of such approach?

Clean instances are really important because many software bugs are caused by previous/prexisting states. By using setup to create new instances we can reset object states to overwrite any prexisting class data. Another advantage is to see the amount of code required for setup. When evaluating the amount of code needed we can reflect on the setup and organization of our classes. This allows us to learn new techniques to better organize our classes during the TDD phase. Those are the two main advantages of clean instances in the setup block.

# Answers for Lab 6, part 2
#### See the Ghost#randomMove() method. It makes use of Java’s Random class to generate random numbers. How would you test such method, if everytime you execute the method you get a different answer? Explain your idea (max 100 words)

Java’s random number class can be tested in a few different ways. We can test the method by storing the previous test result and ensuring it is not equal to the test result you just received. We expect it to hold its randomness property so we can assert the previous result should not be the same as the current result. Another way to test this method more rigorously would be to run it a certain number of time and store all results in an array and ensure the current return value is not contained in the array of previous results.

#### You made intensive use of mocks in this assignment. So, you definitely know its advantages.But, in your opinion, what are the main disadvantages of such approach? Explain your reasons. (max 100 words)

One disadvantage of using mocks is the fact that mocks aren’t objects or instances of a class with real functionality. So, mocks are good for unit tests where you aren’t interested in testing interactions between classes. Another issue is that mocks prevent us from seeing how difficult it is to instantiate our classes which could be troubling. If it is difficult to build our classes, we might want to refactor our code as we want to ensure our code is maintainable. Another disadvantage is if you mock too much you end up with tests that have to much predefined behaviour and tests nothing.

# Answers for Lab 7
#### Exercise 3:
Consider scenarios 2.4 and 2.5. 
Explain why it is harder to create system test cases (when compared to your previous experience with unit testing) for these scenarios.

It is harder to write these system cases because it involves multiple classes and simulated maps. Creating these maps and the infrastructure required to use these maps.
When writing unit tests we also tested each unit's movement and functionality separately. In these tests we are also dealing with the added overhead of the game. 
This means interacting with the game class and the interaction between all the units. This also deals with collisions between units and the effects of these collisions.

#### Exercise 5:
For User Story 3 (moving monsters).
why is it harder to create system test cases (when compared to your previous experience with unit testing) for these scenarios.

This scenario is harder to write a system test for because the ghost movement is random and automatically. 
Since these moves are automatic we also cannot control the moves that occur and how long the collision might occur in.
Similar to exercise 3 we will have to create the maps to simulate and control the interactions as best as possible.
It is also difficult to see the visual interaction because the game iss run in the background.