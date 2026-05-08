# Buzzer MVP

This workspace now contains the MVP version of the buzzer system:

- [master.ino](master.ino)
- [slave.ino](slave.ino)

The temporary `_minimal` sketches have been retired.

## What the MVP does

- Master listens for buzzes.
- First valid buzz locks the round.
- Master broadcasts the winning buzzer ID.
- Only the winning slave turns its LED on.
- Short press on the master reset button clears the round.
- Long press on the master reset button lights all slaves while held, for range testing.

## Hardware assumptions

- Master reset button: pin 10
- Master indicator LED: pin 11
- Slave button: pin 10
- Slave LED: pin 7
- All boards share the same nRF channel and address

## Typical workflow

1. Flash [master.ino](master.ino) to the referee board.
2. Flash [slave.ino](slave.ino) to each buzzer board, giving each one a unique ID.
3. Press the master reset button briefly to arm a round.
4. Players buzz in.
5. The master declares one winner.
6. Hold the master reset button for a range test; release to turn all buzz LEDs off.

## Documents

- [PROTOCOL_ANALYSIS.md](PROTOCOL_ANALYSIS.md) explains why this protocol is the right MVP shape.
- [ARCHITECTURE_DEEP_DIVE.md](ARCHITECTURE_DEEP_DIVE.md) describes the master/slave design at a higher level.
- [SETUP_TESTING_GUIDE.md](SETUP_TESTING_GUIDE.md) gives the practical test sequence.
