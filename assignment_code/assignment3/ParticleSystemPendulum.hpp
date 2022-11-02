#ifndef PARTICLE_SYSTEM_PENDULUM_H_
#define PARTICLE_SYSTEM_PENDULUM_H_

#include "ParticleSystemBase.hpp"

namespace GLOO {
class ParticleSystemPendulum: public ParticleSystemBase {
 public:
  float mass = 3.;
  glm::vec3 g = glm::vec3(0., -9.8, 0.);

  // separate per spring?
  float k = 5.;
  float rest = 0.5;
  // no more: std::vector<SceneNode*> spring_ptrs; // stores indices of connected particle pair, stiffness, rest length

  // std::vector<ParticleState> particles;
  // attach to particles: std::vector<SceneNode*> sphere_node_prts;
  // store connected particle index at each index (vector of vectors)

  void AddParticle(float mass) {
    // TODO
  }
  void AddSpring(ParticleState p1, ParticleState p2) {
    // TODO
  } 
  void FixParticle(ParticleState p) {
    // TODO: set flag so computetimederivative returns 0 derivatives;
    
  }
//   std::vector<ParticleState> A(std::vector<ParticleState> particles_) {
//     std::vector<glm::vec3> x;
//     std::vector<glm::vec3> v;
//     std::vector<glm::vec3> a;
//     for (int i = 0; i < particles_.size(); ++i) {
//         x.push_back(particles_[i].positions[0]);
//         v.push_back(particles_[i].velocities[0]);
//     } 
//     for (int i = 0; i < particles_.size(); ++i) {
//         // float G_i = mass * g;
//         // a[0] = 1/m0 (G_0 + D0(v[0]) + sum of connected particles spring force
//     }
//   }
  // Change this to vector of states?
  ParticleState ComputeTimeDerivative(const ParticleState& state,
                                              float time) const {
    // state change (f)
    ParticleState new_state = ParticleState();
    new_state.positions.push_back(state.velocities[0]);
    // A(x, x')
    glm::vec3 G = mass * g;
    glm::vec3 D = -k * state.velocities[0];
    std::cout << D.x << " " << D.y  << " " << D.z << std::endl;
    glm::vec3 fixed_pt = glm::vec3(-0.5, 1., 0.);
    glm::vec3 d = state.positions[0] - fixed_pt;
    // std::cout << d.x << " " << d.y  << " " << d.z << std::endl;
    // std::cout << glm::length(d) << std::endl;
    glm::vec3 S = -k * (glm::length(d) - rest) * glm::normalize(d);
    std::cout << S.x << " " << S.y  << " " << S.z << std::endl;
    new_state.velocities.push_back((G + D + S)/mass);
    // velocity goes to 0
    std::cout << "old" << state.velocities[0].x << " " << state.velocities[0].y  << " " << state.velocities[0].z << std::endl;

    std::cout << new_state.velocities[0].x << " " << new_state.velocities[0].y  << " " << new_state.velocities[0].z << std::endl;
    return new_state;
  }
};
}  // namespace GLOO

#endif
