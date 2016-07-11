#pragma once
#include <GL\glew.h>
#include <glm\glm.hpp>
#include "Ray.hpp"

using namespace glm;

namespace glt {
	class Camera {
	public:
		static Camera*		active;

		vec3				position = vec3( 0.f, 0.f, 1.f );
		vec3				direction = vec3( 0.f, 0.f, -1.f );

		float				fieldOfView = 90.f;
		float				near = 0.1f;		// Clipping planes
		float				far = 100.f;		//

		bool				perspective = true;
		float				orthoFrustum = 1.f;	// Height of frustum if using orthographic projection

		vec2				screenSize = vec2( 800.f, 600.f ); // Used for calculating aspect ratio

		mat4				getMatrix( );

		//-----------

		Camera( vec2 screenSize );

		// Deprojecting from screen
		Ray				screenToWorld( int x, int y ) { return screenToWorld( vec2( x, y ) ); }
		Ray				screenToWorld( vec2 screen );

		// Projecting to screen
		vec2			worldToScreen( vec3 world );
		
		float			getAspectRatio( ) { return screenSize.x / screenSize.y; }

		// Use this camera
		void			use( );

	private:
		mat4			cameraMatrix;

		//Dirtyness for matrix calculation
		bool			isDirty;

		vec3			position_old;
		vec3			direction_old;

		float			fov_old;
		float			near_old;
		float			far_old;

		bool			perspective_old;
		float			orthoFrustum_old;

		vec2			screenSize_old;

		//---

		void			checkDirty( );
		void			clean( );
	};
}