
# ticktack

- Measure the execution time of a specific range of a program.
- Use labels (strings) to distinguish between different timing regions.
- You can think of it as having a separate stopwatch for each label.
- If the same label is used multiple times, the execution times are accumulated.
- A label can represent an entire function or just a part of it, such as a loop.
- To measure a region, call:
    `__tick("label");` at the beginning and
    `__tack("label");` at the end of that region.
- To measure the total execution time of the program, be sure to measure the main function:
    ```c
    __tick("main");
    ...
    __tack("main");
    ```
- At the end, call `__ticktack_print();` to print the measurement results.
- The first (index 0) stopwatch in the result list is assumed to be for total execution time.
- Labels must be unique strings.
- When measuring function execution time, using the function name as the label is intuitive.
  In such cases, the C macro `__func__` is convenient.
- Since combining with `__func__` is frequently used, the following macros are defined in `ticktack.h`:
    ```c
    __tick_    // equivalent to __tick(__func__)
    __tack_    // equivalent to __tack(__func__)
    __toe_     // equivalent to __ticktack_print()
    ```

# Usage Example

```c
int main()
{
  __tick_; // start main

  ...
  func1();
  ...
  func2();
  ...
  __tack_; // stop main
  __toe_;  // print
}

void func1()
{
  __tick_;             // start func1
  __tick("part_A");    // start Part A
  ...
  __tack("part_A");    // stop Part A
  __tick("part_B");    // start Part B
  ...
  __tack("part_B");    // stop Part B
  __tack_;             // stop func1
}
```
