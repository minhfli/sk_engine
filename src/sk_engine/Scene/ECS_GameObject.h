#pragma once

#include <entt/entt.hpp>
#include <sk_engine/Common/Error.h>
#include "Scene.h"

#include "Scene.h"
namespace sk_engine {
    class GameObject {
        friend class Scene;
        private:
        entt::entity m_entity;
        Scene* m_scene;

        GameObject(entt::entity entity, Scene* scene) : m_entity(entity), m_scene(scene) {}

        public:
        GameObject() : m_entity(entt::null), m_scene(nullptr) {}
        GameObject(const GameObject& g) = default;
        ~GameObject() {}

        /// @brief  check if GameObject has component
        /// @tparam ...T list of component to check
        /// @param check_all true  : return true if gameobject has all of the components
        /// @param check_all false : return true if gameobject has one of the components
        template <class... T>
        bool HasComponent(bool check_all = 1) {
            if (check_all)
                return m_scene->EnTT_reg.all_of<T...>(m_entity);
            else return m_scene->EnTT_reg.any_of<T...>(m_entity);
        }
        /// @brief add component to entity, be careful entity should only have 1 component of any type
        /// @tparam ...Args 
        /// @tparam T component
        /// @param ...args list of argument to create component
        /// @return component ref
        template<typename T, typename... Args>
        T* AddComponent(Args&&... args) {
            if (HasComponent<T>()) {
                Error("Entity already has component! ");
                return nullptr;
            }
            T& component = m_scene->EnTT_reg.emplace<T>(m_entity, std::forward<Args>(args)...);
            //m_scene->OnComponentAdded<T>(*this, component);
            return &component;
        }
        /// @brief add component to entity, replace if entity already has component
        template<typename T, typename... Args>
        T* AddorReplaceComponent(Args&&... args) {
            T& component = m_scene->EnTT_reg.emplace_or_replace<T>(m_entity, std::forward<Args>(args)...);
            //m_scene->OnComponentAdded<T>(*this, component);
            return &component;
        }

        template<typename T>
        T* GetComponent() {
            if (!HasComponent<T>()) {
                Error("Entity does not have component");
                return nullptr;
            }
            return &m_scene->EnTT_reg.get<T>(m_entity);
        }

        template<typename T>
        void RemoveComponent() {
            if (!HasComponent<T>()) {
                Error("Entity does not have component");
                return;
            }
            m_scene->EnTT_reg.remove<T>(m_entity);
        }

        /// @brief not implemented, dont use this
        void Destroy() {
            m_scene->EnTT_reg.destroy(m_entity);
        }
    };
}