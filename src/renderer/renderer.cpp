#include "renderer.h"
#include "mesh.h"
#include "meshRenderer.h"
#include "shaderTypes.h"

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
        std::vector<std::pair<textureId, int>> textureSlots;
        std::vector<texture *> textures;

        renderPipLine->SetClearColor(Scene->backgroundColor->getRGBA());
        renderPipLine->Clear();

        std::sort(
            Scene->objects->begin(),
            Scene->objects->end(),
            [=](gameObject *a, gameObject *b) -> bool
            {
                return a->getComponent<material>()->getTextureId() > b->getComponent<material>()->getTextureId();
            });

        const glm::mat4& ViewProjectionMatrix = Scene->camera->getComponent<coreCameraControler>()->getCamera()->getViewProjectionMatrix();
        
        textureId lastTexture;
        int textureSlotCount = 0;

        for(gameObject *toRender: *Scene->objects)
        {
            mesh *obj = toRender->getComponent<mesh>();
            material *objMat = toRender->getComponent<material>();
            shaderRenderBuffer *s  = submitShape(obj, objMat);
            if(!s)
                continue;
            shadersToRender.insert(s);
            if(objMat->getTexture() && objMat->getTextureId() != lastTexture)
            {
                lastTexture = objMat->getTextureId();
                textureSlots.push_back({lastTexture, textureSlotCount + 1});
                textures.push_back(objMat->getTexture());
                textureSlotCount = (textureSlotCount + 1) % 31;
            }
        }

        for(int i = 0; i < 32 && i < textures.size(); i++)
        {
            textures[i]->bind(textureSlots[i].second);
        }

        for(auto& shaderBuffer: shadersToRender)
        {

            try
            {
                shader *s = shaderBuffer->getShader();

                shaderBuffer->setTextureIndex(textureSlots);
        
                shaderBuffer->bindRenderBatch();
                s->bind();
                
                int *samplers = (int*)calloc(4, 8);
                for(int *i = samplers; i < samplers + 8; i++)
                    *i = ((long)i - (long)samplers)/4;
                
                s->setUniformMat4f("viewProjection", ViewProjectionMatrix);
                s->setUniform1iv("textures", samplers, 8);

                renderPipLine->DrawIndexed(shaderBuffer->getVertexCount()); 
            }
            catch(const std::exception& e)
            {
                LAUGHTALE_ENGINR_LOG_WARNING("could not render game object because: " << e.what());
            }
            catch(...)
            {
                LAUGHTALE_ENGINR_LOG_WARNING("could not render shader data");
            }
        }
    }
}  