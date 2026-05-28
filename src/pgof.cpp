#include "pgof.hpp"

#include <algorithm>

namespace pgof {

namespace {

void validateSize(int size, const char* fieldName) {
  if (size < 1 || size > 3) {
    throw std::invalid_argument(std::string(fieldName) + " must be in [1, 3]");
  }
}

}  // namespace

ParkingGarage::ParkingGarage(const std::vector<int>& lotSizes) {
  lots_.reserve(lotSizes.size());
  for (std::size_t i = 0; i < lotSizes.size(); ++i) {
    validateSize(lotSizes[i], "lot size");
    lots_.push_back(Lot{.id = i + 1, .size = lotSizes[i], .parked = std::nullopt});
  }
}

std::size_t ParkingGarage::enqueueVehicle(int size, int requestedWaitTime) {
  validateSize(size, "vehicle size");
  if (requestedWaitTime < 1) {
    throw std::invalid_argument("requested wait time must be >= 1");
  }

  const std::size_t id = nextVehicleId_++;
  waiting_.push_back(Vehicle{.id = id, .size = size, .waitDuration = requestedWaitTime});
  parkVehicles();
  return id;
}

void ParkingGarage::advanceTime(int timeUnits) {
  if (timeUnits < 1) {
    throw std::invalid_argument("timeUnits must be >= 1");
  }

  for (int i = 0; i < timeUnits; ++i) {
    ++currentTime_;
    processDepartures();
    parkVehicles();
  }
}

std::size_t ParkingGarage::occupiedLotCount() const {
  return static_cast<std::size_t>(std::count_if(lots_.begin(), lots_.end(), [](const Lot& lot) {
    return lot.parked.has_value();
  }));
}

bool ParkingGarage::hasEmptyLot() const {
  return occupiedLotCount() < lots_.size();
}

std::optional<std::size_t> ParkingGarage::findBestFitEmptyLot(int vehicleSize) const {
  std::optional<std::size_t> best;
  for (std::size_t i = 0; i < lots_.size(); ++i) {
    if (lots_[i].parked.has_value() || lots_[i].size < vehicleSize) {
      continue;
    }

    if (!best.has_value() || lots_[i].size < lots_[*best].size) {
      best = i;
    }
  }
  return best;
}

bool ParkingGarage::parkVehicleAtQueueIndex(std::size_t queueIndex) {
  if (queueIndex >= waiting_.size()) {
    return false;
  }

  const Vehicle vehicle = waiting_[queueIndex];
  const auto lotIndex = findBestFitEmptyLot(vehicle.size);
  if (!lotIndex.has_value()) {
    return false;
  }

  lots_[*lotIndex].parked = ParkedVehicle{.vehicle = vehicle, .departureTime = currentTime_ + vehicle.waitDuration};
  waiting_.erase(waiting_.begin() + static_cast<std::ptrdiff_t>(queueIndex));
  ++totalParked_;

  if (occupiedLotCount() == lots_.size()) {
    hasBeenFull_ = true;
  }

  return true;
}

bool ParkingGarage::parkFirstEligibleVehicle() {
  for (std::size_t i = 0; i < waiting_.size(); ++i) {
    if (findBestFitEmptyLot(waiting_[i].size).has_value()) {
      return parkVehicleAtQueueIndex(i);
    }
  }
  return false;
}

void ParkingGarage::processDepartures() {
  for (Lot& lot : lots_) {
    if (!lot.parked.has_value()) {
      continue;
    }
    if (lot.parked->departureTime <= currentTime_) {
      lot.parked = std::nullopt;
      ++totalUnparked_;
    }
  }
}

void ParkingGarage::parkVehicles() {
  while (hasEmptyLot()) {
    if (waiting_.empty()) {
      break;
    }

    const bool parked = hasBeenFull_ ? parkFirstEligibleVehicle() : parkVehicleAtQueueIndex(0);
    if (!parked) {
      break;
    }
  }
}

}  // namespace pgof
