#pragma once
#include <glm\glm.hpp>

namespace glt {
	struct Transform {
	public:
		glm::vec3		position;
		glm::vec3		rotation;
		glm::vec3		scale;

		//-----------

		Transform( );
		Transform( glm::vec3 position, glm::vec3 rotation, glm::vec3 scale );

		glm::mat4		getMatrix( );
		operator glm::mat4( ) { return getMatrix( ); }

	private:
		glm::vec3		position_old;		// Used for dirtiness checking
		glm::vec3		rotation_old;		//
		glm::vec3		scale_old;			//

		glm::mat4		matrixBuffer;		// Used and updated when getting the matrix

		//------------

		bool			isDirty( );			// Compares all the values to see if something is updated
		void			clean( );			// Resets all the old values

	};
}