#ifndef INTEGRATOR_FACTORY_H_
#define INTEGRATOR_FACTORY_H_

#include "IntegratorBase.hpp"

#include <stdexcept>

#include "gloo/utils.hpp"

#include "IntegratorType.hpp"
#include "ForwardEulerIntegrator.hpp"
#include "TrapIntegrator.hpp"
#include "RK4Integrator.hpp"

namespace GLOO {
class IntegratorFactory {
 public:
  template <class TSystem, class TState>
  static std::unique_ptr<IntegratorBase<TSystem, TState>> CreateIntegrator(
      IntegratorType type) {
    if (type == IntegratorType::Euler) {
      return make_unique<ForwardEulerIntegrator<TSystem, TState>>();
    }
    else if (type == IntegratorType::Trapezoidal) {
      return make_unique<TrapIntegrator<TSystem, TState>>();
    }
    else if (type == IntegratorType::RK4) {
      return make_unique<RK4Integrator<TSystem, TState>>();
    }
    else {
      std::cerr << "type mismatch" << std::endl;
      return nullptr;
    }
    
    // return make_unique<IntegratorBase<ParticleSystemBase, type>>();  
  }
};
}  // namespace GLOO

#endif
