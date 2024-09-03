#pragma once

#include "jeffLight.h"

namespace jeffNamespace
{
	class jeffLightDirectional : public jeffLight
	{
	public:
		void initObject() override
		{
			transformRotation.z = 1.0f;
			jeffObject::initObject();
		}


	};
}
