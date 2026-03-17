
---

## `Makefile`


```make
CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Werror -Iinclude

SRC = src/arinc429.c src/can_bus.c src/flight_control.c src/sensor_sim.c src/actuator.c src/system_bus.c
APP_SRC = src/main.c
APP = avionics_demo

TEST_ARINC = test_arinc429
TEST_CAN = test_can
TEST_FC = test_flight_control

all: $(APP) $(TEST_ARINC) $(TEST_CAN) $(TEST_FC)

$(APP): $(SRC) $(APP_SRC)
	$(CC) $(CFLAGS) -o $@ $(SRC) $(APP_SRC)

$(TEST_ARINC): src/arinc429.c tests/test_arinc429.c
	$(CC) $(CFLAGS) -o $@ src/arinc429.c tests/test_arinc429.c

$(TEST_CAN): src/can_bus.c tests/test_can.c
	$(CC) $(CFLAGS) -o $@ src/can_bus.c tests/test_can.c

$(TEST_FC): src/flight_control.c tests/test_flight_control.c
	$(CC) $(CFLAGS) -o $@ src/flight_control.c tests/test_flight_control.c

test: $(TEST_ARINC) $(TEST_CAN) $(TEST_FC)
	./$(TEST_ARINC)
	./$(TEST_CAN)
	./$(TEST_FC)

clean:
	rm -f $(APP) $(TEST_ARINC) $(TEST_CAN) $(TEST_FC)