#include "PendulumNode.hpp"
#include "IntegratorFactory.hpp"
#include "gloo/components/ShadingComponent.hpp"
#include "gloo/components/RenderingComponent.hpp"
#include "gloo/debug/PrimitiveFactory.hpp"

#include <glm/gtx/string_cast.hpp>

namespace GLOO {
PendulumNode::PendulumNode(IntegratorType integrator_type, float integrator_step) : transform_(*this), parent_(nullptr), active_(true) {
    state = ParticleState();
    state.positions.push_back(glm::vec3(-0.5, 0.5, 0.));
    state.velocities.push_back(glm::vec3(0., -0.5, 0.));

    integrator = IntegratorFactory::CreateIntegrator<ParticleSystemPendulum, ParticleState>(integrator_type);
    system = ParticleSystemPendulum();
    step = integrator_step;

    sphere_mesh_ = PrimitiveFactory::CreateSphere(0.03, 25, 25);
    auto sphere_node = make_unique<SceneNode>();
    sphere_node->GetTransform().SetPosition(state.positions[0]);
    sphere_node->CreateComponent<RenderingComponent>(sphere_mesh_);
    sphere_node->CreateComponent<ShadingComponent>(shader_);
    sphere_node_ptr = sphere_node.get();

    AddChild(std::move(sphere_node));

    sphere_mesh_ = PrimitiveFactory::CreateSphere(0.03, 25, 25);
    auto sphere_node1 = make_unique<SceneNode>();
    // fixed pos
    sphere_node1->GetTransform().SetPosition(glm::vec3(-0.5, 1., 0.));
    sphere_node1->CreateComponent<RenderingComponent>(sphere_mesh_);
    sphere_node1->CreateComponent<ShadingComponent>(shader_);
    AddChild(std::move(sphere_node1));
}

void PendulumNode::Update(double delta_time) {
  if (step > delta_time) {
    state = integrator->Integrate(system, state, 0, delta_time);
    sphere_node_ptr->GetTransform().SetPosition(state.positions[0]);
  }
  else {
    float i;
    for (i = 0; i < delta_time; i += step) {
        // integrate 
        state = integrator->Integrate(system, state, i, step);
        sphere_node_ptr->GetTransform().SetPosition(state.positions[0]);
    }

    // remainder
    if (i > 0) {
        state = integrator->Integrate(system, state, i, delta_time - i);
        sphere_node_ptr->GetTransform().SetPosition(state.positions[0]);
    }
  }
//   std::cout << state.positions[0].x << std::endl;
//   std::cout << state.velocities[0].x << std::endl;

}



}  // namespace GLOO
