#include "Scene.hpp"
#include "Vector3d.hpp"

#include "Mesh.hpp"

e3e::Scene::Scene(int w, int h) :
	camera(float(w)/float(h), 70.f),
	w(w), h(h)
{
	projectionShader.loadVert("shaders/projection.vert");
	projectionShader.link();

	reloadProjectionMatrix();

	// add test cube
	e3e::Node *parent = new e3e::Node(this, new e3e::Mesh());
	parent->translate(-1, 0, -15);

	e3e::Node *child = new e3e::Node(this, new e3e::Mesh());
	child->translate(2.2, 0, 0);
//	parent->addChildNode(child);

	sceneNodes.push_back( parent );
}

void e3e::Scene::reloadProjectionMatrix()
{

	GLint projectionMatrixUniform = glGetUniformLocation(projectionShader.getProgram(),
																		  "projectionMatrix");

	e3e::Matrix4f pm = camera.getProjectionMatrix();

	float *pmcm = pm.createColumnMajorArray();

	glUseProgram(projectionShader.getProgram());
	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, pmcm);
	glUseProgram(0);

	delete pmcm;
}

void e3e::Scene::applyMatrix()
{
	GLint worldTransformMatrixUniform = glGetUniformLocation(projectionShader.getProgram(),
																 "worldTransformMatrix");
	float *tcm = sceneMatrixStack.top()->createColumnMajorArray();

	glUseProgram(projectionShader.getProgram());
	glUniformMatrix4fv(worldTransformMatrixUniform, 1, GL_FALSE, tcm);

	delete tcm;
}

void e3e::Scene::drawAxis(float scale)
{
	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
	glColor3ub(0,0,255);
	glVertex3f(0,0,0);
	glVertex3f(0,0,scale);
	glColor3ub(0,255,0);
	glVertex3f(0,0,0);
	glVertex3f(0,scale,0);
	glColor3ub(255,0,0);
	glVertex3f(0,0,0);
	glVertex3f(scale,0,0);
	glColor3ub(255,255,255);
	glEnd();
	glEnable(GL_LIGHTING);
}

void e3e::Scene::render()
{
	glUseProgram(projectionShader.getProgram());

	drawAxis(1.f);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	std::vector<e3e::Node*>::iterator it;
	for(it = sceneNodes.begin(); it != sceneNodes.end(); it++)
	{
		(*it)->render();
//		(*it)->rotateXYZ(.005f, .020f, .015f);

//		(*it)->rotateXYZ(0, 0, .005f);
//		(*it).translate(.005f, 0, 0);
//		(*it).translate(0,0,.005f);
	}
}

