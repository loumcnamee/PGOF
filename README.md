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
