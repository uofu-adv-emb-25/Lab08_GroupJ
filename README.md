# Lab 08

Status Badge: ![Lab 08 Status](https://github.com/uofu-adv-emb-25/Lab08_GroupJ/actions/workflows/main.yml/badge.svg)

By Kasey Kemp and Caitlin Mastroianni

## Activity 1
Files used are can_rx.c and can_tx.c, both found in "./src/". We observed expected bus line interaction between the two can transceivers, sending messages approximately every 1 second.

## Activity 2
Files used are can_high.c representing the high priority can transceiver and can_low.c representing the low priority can transceiver, both found in "./src/". We originally couldn't get the high priority "babbling node" to prevent transmission from the lower priority, periodic can transceiver. After tweaking values (setting can ID's to absolute limits for can priority, stripping code to the bare minimum, etc.), we finally observed the high priority "babbling node" (using can_high.c) completely overtaking the can bus line, preventing the lower priority, periodic transceiver (using can_low.c) from transmitting at all.

After introducing a short busy wait, the high priority babbling node still prevented anything else from being transmitted on the bus line. We had issues with the compiler optimizing out the busy loop, but after fixing that, the following was observed:

- Looping at 100 iterations: practically no change
- Looping at 1000 iterations: practically no change
- Looping at 10000 iterations: only occasionally receiving messages when not monitoring low priority side
- Looping at 100000 iterations: consistantly receiving messages from both sides
- Looping at 50000 iterations: consistantly receiving messages from both sides
- Looping at 20000 iterations: consistantly receiving messages from both sides
- Looping at 15000 iterations: lowest value while still consistantly receiving messages from both sides

We found that connecting both transceivers to the serial monitor slightly affected outcomes, most likely due to the extra time it takes to transmit messages. However, this didn't greatly affect outcomes.