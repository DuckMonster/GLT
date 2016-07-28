#pragma once
#include <glm\glm.hpp>

namespace glt {
	struct Transform {
	public:
		glm::vec3				position	= glm::vec3( 0.f );
		glm::vec3				rotation	= glm::vec3( 0.f );
		glm::vec3				scale		= glm::vec3( 1.f );

		//-----------

		Transform( );
		Transform( glm::vec3 position, glm::vec3 rotation, glm::vec3 scale );

		glm::mat4				getMatrix( );
		glm::mat4				getNormalMatrix( );
		void					setMatrix( glm::mat4 matrix );

		bool operator			==( Transform& other );
		operator glm::mat4( ) { return getMatrix( ); }

	private:
		glm::vec3				position_old;		// Used for dirtiness checking
		glm::vec3				rotation_old;		//
		glm::vec3				scale_old;			//

		glm::mat4				matrixBuffer;		// Used and updated when getting the matrix
		glm::mat4				normalMatrix;		//

		//------------

		bool					isDirty( );			// Compares all the values to see if something is updated
		void					clean( );			// Resets all the old values

	};
}