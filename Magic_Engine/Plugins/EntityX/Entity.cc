/*
 * Copyright (C) 2012 Alec Thomas <alec@swapoff.org>
 * All rights reserved.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.
 *
 * Author: Alec Thomas <alec@swapoff.org>
 */

#include <algorithm>
#include "Entity.h"

namespace EntityX
{

	const Entity::Id Entity::INVALID;
	BaseComponent::Family BaseComponent::family_counter_ = 0;

	void Entity::invalidate()
	{
		m_id = INVALID;
		pManager = nullptr;
	}

	void Entity::destroy()
	{
		assert(valid());
		pManager->destroy(m_id);
		invalidate();
	}

	std::bitset<EntityX::MAX_COMPONENTS> Entity::GetComponentMask() const
	{
		return pManager->GetComponentMask(m_id);
	}

	EntityManager::EntityManager(EventManager &event_manager) : event_manager_(event_manager)
	{
		m_ComponentSize = 0;
	}

	EntityManager::~EntityManager()
	{
		reset();
	}

	void EntityManager::reset()
	{
		for (Entity entity : entities_for_debugging())
			entity.destroy();
		for (BasePool *pool : component_pools_)
		{
			if (pool) delete pool;
		}
		for (BaseComponentHelper *helper : component_helpers_)
		{
			if (helper) delete helper;
		}
		component_pools_.clear();
		component_helpers_.clear();
		entity_component_mask_.clear();
		entity_version_.clear();
		free_list_.clear();
		m_ComponentReal.clear();
		index_counter_ = 0;
		m_ComponentSize = 0;
	}

	EntityCreatedEvent::~EntityCreatedEvent() {}
	EntityDestroyedEvent::~EntityDestroyedEvent() {}


}  // namespace EntityX
