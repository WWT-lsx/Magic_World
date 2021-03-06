#include "Supervisor.h"

namespace Magic
{
	namespace System
	{
		ThreadsComponent::ThreadsComponent() :m_RunState(false), m_DiffTime(0.0), m_LastTime(0.0), m_Threads(0)
		{
			InitializeCriticalSection(&m_MutexObjectMessage);
			InitializeCriticalSection(&m_MutexSceneCommon);
		}

		ThreadsComponent::~ThreadsComponent()
		{
			DeleteCriticalSection(&m_MutexSceneCommon);
			DeleteCriticalSection(&m_MutexObjectMessage);
		}

		void MessageHandleSystem::Update(EntityX::EntityManager &_es, EntityX::EventManager &_events, ::EntityX::Entity _NowEntity, EntityX::TimeDelta _time)
		{
			EntityX::ComponentHandle<MessageHandleComponent> _MessageHandleComponent;
			for (EntityX::Entity _entity : _es.entities_with_components<MessageHandleComponent>(_MessageHandleComponent))
			{
				if (_MessageHandleComponent->m_Call_MessageHandle)
				{
					m_Return = _MessageHandleComponent->m_Call_MessageHandle(_entity, m_MessageStruct);
					if (m_Return == MAGIC_RETURN_EXIT)
						return;
				}
				if (_entity.has_component<ObjectSupervisor>())
				{
					ObjectSupervisor* _pObjectSupervisor = _entity.GetComponent<ObjectSupervisor>().operator->();
					_pObjectSupervisor->m_Supervisor.m_systems.system<MessageHandleSystem>()->m_MessageStruct = m_MessageStruct;
					_pObjectSupervisor->m_Supervisor.m_systems.Update<MessageHandleSystem>(_entity, _time);
					m_Return = _pObjectSupervisor->m_Supervisor.m_systems.system<MessageHandleSystem>()->m_Return;
					if (m_Return == MAGIC_RETURN_EXIT)
						return;
				}
			}
		}

		void ThreadsMessageForwardSystem::Update(EntityX::EntityManager &_es, EntityX::EventManager &_events, ::EntityX::Entity _NowEntity, EntityX::TimeDelta _time)
		{
			ThreadsComponent* _pThreadsComponent = _NowEntity.GetComponent<ThreadsComponent>().operator->();

			for (auto _Message : _pThreadsComponent->m_vec_ObjectMessageStruct)
			{
				if (_Message.Object.valid() && _Message.Object.has_component<MessageHandleComponent>())
				{
					MessageHandleComponent* _pMessageHandleComponent = _Message.Object.GetComponent<MessageHandleComponent>().operator->();
					if (_pMessageHandleComponent->m_Call_MessageHandle)
					{
						int _result = _pMessageHandleComponent->m_Call_MessageHandle(_Message.Object, _Message.Message);
						if (_result == MAGIC_RETURN_EXIT)
							goto GOTO_TMFS_RRTURN;
					}
					if (_Message.Object.has_component<ObjectSupervisor>())
					{
						ObjectSupervisor* _pObjectSupervisor = _Message.Object.GetComponent<ObjectSupervisor>().operator->();
						_pObjectSupervisor->m_Supervisor.m_systems.system<MessageHandleSystem>()->m_MessageStruct = _Message.Message;
						_pObjectSupervisor->m_Supervisor.m_systems.Update<MessageHandleSystem>(_Message.Object, _time);
						if (_pObjectSupervisor->m_Supervisor.m_systems.system<MessageHandleSystem>()->m_Return == MAGIC_RETURN_EXIT)
							goto GOTO_TMFS_RRTURN;
					}

					continue;
				}

				EntityX::ComponentHandle<MessageHandleComponent> _MessageHandleComponent;
				for (EntityX::Entity _entity : _es.entities_with_components<MessageHandleComponent>(_MessageHandleComponent))
				{
					if (_MessageHandleComponent->m_Call_MessageHandle)
					{
						int _result = _MessageHandleComponent->m_Call_MessageHandle(_entity, _Message.Message);
						if (_result == MAGIC_RETURN_EXIT)
							goto GOTO_TMFS_RRTURN;
					}
					if (_entity.has_component<ObjectSupervisor>())
					{
						ObjectSupervisor* _pObjectSupervisor = _entity.GetComponent<ObjectSupervisor>().operator->();
						_pObjectSupervisor->m_Supervisor.m_systems.system<MessageHandleSystem>()->m_MessageStruct = _Message.Message;
						_pObjectSupervisor->m_Supervisor.m_systems.Update<MessageHandleSystem>(_entity, _time);
						if (_pObjectSupervisor->m_Supervisor.m_systems.system<MessageHandleSystem>()->m_Return == MAGIC_RETURN_EXIT)
							goto GOTO_TMFS_RRTURN;
					}
				}
			}

		GOTO_TMFS_RRTURN:
			if (_pThreadsComponent->m_vec_ObjectMessageStruct.size())
				_pThreadsComponent->m_vec_ObjectMessageStruct.clear();
		}

