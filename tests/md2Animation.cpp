// Copyright (C) 2008-2009 Colin MacDonald
// No rights reserved: this software is in the public domain.

#include "testUtils.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

// Tests MD2 animations.
/** At the moment, this just verifies that the last frame of the animation produces the expected bitmap. */
bool md2Animation(void)
{
	// Use EDT_BURNINGSVIDEO since it is not dependent on (e.g.) OpenGL driver versions.
	IrrlichtDevice *device = createDevice( EDT_BURNINGSVIDEO, dimension2d<u32>(160, 120), 32);
	assert(device);
	if (!device)
		return false;

	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager * smgr = device->getSceneManager();

	IAnimatedMesh* mesh = smgr->getMesh("../media/sydney.md2");
	IAnimatedMeshSceneNode* node;
	assert(mesh);

	bool result = (mesh != 0);
	if(mesh)
	{
		node = smgr->addAnimatedMeshSceneNode(mesh);
		assert(node);

		if(node)
		{
			node->setPosition(vector3df(20, 0, 30));
			node->setMaterialFlag(EMF_LIGHTING, false);
			node->setMaterialTexture(0, driver->getTexture("../media/sydney.bmp"));
			node->setLoopMode(false);

			(void)smgr->addCameraSceneNode();

			// Just jump to the last frame since that's all we're interested in.
			node->setMD2Animation(EMAT_DEATH_FALLBACK);
			node->setCurrentFrame((f32)(node->getEndFrame()));
			node->setAnimationSpeed(0);
			device->run();
			driver->beginScene(true, true, SColor(255, 255, 255, 0));
			smgr->drawAll();
			driver->endScene();
			if (mesh->getBoundingBox() != mesh->getMesh(node->getEndFrame())->getBoundingBox())
			{
				logTestString("bbox of md2 mesh not updated.\n");
				result = false;
			}
			//TODO: Does not yet work, not sure if this is correct or not
#if 0
			if (node->getBoundingBox() != mesh->getMesh(node->getFrameNr())->getBoundingBox())
			{
				logTestString("bbox of md2 scene node not updated.\n");
				result = false;
			}
#endif
			if (node->getTransformedBoundingBox() == core::aabbox3df())
			{
				logTestString("md2 node returns empty bbox.\n");
				result = false;
			}
		}
	}

	result &= takeScreenshotAndCompareAgainstReference(driver, "-md2Animation.png");
	device->closeDevice();
	device->run();
	device->drop();

	return result;
}

