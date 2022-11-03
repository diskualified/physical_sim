#include "PendulumNode.hpp"
#include "IntegratorFactory.hpp"
#include "gloo/components/ShadingComponent.hpp"
#include "gloo/components/RenderingComponent.hpp"

#include <glm/gtx/string_cast.hpp>

namespace GLOO {
PendulumNode::PendulumNode(IntegratorType integrator_type, float integrator_step) : transform_(*this), parent_(nullptr), active_(true) {
    state = ParticleState();
    integrator = IntegratorFactory::CreateIntegrator<ParticleSystemPendulum, ParticleState>(integrator_type);
    system = ParticleSystemPendulum();
    step = integrator_step;

    float k = 4.;
    float r = 0.5;

    float y = 1.;
    for (int i = 0; i < 5; ++i) {
      system.AddParticle(state, glm::vec3(-1., y, 0.), glm::vec3(0.05, -1, 0.));
      system.fixed.push_back(false);
      if (i > 0) {
        system.AddSpring(i-1, i, r, k);
      }
      r -= 0.1;
      k -= 0.01;
      y -= 0.01;

      auto sphere_node = make_unique<SceneNode>();
      sphere_node->GetTransform().SetPosition(state.positions[i]);
      sphere_node->CreateComponent<RenderingComponent>(sphere_mesh_);
      sphere_node->CreateComponent<ShadingComponent>(shader_);
      sphere_node_ptrs.push_back(sphere_node.get());
      AddChild(std::move(sphere_node));
    }

    system.FixParticle(0);
}

void PendulumNode::Update(double delta_time) {
  if (step > delta_time) {
    state = integrator->Integrate(system, state, 0, delta_time);
    for (int i = 0; i < sphere_node_ptrs.size(); ++i) {
      sphere_node_ptrs[i]->GetTransform().SetPosition(state.positions[i]);
    }
  }
  else {
    float j;
    for (j = 0; j < delta_time; j += step) {
        // integrate 
        state = integrator->Integrate(system, state, j, step);
        for (int i = 0; i < sphere_node_ptrs.size(); ++i) {
          sphere_node_ptrs[i]->GetTransform().SetPosition(state.positions[i]);
        }
    }
    // remainder
    if (j > 0) {
        state = integrator->Integrate(system, state, j, delta_time - j);
        for (int i = 0; i < sphere_node_ptrs.size(); ++i) {
          sphere_node_ptrs[i]->GetTransform().SetPosition(state.positions[i]);
        }
    }
  }

}



}  // namespace GLOO
