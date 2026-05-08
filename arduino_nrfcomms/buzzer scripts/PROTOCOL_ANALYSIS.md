# MVP Protocol

This build uses a simple master-orchestrated star topology.

## Message types

- `0x10` buzz from slave, low nibble is the slave ID
- `0x20` winner announcement from master, low nibble is the winner ID
- `0x30` round reset from master
- `0x40` range test / light-all command from master

## Why this works for the MVP

1. The master stays in receive mode for the normal round.
2. Slaves only transmit a single short message when their button is pressed.
3. The first buzz the master sees becomes the winner.
4. The master then broadcasts that winner ID to every slave.
5. Slaves only light their LED when the winner ID matches their own ID.
6. A master long press is reserved for range testing and does not start a round.

This keeps the protocol simple, deterministic, and easy to extend later.

## Current behavior

- Short press on master reset button: clear the round and reopen buzzing.
- Long press on master reset button: light all slave LEDs while held.
- Slave button press during an open round: send one buzz message and wait for the winner broadcast.

## Why no queue yet

We deliberately stopped at one winner for now because the core goal is a stable MVP. The architecture still leaves room to add second-place handling later without changing the basic message format.

## Practical notes

- The radio is half-duplex, so the master should only transmit after it has decided the winner.
- The current implementation already proves the bidirectional link and multi-slave addressing.
- If needed later, a short capture window can be added before winner broadcast without introducing a full queue.
