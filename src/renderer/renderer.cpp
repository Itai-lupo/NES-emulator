#include "renderer.h"
#include "mesh.h"
#include "meshRenderer.h"
#include "shaderTypes.h"
#include "LTEError.h"
#include "app.h"

#include <map>
#include <set>
#include <algorithm>

namespace LTE
{
    shaderRenderBuffer *renderer::submitShape(mesh *shape, material *shapeMatrial)
    {
        shaderRenderBuffer *s  = windowManger::getWindow(shape->getWindowId())->assetLibrary->getAsset<shaderRenderBuffer>(shape->getShaderName());
        if(!s)
            return nullptr;
        s->pushShape(shape, shapeMatrial);
        return s;
    }

    void renderer::renderScene(windowPieceId winId, renderApi *renderPipLine)
    {
        window *win;
        scene *Scene;
        win = windowManger::getWindow(winId);
        Scene = win->activeScene;
        renderPipLine = renderPipLine;
        std::set<shaderRenderBuffer*> shadersToRender;
        std::map<textureId, int> texturesToUse;
        uint8_t textureCounter = 0;
        std::vector<std::pair<textureId, int>> textureSlots;
        std::map<textureId, texture *> textures;

        renderPipLine->SetClearColor(Scene->backgroundColor->getRGBA());
        renderPipLine->Clear();

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

        const glm::mat4& ViewProjectionMatrix = Scene->camera->getComponent<coreCameraControler>()->getCamera()->getViewProjectionMatrix();
        
        textureId lastTexture;
        int textureSlotCount = 0;

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


        for(auto& shaderBuffer: shadersToRender)
        {
            int i = 0;
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
                    
                    shaderBuffer->bindRenderBatch();
                    
                    renderPipLine->DrawIndexed(shaderBuffer->getVertexCount()); 
                    i++;
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
}  