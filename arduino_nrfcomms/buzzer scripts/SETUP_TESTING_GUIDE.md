# Setup & Testing Guide

## Files to flash

- [master.ino](master.ino) to the referee board
- [slave.ino](slave.ino) to every buzzer board

Do not use the retired `_minimal` files anymore.

## Before flashing slaves

Each slave must have a unique ID.

In [slave.ino](slave.ino), change:

```cpp
uint8_t my_id = 3;
```

Use `1`, `2`, `3`, and so on for each board.

## Hardware pins

- Master reset button: pin 10
- Master status LED: pin 11
- Slave button: pin 10
- Slave LED: pin 7

## Basic test flow

1. Open Serial Monitor on both boards at 115200 baud.
2. Press the master reset button briefly.
3. Press one slave button.
4. Confirm the master prints a winner and only that slave lights its LED.
5. Press and hold the master reset button for about 2 seconds.
6. Confirm all slave LEDs light while the button is held.
7. Release the master button and confirm all slave LEDs turn off immediately.

## Multi-slave test

1. Flash several slaves with different IDs.
2. Press the master reset button briefly.
3. Buzz two or more slaves close together.
4. Confirm only one winner is declared.
5. Repeat a few rounds to confirm the protocol stays stable.

## What to look for on Serial

Master:

```text
BUTTON DOWN
ROUND RESET
WINNER: 3
TEST ALL SENT
TEST ALL OFF
```

Slave:

```text
ROUND OPEN
Button pressed, buzzing ID: 3
Write result: 1
I WON
Winner is 2
TEST ALL ON
```

## If something looks wrong

- If no slave ever buzzes, check the radio wiring and confirm both boards are on the same channel and address.
- If only one direction works, swap the radio modules to separate wiring issues from module issues.
- If the test-all mode works but buzzing does not, the button wiring or ID setup is the likely problem.
