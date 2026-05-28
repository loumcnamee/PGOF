# PGOF

Parking Garage of the Future (PGOF) reference solution in C++.

## Top-level use cases
- Register parking lots with capacities `{1,2,3}`.
- Accept autonomous vehicles in arrival order with:
  - vehicle size `{1,2,3}`
  - requested wait time `>= 1`
- Park cars according to policy:
  1. Before the garage is ever full, park strictly in arrival order.
  2. After the garage has been full at least once, when space opens, park the first eligible queued vehicle.
- Unpark vehicles automatically when their wait time expires.
- Track throughput (`totalParked`) over long periods.

## Architecture
- `ParkingGarage` is the application service and policy engine.
- Core model objects:
  - `Vehicle`
  - `Lot`
  - `ParkedVehicle`
- Internal state:
  - waiting queue (`std::deque<Vehicle>`)
  - lot inventory (`std::vector<Lot>`)
  - simulated clock (`currentTime`)
  - metrics (`totalParked`, `totalUnparked`)

## Data model and policy details
- A lot can hold at most one car.
- A vehicle can park if `lot.size >= vehicle.size`.
- Best-fit placement is used (smallest eligible empty lot) to improve utilization.
- Departure time is `park_time + wait_duration`.

## Build and test
```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

## Test coverage
Focused tests verify:
- FIFO behavior before first full occupancy.
- First-eligible behavior after full occupancy has occurred.
- Eligibility blocking behavior.
- Wait-time expiration and unparking behavior.

## Initial Prompt

I am building a design problem suitable for entry to mid level software developers. The solution is to be implemented in C++. The problem description follows:

1. Autonomous vehicles join the waiting line in order of arrival.
2. Vehicles come in sizes of `{1, 2, 3}`.
3. The parking lot comprises parking with N lots of size `{1, 2, 3}`.
4. The following parking states are allowed:
   - lot = `{1, 2, 3}` holds no cars `{0}`
   - lot = `{1}` holds car = `{1}`
   - lot = `{2}` holds car = `{1, 2}`
   - lot = `{3}` holds car = `{1, 2, 3}` (a lot of size 3 can hold one car of size 1, 2, or 3)
5. Each car arrives with a requested wait time `{1, 2, 3, 4, 5, ...}`.
6. PGOF parks each car in order of arrival until there are no empty lots. After that PGOF can select the first eligible car to park.
7. PGOF unparks cars once the wait time expires.
8. The goal is maximize the number of cars that can be parked over a given time period `>> 10` time units.

Design and implement PGOF in C++.

Key behaviors sought in successful solutions:
1. Ensuring top level use cases are well defined.
2. Definition of a system architecture that scales and satisfies best design practices and key quality attributes.
3. A data model that supports the problem.
4. Good use of C++ language features to implement PGOF.
5. Design and implementation of tests to ensure PGOF is defect free.
