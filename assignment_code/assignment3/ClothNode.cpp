#include "ClothNode.hpp"
#include "IntegratorFactory.hpp"
#include "gloo/components/ShadingComponent.hpp"
#include "gloo/components/RenderingComponent.hpp"
#include "gloo/debug/PrimitiveFactory.hpp"
#include "gloo/components/MaterialComponent.hpp"
#include "gloo/InputManager.hpp"

#include <glm/gtx/string_cast.hpp>

namespace GLOO {
ClothNode::ClothNode(IntegratorType integrator_type, float integrator_step) : transform_(*this), parent_(nullptr), active_(true) {
    state = ParticleState();
    integrator = IntegratorFactory::CreateIntegrator<ParticleSystemPendulum, ParticleState>(integrator_type);
    system = ParticleSystemPendulum();
    step = integrator_step;

    cloth_mesh_ = std::make_shared<VertexObject>();
    auto positions = make_unique<PositionArray>();
    auto indices = make_unique<IndexArray>();
    
    system.drag = 0.4;
    float k = 95.;
    float r = .3;

    float y = 1.;
    for (int i = 0; i < N; ++i) {
      float x = 1.;
      for (int j = 0; j < N; ++j) {
        system.AddParticle(state, glm::vec3(x, y, 0.), glm::vec3(0.2, -0.1, 0.));
        system.fixed.push_back(false);
        positions->push_back(state.positions[IndexOf(i,j)]);

        // structural springs
        if (i < N - 1) {
          system.AddSpring(IndexOf(i, j), IndexOf(i + 1, j), r, k);
          indices->push_back(IndexOf(i, j));
          indices->push_back(IndexOf(i + 1, j));
        }
        if (j < N - 1) {
          system.AddSpring(IndexOf(i, j), IndexOf(i, j + 1), r, k);
          indices->push_back(IndexOf(i, j));
          indices->push_back(IndexOf(i, j + 1));
        }

        // shear springs
        if (i < N - 1 && j < N - 1) {
          system.AddSpring(IndexOf(i,j), IndexOf(i + 1, j + 1), std::sqrt(r*r), k);
          // indices->push_back(IndexOf(i, j));
          // indices->push_back(IndexOf(i + 1, j + 1));
        }
        if (i < N - 1 && j > 0) {
          system.AddSpring(IndexOf(i,j), IndexOf(i + 1, j - 1), std::sqrt(r*r), k);
          // indices->push_back(IndexOf(i, j));
          // indices->push_back(IndexOf(i + 1, j - 1));
        }

        // flex springs
        if (i < N - 2) {
          system.AddSpring(IndexOf(i, j), IndexOf(i + 2, j), 2*r, k);
          // indices->push_back(IndexOf(i, j));
          // indices->push_back(IndexOf(i + 2, j));
        }
        if (j < N - 2) {
          system.AddSpring(IndexOf(i, j), IndexOf(i, j + 2), 2*r, k);
          // indices->push_back(IndexOf(i, j));
          // indices->push_back(IndexOf(i, j + 2));
        }

        auto sphere_node = make_unique<SceneNode>();
        sphere_node->GetTransform().SetPosition(state.positions[IndexOf(i, j)]);
        sphere_node->CreateComponent<RenderingComponent>(sphere_mesh_);
        sphere_node->CreateComponent<ShadingComponent>(shader_);
        sphere_node_ptrs.push_back(sphere_node.get());
        AddChild(std::move(sphere_node));
        x += 0.3;
      }
      y -= 0.3;
    }
    // fix corner particles
    system.FixParticle(0);
    system.FixParticle(IndexOf(0, N - 1));

    cloth_mesh_->UpdatePositions(std::move(positions));
    cloth_mesh_->UpdateIndices(std::move(indices));

    auto line_node = make_unique<SceneNode>();
    line_node->CreateComponent<ShadingComponent>(line_shader_);
    auto& component = line_node->CreateComponent<RenderingComponent>(cloth_mesh_);
    component.SetDrawMode(DrawMode::Lines);
    auto material = std::make_shared<Material>(Material::GetDefault());
    line_node->CreateComponent<MaterialComponent>(material);
    line_ptr = line_node.get();
    AddChild(std::move(line_node));
}

int ClothNode::IndexOf(int i, int j) {
    return N * i + j;
}

void ClothNode::Update(double delta_time) {
  static bool prev_released = true;
  if (InputManager::GetInstance().IsKeyPressed('R')) {
    if (prev_released) {
      state = ParticleState();
      float y = 1.;
      // auto positions = make_unique<PositionArray>();
      for (int i = 0; i < N; ++i) {
        float x = 1.;
        for (int j = 0; j < N; ++j) {
          system.AddParticle(state, glm::vec3(x, y, 0.), glm::vec3(0.2, -0.1, 0.));
          // positions->push_back(state.positions[IndexOf(i,j)]);
          sphere_node_ptrs[IndexOf(i, j)]->GetTransform().SetPosition(state.positions[IndexOf(i, j)]);
          x += 0.3;
        }
        y -= 0.3;
      }
    }
    prev_released = false;
  } else {
    prev_released = true;
  }

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
  auto positions = make_unique<PositionArray>();
  for (int i = 0; i < N * N; ++i) {
    positions->push_back(state.positions[i]);
  }
  cloth_mesh_->UpdatePositions(std::move(positions));
}

}  // namespace GLOO
