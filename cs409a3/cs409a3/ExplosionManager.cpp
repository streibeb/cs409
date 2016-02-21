//
//  ExplosionManager.cpp
//

#include <cassert>

#include "GetGlut.h"
#include "../../ObjLibrary/Vector3.h"
#include "../../ObjLibrary/DisplayList.h"
#include "../../ObjLibrary/TextureManager.h"

#include "TimeSystem.h"
#include "ExplosionManagerInterface.h"
#include "ExplosionManager.h"

using namespace std;



const double ExplosionManager :: LIFESPAN = 0.75;



ExplosionManager :: ExplosionManager ()
		: ExplosionManagerInterface(),
		  m_frame_count(0),
		  m_oldest_explosion(0),
		  m_next_explosion(0)
{
	assert(mv_frames.empty());
	// ma_explosions contains logical garbage

	assert(invariant());
}

ExplosionManager :: ExplosionManager (const ExplosionManager& original)
		: ExplosionManagerInterface(original)
{
	copy(original);

	assert(invariant());
}

ExplosionManager :: ~ExplosionManager ()
{
	// no dynamic memory to free
}

ExplosionManager& ExplosionManager :: operator= (const ExplosionManager& original)
{
	if(&original != this)
	{
		ExplosionManagerInterface::operator=(original);

		copy(original);
	}

	return *this;
	assert(invariant());
}



bool ExplosionManager :: isInitialized () const
{
	return !mv_frames.empty();
}

bool ExplosionManager :: isEmpty () const
{
	if(m_oldest_explosion == m_next_explosion)
		return true;
	else
		return false;
}

ExplosionManager* ExplosionManager :: getClone () const
{
	return new ExplosionManager(*this);
}

void ExplosionManager :: draw (const Vector3& camera_forward,
                               const Vector3& camera_up) const
{
	assert(isInitialized());
	assert(camera_forward.isNormal());
	assert(camera_up.isNormal());
	assert(camera_forward.isOrthogonal(camera_up));

	//
	//  Step 1: Create orientation matrix
	//
	//  OpenGL's opinions on matrix specification
	//
	//  C[0]  C[4]  C[8]  C[12]     M[0]  M[4]  M[8]  M[12]     v[0]
	//  C[1]  C[5]  C[9]  C[13]     M[1]  M[5]  M[9]  M[13]     v[1]
	//  C[2]  C[6]  C[10] C[14]  x  M[2]  M[6]  M[10] M[14]  x  v[2]
	//  C[3]  C[7]  C[11] C[15]     M[3]  M[7]  M[11] M[15]     v[3]
	//
	//  C is the existing transforation matrix
	//  M is the new matrix
	//  v is the vector
	//
	//  https://www.opengl.org/sdk/docs/man2/xhtml/glMultMatrix.xml
	//

	Vector3 camera_right = camera_forward.crossProduct(camera_up);

/*	// for sprites aligned to face Z+
	double a_matrix[16] =
	{ -camera_right.x,   -camera_right.y,   -camera_right.z,   0.0,
	   camera_up.x,       camera_up.y,       camera_up.z,      0.0,
	   camera_forward.x,  camera_forward.y,  camera_forward.z, 0.0,
	   0.0,               0.0,               0.0,              1.0 };
*/

	// for sprites aligned to face Y+
	double a_matrix[16] =
	{ camera_right.x,   camera_right.y,   camera_right.z,   0.0,
	  camera_forward.x, camera_forward.y, camera_forward.z, 0.0,
	  camera_up.x,      camera_up.y,      camera_up.z,      0.0,
	  0.0,              0.0,              0.0,              1.0 };

	//
	//  Step 2: Display explosions
	//

	unsigned int end = m_next_explosion;
	if(end < m_oldest_explosion)
		end += EXPLOSION_COUNT_MAX;

	glColor3d(1.0, 1.0, 1.0);
	for(unsigned int i = m_oldest_explosion; i < end; i++)
	{
		unsigned int index = i % EXPLOSION_COUNT_MAX;
		assert(index < EXPLOSION_COUNT_MAX);

		double life_elapsed_seconds = TimeSystem::getFrameStartTime() - ma_explosions[index].m_creation_time;
		assert(life_elapsed_seconds >= 0.0);

		assert(LIFESPAN > 0.0);
		double life_elapsed_fraction = life_elapsed_seconds / LIFESPAN;
		assert(life_elapsed_fraction >= 0.0);

		if(life_elapsed_fraction < 1.0)
		{
			const Vector3& position = ma_explosions[index].m_position;
			double size = ma_explosions[index].m_size;
			unsigned int frame = (unsigned int)(life_elapsed_fraction * m_frame_count);

			glPushMatrix();
				glTranslated(position.x, position.y, position.z);
				glMultMatrixd(a_matrix);  // apply orientation matrix
				glScaled(size, size, size);

				assert(frame < m_frame_count);
				assert(mv_frames[frame].isReady());
				mv_frames[frame].draw();
			glPopMatrix();
		}
	}
}



