#pragma once

#include <cstddef>
#include <deque>
#include <optional>
#include <stdexcept>
#include <vector>

namespace pgof {

struct Vehicle {
  std::size_t id;
  int size;
  int waitDuration;
};

struct ParkedVehicle {
  Vehicle vehicle;
  int departureTime;
};

struct Lot {
  std::size_t id;
  int size;
  std::optional<ParkedVehicle> parked;
};

class ParkingGarage {
 public:
  explicit ParkingGarage(const std::vector<int>& lotSizes);

  std::size_t enqueueVehicle(int size, int requestedWaitTime);
  void advanceTime(int timeUnits = 1);

  int currentTime() const { return currentTime_; }
  std::size_t totalParked() const { return totalParked_; }
  std::size_t totalUnparked() const { return totalUnparked_; }
  std::size_t waitingCount() const { return waiting_.size(); }
  std::size_t occupiedLotCount() const;
  std::size_t lotCount() const { return lots_.size(); }

  const std::vector<Lot>& lots() const { return lots_; }
  const std::deque<Vehicle>& waitingLine() const { return waiting_; }

 private:
  bool hasEmptyLot() const;
  std::optional<std::size_t> findBestFitEmptyLot(int vehicleSize) const;
  bool parkVehicleAtQueueIndex(std::size_t queueIndex);
  bool parkFirstEligibleVehicle();
  void processDepartures();
  void parkVehicles();

  std::deque<Vehicle> waiting_;
  std::vector<Lot> lots_;

  int currentTime_ = 0;
  std::size_t nextVehicleId_ = 1;
  std::size_t totalParked_ = 0;
  std::size_t totalUnparked_ = 0;
  bool hasBeenFull_ = false;
};

}  // namespace pgof
