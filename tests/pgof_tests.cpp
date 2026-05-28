#include "pgof.hpp"

#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>

using pgof::ParkingGarage;

namespace {

void expect(bool condition, const std::string& message) {
  if (!condition) {
    throw std::runtime_error(message);
  }
}

void parksInArrivalOrderUntilFull() {
  ParkingGarage garage({1, 2});

  garage.enqueueVehicle(2, 5);
  garage.enqueueVehicle(1, 5);

  expect(garage.occupiedLotCount() == 2, "both lots should be occupied");
  expect(garage.waitingCount() == 0, "no vehicles should be waiting");
  expect(garage.totalParked() == 2, "two vehicles should be parked");
  expect(garage.lots()[0].parked->vehicle.size == 1, "size-1 lot should hold size-1 vehicle");
  expect(garage.lots()[1].parked->vehicle.size == 2, "size-2 lot should hold size-2 vehicle");
}

void blocksAtFrontBeforeGarageEverFull() {
  ParkingGarage garage({1, 1});

  garage.enqueueVehicle(2, 3);
  garage.enqueueVehicle(1, 3);

  expect(garage.occupiedLotCount() == 0, "no vehicle should park before first is eligible");
  expect(garage.waitingCount() == 2, "both vehicles should remain queued");
}

void afterFullParksFirstEligibleVehicle() {
  ParkingGarage garage({1, 3});

  garage.enqueueVehicle(1, 1);
  garage.enqueueVehicle(3, 5);
  garage.enqueueVehicle(3, 5);
  garage.enqueueVehicle(1, 5);

  garage.advanceTime(1);

  expect(garage.currentTime() == 1, "time should advance");
  expect(garage.occupiedLotCount() == 2, "garage should remain full");
  expect(garage.lots()[0].parked.has_value(), "size-1 lot should be occupied");
  expect(garage.lots()[0].parked->vehicle.size == 1,
         "after full, first eligible should park in size-1 lot");
  expect(garage.waitingCount() == 1, "one larger vehicle should still be waiting");
  expect(garage.waitingLine().front().size == 3, "remaining waiting vehicle should be size-3");
}

void waitExpirationUnparksCars() {
  ParkingGarage garage({2});

  garage.enqueueVehicle(1, 2);
  expect(garage.occupiedLotCount() == 1, "vehicle should initially park");

  garage.advanceTime(1);
  expect(garage.occupiedLotCount() == 1, "vehicle should still be parked at t=1");

  garage.advanceTime(1);
  expect(garage.occupiedLotCount() == 0, "vehicle should depart at t=2");
  expect(garage.totalUnparked() == 1, "one vehicle should have departed");
}

}  // namespace

int main() {
  try {
    parksInArrivalOrderUntilFull();
    blocksAtFrontBeforeGarageEverFull();
    afterFullParksFirstEligibleVehicle();
    waitExpirationUnparksCars();
  } catch (const std::exception& ex) {
    std::cerr << "Test failure: " << ex.what() << '\n';
    return EXIT_FAILURE;
  }

  std::cout << "All PGOF tests passed.\n";
  return EXIT_SUCCESS;
}
