#ifndef SIMPLE_ODE_H_
#define SIMPLE_ODE_H_

#include "ParticleSystemBase.hpp"

namespace GLOO {
class SimpleODE: public ParticleSystemBase {
 public:
  
  ParticleState ComputeTimeDerivative(const ParticleState& state,
                                              float time) const {
    ParticleState new_state = ParticleState();
    new_state.positions.push_back(glm::vec3(-state.positions[0].y, state.positions[0].x, 0));
    new_state.velocities.push_back(glm::vec3(0., 0., 0.));
    return new_state;
  }
};
}  // namespace GLOO

#endif
