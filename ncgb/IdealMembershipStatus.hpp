#pragma once

namespace ncgb {

// Return value for inIdeal functions
enum IdealMembershipStatus {
  InIdeal,    // The element is definitely in the ideal
  NotInIdeal, // The element is definitely not in the ideal
  Unknown     // More steps are needed to determine if the element is in the ideal
};

} // namespace ncgb
