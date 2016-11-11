#include "data/TaskInjectStorage.h"

#include <chrono>
#include <thread>

#include "data/Storage.h"
#include "data/StorageProvider.h"
#include "utility/scheduling/Blackboard.h"

TaskInjectStorage::TaskInjectStorage(
	std::shared_ptr<StorageProvider> storageProvider,
	std::shared_ptr<Storage> target
)
	: m_storageProvider(storageProvider)
	, m_target(target)
	, m_hasInjected(false)
{
}

void TaskInjectStorage::doEnter(std::shared_ptr<Blackboard> blackboard)
{
}

Task::TaskState TaskInjectStorage::doUpdate(std::shared_ptr<Blackboard> blackboard)
{
	if (m_storageProvider->getStorageCount() > 0)
	{
		std::shared_ptr<IntermediateStorage> source = m_storageProvider->popInjectionSource();
		if (source)
		{
			m_target->inject(source.get());
			return STATE_SUCCESS;
		}
	}
	else
	{
		const int SLEEP_TIME_MS = 25;
		std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME_MS));
	}

	return STATE_FAILURE;
}

void TaskInjectStorage::doExit(std::shared_ptr<Blackboard> blackboard)
{
}

void TaskInjectStorage::doReset(std::shared_ptr<Blackboard> blackboard)
{
}
