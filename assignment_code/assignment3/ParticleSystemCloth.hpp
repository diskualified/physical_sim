#ifndef PARTICLE_SYSTEM_CLOTH_H_
#define PARTICLE_SYSTEM_CLOTH_H_

#include "ParticleSystemBase.hpp"

namespace GLOO {
class ParticleSystemCloth: public ParticleSystemBase {
 public:
  virtual ~ParticleSystemBase() {
  }

  virtual ParticleState ComputeTimeDerivative(const ParticleState& state,
                                              float time) const = 0;
};
}  // namespace GLOO

#endif
