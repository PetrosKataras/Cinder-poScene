//
//  poScene.cpp
//  BasicTest
//
//  Created by Stephen Varga on 3/17/14.
//
//
#include "cinder/app/App.h"

#include "poScene.h"
#include "poNodeContainer.h"


namespace po {
    SceneRef Scene::create()
    {
        return create(po::NodeContainer::create());
    }
    
    
    SceneRef Scene::create(NodeContainerRef rootNode)
    {
        SceneRef scene(new Scene(rootNode));
        scene->getRootNode()->setScene(scene);
        return scene;
    }
    
    Scene::Scene(NodeContainerRef rootNode)
    : rootNode(rootNode)
    , eventCenter(EventCenter::create())
    {
    }
    
    Scene::~Scene()
    {
    }
    
    #pragma mark -
    
    void Scene::update()
    {
        //Send a copy of all over our children to be processed
        processTrackingQueue();
        eventCenter->processEvents(allChildren);
        getRootNode()->updateTree();
    }
    
    void Scene::draw()
    {
        drawOrderCounter = 0;
        getRootNode()->drawTree();
    }
    
    #pragma mark -
    
    uint32_t Scene::getNextDrawOrder()
    {
        return drawOrderCounter++;
    }
    
    void Scene::setRootNode(NodeContainerRef node)
    {
        rootNode->removeScene();
        
        rootNode = node;
        rootNode->setScene(shared_from_this());
    }
    
    
    
    #pragma mark -
    void Scene::trackChildNode(NodeRef node) {
        mTrackingQueue.push_back(TrackedNode(node, true));
    }
    
    void Scene::untrackChildNode(NodeRef node) {
        mTrackingQueue.push_back(TrackedNode(node, false));
    }
    
    void Scene::processTrackingQueue()
    {
        for (const TrackedNode &node : mTrackingQueue) {
            std::vector<NodeRef>::iterator iter = std::find(allChildren.begin(), allChildren.end(), node.node);
            
            if(node.bTrack && iter == allChildren.end()) {
                allChildren.push_back(node.node);
            }
            
            else if(!node.bTrack && iter != allChildren.end()) {
                allChildren.erase(iter);
            }
        }
        
        mTrackingQueue.clear();
    }
}
