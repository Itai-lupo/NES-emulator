#include "renderer.h"
#include "mesh.h"
#include "meshRenderer.h"
#include "shaderTypes.h"
#include "LTEError.h"
#include "app.h"

#include <algorithm>

namespace LTE
{
    renderer::renderer(windowPieceId winId, renderApi *renderPipLine): 
        win(windowManger::getWindow(winId)), Scene(win->activeScene), renderPipLine(renderPipLine)
    {

    }

    shaderRenderBuffer *renderer::submitShape(mesh *shape, material *shapeMatrial)
    {
        shaderRenderBuffer *s  = windowManger::getWindow(shape->getWindowId())->assetLibrary->getAsset<shaderRenderBuffer>(shape->getShaderName());
        if(!s)
            return nullptr;
        s->pushShape(shape, shapeMatrial);
        return s;
    }

    void renderer::sortSceneToRender()
    {
        std::sort(
            Scene->objects->begin(),
            Scene->objects->end(),
            [=](gameObject *a, gameObject *b) -> bool
            {
                try
                {
                    return a->getTransform()->getPostion().z < b->getTransform()->getPostion().z;
                }
                catch(LTEException* e)
                {
                    LAUGHTALE_ENGINR_LOG_ERROR(e->what());
                    return false;
                }
                
            });
    }

    void renderer::batchSceneData()
    {

        for(gameObject *toRender: *Scene->objects)
        {
            try
            {
                mesh *obj = toRender->getComponent<mesh>();
                material *objMat = toRender->getComponent<material>();
                if(!obj || !objMat)
                    continue;
                shaderRenderBuffer *s  = submitShape(obj, objMat);
                if(!s)
                    continue;
                shadersToRender.insert(s);
                if(!textures[objMat->getTextureId()])
                {
                    textures[objMat->getTextureId()] = objMat->getTexture();
                }
            }
            catch(LTEException* e)
            {
                LAUGHTALE_ENGINR_LOG_ERROR("can't render " << e->what());
            }
        }
    }

    void renderer::batchRenderScene()
    {
        for(auto& shaderBuffer: shadersToRender)
        {
            shader *s = shaderBuffer->getShader();
            s->bind();
            int *samplers = (int*)calloc(4, 8);
            for(int *i = samplers; i < samplers + 8; i++)
                *i = ((long)i - (long)samplers)/4;
            
            s->setUniformMat4f("viewProjection", ViewProjectionMatrix);
            s->setUniform1iv("textures", samplers, 8);

            while (!shaderBuffer->isAllRendered())
            {
                try
                {
                    shaderBuffer->setTextureIndex(texturesToUse);
                    for(const auto& [id, slot]: texturesToUse)
                        if(textures[id])
                            textures[id]->bind(slot);
                    // indicesToRender = new openGLIndexBuffer(shaderBuffer->getIndecesData(), shaderBuffer->getIndecesCount());
                    // indicesToRender->init();

                    // indicesToRender->bind();
                    shaderBuffer->bind();

                    renderPipLine->DrawIndexed(shaderBuffer->getIndecesCount());
                }
                catch(const std::exception& e)
                {
                    LAUGHTALE_ENGINR_LOG_WARNING("could not render game object because: " << e.what());
                }
                catch(...)
                {
                    LAUGHTALE_ENGINR_LOG_WARNING("could not render shader data");
                }

                texturesToUse.clear();
            }
            shaderBuffer->clear();
        }
    }

    void renderer::renderScene()
    {
        renderPipLine->SetClearColor(Scene->backgroundColor->getRGBA());
        renderPipLine->Clear();

        sortSceneToRender();
        batchSceneData();

        ViewProjectionMatrix = Scene->camera->getComponent<coreCameraControler>()->getCamera()->getViewProjectionMatrix();
        batchRenderScene();
        
    }
}  