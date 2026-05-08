# Architecture Deep Dive

## Core model

The current build uses a simple star network:

- one master as referee
- many slaves as buzzers
- one shared RF channel
- the master is the source of truth for round state

That is the right shape for a quiz buzzer MVP because it keeps the logic centralized and easy to reason about.

## Why the current approach is sensible

1. The master normally listens, which keeps the round open and responsive.
2. Each slave sends one short buzz packet only when its button is pressed.
3. The master locks the round on the first valid buzz it receives.
4. The master broadcasts a winner ID so every slave can decide locally whether to light its LED.
5. A separate reset broadcast cleanly reopens the round.
6. A long press test broadcast is useful for checking range and coverage without starting a real round.

This is much simpler than a retry-heavy or queue-heavy design, and it matches the current MVP requirement.

## About half duplex

The radio is not truly full duplex. If the master transmits, it is effectively deaf during that transmit window. That is fine for this MVP because the master only transmits after it has already decided the result.

In other words:

- receive while the round is open
- transmit only after the first buzz is accepted
- go back to receive after the broadcast

That pattern avoids the race condition you were worried about.

## Why no queue yet

You asked to stop at one winner for now, and that is the right choice.

Adding a second-place queue later is still possible, but it adds complexity in exactly the place where the MVP needs to stay simple:

- tracking buzz order
- deciding what counts as near-simultaneous
- deciding how long to wait before locking the winner

For now, the minimal version proves the hardware, proves the protocol, and supports multiple buzzers reliably enough for the MVP.

## Long-press range test

The hold-to-test feature is deliberately separate from normal round flow.

- short press: reset and reopen the round
- long press: light all slaves while held
- release: immediately send the off/reset signal

That makes it a useful installation test and avoids mixing diagnostics with game logic.

## What to add later if needed

If the project grows, the next likely additions are:

- a short capture window before winner broadcast
- second-place handling
- score tracking
- a stronger reset/arming state machine

Those are optional future layers, not prerequisites for the current MVP.
