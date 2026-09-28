 #ifndef EC_TASK_USER_H
 #define EC_TASK_USER_H

 #include <vector>
 #include "EcTaskUserBase.h"
 #include "EcTaskUserFactory.h"

 class EcTaskUser
 {
 public:
	EcTaskUser();

	~EcTaskUser();

	uint32_t addAll(std::vector<EcTaskUserFactoryNs::TaskData>& taskData);

	uint32_t cleanup();

	uint32_t registerXml();

	uint32_t registerPublisher();

	uint32_t registerSubscriber();

	uint32_t publishData();

	uint32_t subscribeData();

	uint32_t mainProcess();

	EcTaskUserBase* getElement(uint32_t index);

 private:
	std::vector<EcTaskUserBase*> m_ecTaskUserBaseVector;
	
	uint32_t addTaskUser(EcTaskUserBase* pTaskUser);
 };

 #endif // EC_TASK_USER_H
