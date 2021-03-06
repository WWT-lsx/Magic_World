/*
 * Copyright (C) 2012 Alec Thomas <alec@swapoff.org>
 * All rights reserved.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.
 *
 * Author: Alec Thomas <alec@swapoff.org>
 */

#ifndef _ENTITYX_SYSTEM_H_
#define _ENTITYX_SYSTEM_H_


#include <cstdint>
#include <unordered_map>
#include <utility>
#include <cassert>
#include "config.h"
#include "Entity.h"
#include "Event.h"
#include "help/NonCopyable.h"
#include "Define/Magic_Macro.h"


#pragma warning(push)
#pragma warning(disable:4251)

namespace EntityX {


	class SystemManager;


	/**
	 * Base System class. Generally should not be directly used, instead see System<Derived>.
	 */
	class DLL_MAGIC_ENGINE_OUTPUT_INPUT BaseSystem : ::EntityX::help::NonCopyable
	{
	public:
		typedef size_t Family;

		virtual ~BaseSystem();

		/**
		 * Called once all Systems have been added to the SystemManager.
		 *
		 * Typically used to set up event handlers.
		 */
		 //一旦所有系统被添加到SystemManager中，就会被调用。
		 //通常用于设置事件处理程序。
		virtual void configure(EntityManager &entities, EventManager &events)
		{
			configure(events);
		}

		/**
		 * Legacy configure(). Called by default implementation of configure(EntityManager&, EventManager&).
		 */
		virtual void configure(EventManager &events) {}

		/**
		 * Apply System behavior.
		 *
		 * Called every game step.
		 */
		virtual void Update(EntityManager &entities, EventManager &events, ::EntityX::Entity _NowEntity, TimeDelta dt) = 0;

		static Family family_counter_;

	protected:
	};


	/**
	 * Use this class when implementing Systems.
	 *
	 * struct MovementSystem : public System<MovementSystem> {
	 *   void update(EntityManager &entities, EventManager &events, TimeDelta dt) {
	 *     // Do stuff to/with entities...
	 *   }
	 * }
	 */
	template <typename Derived>
	class System : public BaseSystem
	{
	public:
		virtual ~System() {}

	private:
		friend class SystemManager;

		static Family family()
		{
			static Family family = family_counter_++;
			return family;
		}
	};


	class DLL_MAGIC_ENGINE_OUTPUT_INPUT SystemManager : ::EntityX::help::NonCopyable
	{
	public:
		SystemManager(EntityManager &entity_manager, EventManager &event_manager) :
			entity_manager_(entity_manager), event_manager_(event_manager) {}

		/**
		 * Add a System to the SystemManager.
		 *
		 * Must be called before Systems can be used.
		 *
		 * eg.
		 * std::shared_ptr<MovementSystem> movement = EntityX::make_shared<MovementSystem>();
		 * system.add(movement);
		 */
		template <typename S>
		void add(std::shared_ptr<S> system)
		{
			systems_.insert(std::make_pair(S::family(), system));
		}

		/**
		 * Add a System to the SystemManager.
		 *
		 * Must be called before Systems can be used.
		 *
		 * eg.
		 * auto movement = system.add<MovementSystem>();
		 */
		template <typename S, typename ... Args>
		std::shared_ptr<S> add(Args && ... args)
		{
			std::shared_ptr<S> s(new S(std::forward<Args>(args) ...));
			add(s);
			return s;
		}

		/*
		*功能：
		*	检查是否存在某个系统
		*模板参数：
		*	系统类的类型
		*参数：
		*	空
		*返回值：
		*	bool = true 存在 | false 不存在
		*/
		template <typename S>
		bool Has_System()
		{
			return systems_.find(S::family()) != systems_.end();
		}

		/**
		 * Retrieve the registered System instance, if any.
		 *
		 *   std::shared_ptr<CollisionSystem> collisions = systems.system<CollisionSystem>();
		 *
		 * @return System instance or empty shared_std::shared_ptr<S>.
		 */
		template <typename S>
		std::shared_ptr<S> system()
		{
			auto it = systems_.find(S::family());
			assert(it != systems_.end());
			return it == systems_.end() ? std::shared_ptr<S>() : std::shared_ptr<S>(std::static_pointer_cast<S>(it->second));
		}

		/**
		 * Call the System::update() method for a registered system.
		 */
		template <typename S>
		void Update(::EntityX::Entity _NowEntity, TimeDelta dt)
		{
			assert(initialized_ && "SystemManager::configure() not called");
			std::shared_ptr<S> s = system<S>();
			s->Update(entity_manager_, event_manager_, _NowEntity, dt);
		}

		/**
		 * Call System::update() on all registered systems.
		 *
		 * The order which the registered systems are updated is arbitrary but consistent,
		 * meaning the order which they will be updated cannot be specified, but that order
		 * will stay the same as long no systems are added or removed.
		 *
		 * If the order in which systems update is important, use SystemManager::update()
		 * to manually specify the update order. EntityX does not yet support a way of
		 * specifying priority for update_all().
		 */
		void update_all(::EntityX::Entity _NowEntity, TimeDelta dt);

		/**
		 * Configure the system. Call after adding all Systems.
		 *
		 * This is typically used to set up event handlers.
		 */
		 //配置系统。 添加所有系统后调用。
		 //这通常用于设置事件处理程序。
		void configure();

	private:
		bool initialized_ = false;
		EntityManager &entity_manager_;
		EventManager &event_manager_;
		std::unordered_map<BaseSystem::Family, std::shared_ptr<BaseSystem>> systems_;
	};

}  // namespace EntityX

#pragma warning(pop)

#endif