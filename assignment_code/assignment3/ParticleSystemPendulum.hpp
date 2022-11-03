#ifndef PARTICLE_SYSTEM_PENDULUM_H_
#define PARTICLE_SYSTEM_PENDULUM_H_

#include "ParticleSystemBase.hpp"

namespace GLOO {
class ParticleSystemPendulum: public ParticleSystemBase {
 public:
  float mass = 0.1;
  float drag = 0.04;
  glm::vec3 g = glm::vec3(0., -9.8, 0.);
  // make 2d vector that stores 2 indices, rest len, spring constant k
  std::vector<glm::vec4> springs;
  std::vector<bool> fixed;

  void AddParticle(ParticleState& state, glm::vec3 pos, glm::vec3 v) {
    state.positions.push_back(pos);
    state.velocities.push_back(v);
  }
  void AddSpring(int ind1, int ind2, float r, float k) {
    springs.push_back(glm::vec4(ind1, ind2, r, k));
  } 
  void FixParticle(int ind) {
    fixed[ind] = true;
  }
  
  ParticleState ComputeTimeDerivative(const ParticleState& state,
                                              float time) const {
    // state change (f)
    ParticleState new_state = ParticleState();
    for (int i = 0; i < state.positions.size(); ++i) {
      if (fixed[i] == 1 || springs.size() == 0) {
        new_state.positions.push_back(glm::vec3(0.));
        new_state.velocities.push_back(glm::vec3(0.));
      } else {
        new_state.positions.push_back(state.velocities[i]);
        // A(x, x')
        glm::vec3 G = mass * g;
        glm::vec3 D = -drag * state.velocities[i];
        glm::vec3 S = glm::vec3(0.);
        for (int j = 0; j < springs.size(); ++j) {
          if (springs[j].x == i) {
            glm::vec3 d = state.positions[i] - state.positions[springs[j].y];
            S += -springs[j].w * (glm::length(d) - springs[j].z) * glm::normalize(d);
          } else if (springs[j].y == i) {
            glm::vec3 d = state.positions[i] - state.positions[springs[j].x];
            S += -springs[j].w * (glm::length(d) - springs[j].z) * glm::normalize(d);
          }
        }
        new_state.velocities.push_back((G + D + S)/mass);
      }
    }
    return new_state;
  }
};
}  // namespace GLOO

#endif
