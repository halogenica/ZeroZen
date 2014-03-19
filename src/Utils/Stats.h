
#pragma once

#include "Common.h"

namespace gen
{

class Stats
{
public:
	Stats();
	~Stats();

	void OnLostDevice();
	void OnResetDevice();

	void addVertices(unsigned int n);
	void subVertices(unsigned int n);
	void addTriangles(unsigned int n);
	void subTriangles(unsigned int n);
    void addRigidBodies(unsigned int n);
	void subRigidBodies(unsigned int n);

    void setCursorPosition(float x, float y);
	void setTriCount(unsigned int n);
	void setVertexCount(unsigned int n);
    void setRigidBodyCount(unsigned int n);

	void updateFPS(float dt);
    void updateUPS(float dt);
    void display();


	float m_FPS;
    float m_UPS;

private:
	// Prevent copying
	Stats(const Stats& rhs);
	Stats& operator=(const Stats& rhs);
	
private:

	ci::gl::TextureFontRef  m_TextFont;
    ci::Rectf               m_TextBoundsRect;
	float m_MilliSecPerFrame;
    float m_MilliSecPerUpdate;
	unsigned int m_NumTris;
	unsigned int m_NumVertices;
    unsigned int m_NumRigidBodies;
    float m_CursorX;
    float m_CursorY;
};
    
}
