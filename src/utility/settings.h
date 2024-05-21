#ifndef SETTINGS_H
#define SETTINGS_H

#include <glm/glm.hpp>

namespace Winedark
{
	struct Settings
	{
		/*------------------------------------------------------------------------------------------------------*/
		/*  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  */
		/* Rendering                                                                                            */
		/*  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  */
		/*------------------------------------------------------------------------------------------------------*/
		/*-------------------------------------------------*/
		/* Camera                                          */
		/*-------------------------------------------------*/
		static constexpr float			FOV = 1.0f;
		static constexpr float			NearClip = 0.001f;
		static constexpr float			FarClip = 10000.0f;

		static constexpr float			CameraMovementSpeed = 80.0f;
		static constexpr float			CameraZoomSpeed = 0.2f;

		/*-------------------------------------------------*/
		/* Background                                      */
		/*-------------------------------------------------*/
		static constexpr glm::vec4		BackgroundColor = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);

		/*-------------------------------------------------*/
		/* Batches                                         */
		/*-------------------------------------------------*/
		static constexpr unsigned int	TrisPerBatch = 10000;
	};
}

#endif