		void ObjectUpdataSystem::Update(EntityX::EntityManager &_es, EntityX::EventManager &_events, ::EntityX::Entity _NowEntity, EntityX::TimeDelta _time)
		{
			EntityX::ComponentHandle<UpdataComponent> _UpdataComponent;
			for (EntityX::Entity _entity : _es.entities_with_components<UpdataComponent>(_UpdataComponent))
			{
				if (_UpdataComponent->m_Call_Updata)
					_UpdataComponent->m_Call_Updata(_entity);
				if (_entity.has_component<ObjectSupervisor>())
					_entity.GetComponent<ObjectSupervisor>()->m_Supervisor.m_systems.Update<ObjectUpdataSystem>(_entity, _time);
			}
		}

		void ThreadsRenderSystem::Update(EntityX::EntityManager &_es, EntityX::EventManager &_events, ::EntityX::Entity _NowEntity, EntityX::TimeDelta _time)
		{
			if (_NowEntity.has_component<Magic::System::RenderComponent>())
			{
				EntityX::ComponentHandle<Magic::System::RenderComponent> _RenderComponent = _NowEntity.GetComponent<Magic::System::RenderComponent>();
				if (_RenderComponent->m_Call_RenderStart)
					_RenderComponent->m_Call_RenderStart(_NowEntity);
				if (_RenderComponent->m_Call_Render)
					_RenderComponent->m_Call_Render(_NowEntity);
			}

			EntityX::ComponentHandle<RenderComponent> _RenderComponent;
			for (EntityX::Entity _entity : _es.entities_with_components<RenderComponent>(_RenderComponent))
			{
				if (_RenderComponent->m_Call_RenderStart)
					_RenderComponent->m_Call_RenderStart(_entity);
				if (_RenderComponent->m_Call_Render)
					_RenderComponent->m_Call_Render(_entity);
				if (_entity.has_component<ObjectSupervisor>())
					_entity.GetComponent<ObjectSupervisor>()->m_Supervisor.m_systems.Update<ObjectRenderSystem>(_entity, _time);
				if (_RenderComponent->m_Call_RenderEnd)
					_RenderComponent->m_Call_RenderEnd(_entity);
			}

			if (_NowEntity.has_component<Magic::System::RenderComponent>())
			{
				EntityX::ComponentHandle<Magic::System::RenderComponent> _RenderComponent = _NowEntity.GetComponent<Magic::System::RenderComponent>();
				if (_RenderComponent->m_Call_RenderEnd)
					_RenderComponent->m_Call_RenderEnd(_NowEntity);
			}
		}

		void ObjectRenderSystem::Update(EntityX::EntityManager &_es, EntityX::EventManager &_events, ::EntityX::Entity _NowEntity, EntityX::TimeDelta _time)
		{
			EntityX::ComponentHandle<RenderComponent> _RenderComponent;
			for (EntityX::Entity _entity : _es.entities_with_components<RenderComponent>(_RenderComponent))
			{
				if (_RenderComponent->m_Call_RenderStart)
					_RenderComponent->m_Call_RenderStart(_entity);
				if (_RenderComponent->m_Call_Render)
					_RenderComponent->m_Call_Render(_entity);
				if (_entity.has_component<ObjectSupervisor>())
					_entity.GetComponent<ObjectSupervisor>()->m_Supervisor.m_systems.Update<ObjectRenderSystem>(_entity, _time);
				if (_RenderComponent->m_Call_RenderEnd)
					_RenderComponent->m_Call_RenderEnd(_entity);
			}
		}
	}
}