void ExplosionManager :: init (const string& filename,
                               unsigned int frame_count)
{
	assert(filename != "");
	assert(frame_count > 0);

	if(!isInitialized())
	{
		m_frame_count = frame_count;
		mv_frames.resize(m_frame_count);

		// ensure the texture is in video memory
		TextureManager::activate(filename);

		for(unsigned int i = 0; i < frame_count; i++)
		{
			float x1 =  i      / (double)(m_frame_count);
			float x2 = (i + 1) / (double)(m_frame_count);

			assert(mv_frames[i].isEmpty());
			mv_frames[i].begin();
				glEnable(GL_TEXTURE_2D);
				TextureManager::activate(filename);
				glBegin(GL_QUADS);
					glTexCoord2f(x1, 0.0f);  glVertex3f(-1.0, 0.0f,  1.0);
					glTexCoord2f(x1, 1.0f);  glVertex3f(-1.0, 0.0f, -1.0);
					glTexCoord2f(x2, 1.0f);  glVertex3f( 1.0, 0.0f, -1.0);
					glTexCoord2f(x2, 0.0f);  glVertex3f( 1.0, 0.0f,  1.0);
				glEnd();
				glDisable(GL_TEXTURE_2D);
			mv_frames[i].end();
			assert(mv_frames[i].isReady());
		}
	}

	assert(isInitialized());
	assert(invariant());
}

void ExplosionManager :: add (const Vector3& position, double size)
{
	ma_explosions[m_next_explosion].m_position      = position;
	ma_explosions[m_next_explosion].m_size          = size;
	ma_explosions[m_next_explosion].m_creation_time = TimeSystem::getFrameStartTime();
	m_next_explosion = (m_next_explosion + 1) % EXPLOSION_COUNT_MAX;

	assert(invariant());
}

void ExplosionManager :: update ()
{
	unsigned int end = m_next_explosion;
	if(end < m_oldest_explosion)
		end += EXPLOSION_COUNT_MAX;

	for(unsigned int i = m_oldest_explosion; i < end; i++)
	{
		assert(i >= m_oldest_explosion);

		unsigned int index = i % EXPLOSION_COUNT_MAX;
		assert(index < EXPLOSION_COUNT_MAX);

		if(i == m_oldest_explosion &&
		   ma_explosions[index].m_creation_time + LIFESPAN <= TimeSystem::getFrameStartTime())
		{
			// move along oldest counter if explosion is done
			m_oldest_explosion++;
		}
		else
		{
			//
			//  All explosion take the same time, so if this one
			//    isn't done, none are.
			//
			break;
		}
	}

	assert(m_oldest_explosion <= end);
	m_oldest_explosion %= EXPLOSION_COUNT_MAX;

	assert(invariant());
}

void ExplosionManager :: removeAll ()
{
	m_oldest_explosion = 0;
	m_next_explosion   = 0;

	assert(invariant());
}



void ExplosionManager :: copy (const ExplosionManager& original)
{
	m_frame_count = original.m_frame_count;

	mv_frames.resize(m_frame_count);
	for(unsigned int i = 0; i < m_frame_count; i++)
		mv_frames[i] = original.mv_frames[i];

	for(unsigned int i = 0; i < EXPLOSION_COUNT_MAX; i++)
		ma_explosions[i] = original.ma_explosions[i];

	m_oldest_explosion = original.m_oldest_explosion;
	m_next_explosion   = original.m_next_explosion;

	assert(invariant());
}

bool ExplosionManager :: invariant () const
{
	if(mv_frames.size() != m_frame_count) return false;
	if(!mv_frames.empty() && mv_frames[0].isPartial()) return false;
	for(unsigned int i = 1; i < m_frame_count ; i++)
		if(!mv_frames.empty() && mv_frames[0].getState() != mv_frames[i].getState())
			return false;

	if(m_oldest_explosion >= EXPLOSION_COUNT_MAX) return false;
	if(m_next_explosion   >= EXPLOSION_COUNT_MAX) return false;

	return true;
}

