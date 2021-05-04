# Real Time Computer Systems Design Project
- Techniques to guarantee deadline sensitive tasks complete by their deadline
- FreeRTOS was used as the real time operating system
- Traffic Light System:
  - A program simulates a traffic light, and a row of LEDs to represent the vehicle traffic. This is aimed to recreate a one way, one-lane road. To adjust the flow of traffic the potentiometer value can be increased which will result in more cars being produced. If a LED within the lane row is on this indicates a car is present in this location.
- Deadline Driven Scheduler:
  - This scheduler was intended to manage tasks which occur in a dynamic environment. Each task managed has a hard deadline. The algorithm behind this scheduler was the Earliest Deadline First (EDF) algorithm. The EDF algorithm was chosen as it is aimed to maximize processor utilization and might be more able to produce a feasible schedule. EDF is not built into the FreeRTOS system so this scheduler is a customized solution built on top of the scheduler.