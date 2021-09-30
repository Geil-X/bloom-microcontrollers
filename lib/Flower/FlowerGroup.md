# Flower Group Execution

The `FlowerGroup.h` file controls several flower motors at the same time. This class is entirely static, so you can't
create any object with it.

## Threading

* Each flower motor is given its own thread pool.
* Flower operations must run on their own threads so that they can all be run simultaneously.
* When a new action is called on the same flower motor, the previous actions are canceled.