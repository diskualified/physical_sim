#ifndef EULER_SCENE_NODE_H_
#define EULER_SCENE_NODE_H_

#include <vector>
#include <memory>
#include <unordered_map>
#include <iostream>
#include <typeinfo>
#include <stdexcept>

#include <glm/vec3.hpp>

#include "gloo/components/ComponentBase.hpp"
#include "gloo/components/ComponentType.hpp"
#include "gloo/Transform.hpp"

#include "gloo/SceneNode.hpp"
#include "ParticleState.hpp"
#include "ForwardEulerIntegrator.hpp"
#include "SimpleODE.hpp"
#include "IntegratorType.hpp"
#include "gloo/VertexObject.hpp"
#include "gloo/shaders/ShaderProgram.hpp"
#include "gloo/shaders/PhongShader.hpp"


namespace GLOO {
class EulerSceneNode: public SceneNode {
 public:
  EulerSceneNode(IntegratorType type, float step);
  void Update(double delta_time) override;

 private:
  ParticleState state;
  std::unique_ptr<IntegratorBase<SimpleODE, ParticleState>> integrator;
  SimpleODE system;
  float step;
  std::shared_ptr<VertexObject> sphere_mesh_;
  std::shared_ptr<ShaderProgram> shader_ = std::make_shared<PhongShader>();
  SceneNode* sphere_node_ptr;

  ComponentBase* GetComponentPtrByType(ComponentType type) const;
  std::vector<ComponentBase*> GetComponentsPtrInChildrenByType(
      ComponentType type) const;
  void GatherComponentPtrsRecursivelyByType(
      ComponentType type,
      std::vector<ComponentBase*>& result) const;

  Transform transform_;
  std::unordered_map<ComponentType,
                     std::unique_ptr<ComponentBase>,
                     EnumClassHash>
      component_dict_;
  std::vector<std::unique_ptr<SceneNode>> children_;
  SceneNode* parent_;
  bool active_;
};
}  // namespace GLOO

#endif
