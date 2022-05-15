#pragma once
#include "core.h"
#include "material.h"
#include "entity.h"
#include "mesh.h"

#include <glm/glm.hpp>
#include <map>

namespace LTE
{
    template <typename T>
    class vector3d 
    {
        public:
            vector3d(size_t d1=0, size_t d2=0, size_t d3=0, T const & t=T()) :
                d1(d1), d2(d2), d3(d3), data(d1*d2*d3, t)
            {}

            T & operator()(size_t i, size_t j, size_t k) 
            {
                    return (i<=d1 && j<=d2 && k<=d3) ? data[i*d2*d3 + j*d3 + k] 
                                                    : data.at(i*d2*d3 + j*d3 + k);
            }

            T const & operator()(size_t i, size_t j, size_t k) const 
            {
                return data[i*d2*d3 + j*d3 + k];
            }

            void resize(const size_t _d1=0, const size_t _d2=0, const size_t _d3=0)
            {
                data.resize(_d1*_d2*_d3);
                d1=_d1;
                d2=_d2;
                d3=_d3;
            }

            void shrink_to_fit()
            {
                data.shrink_to_fit();
            }

            const size_t length() const
            {
                return data.size();
            }

            const size_t capacity() const
            {
                return data.capacity();
            }

            const size_t x() const
            {
                return d1;
            }

            const size_t y() const
            {
                return d2;
            }

            const size_t z() const
            {
                return d3;
            }


            private:
                size_t d1,d2,d3;
                std::vector<T> data;
        };

    class tilemap
    {
        public:
            class tileBuilder
            {
                private:
                    tilemap *parent;
                    int layer;
                    glm::ivec2 tileDimensions;
                    glm::ivec2 tilePostion;
                    material *tileMat;
                    std::string tileName;
                    std::vector<component*> tileComponents;

                    static inline float tilePostions[12] =
                        {
                            -0.5,  0.5f, 0.0f,
                            -0.5, -0.5f, 0.0f,
                            0.5, -0.5f, 0.0f,
                            0.5,  0.5f, 0.0f
                        };

                    static inline unsigned int tileIndices[6] =
                        {
                            0,
                            1,
                            2,
                            0,
                            3,
                            2,
                        };

                public: 
                    tileBuilder(tilemap *parent):parent(parent){}
                    ~tileBuilder(){}

                    tileBuilder *setLayer(int layer)
                    {
                        this->layer = layer;
                        return this;
                    }

                    tileBuilder *setTileDimensions(glm::ivec2 tileDimensions)
                    {
                        this->tileDimensions = tileDimensions;
                        return this;                        
                    }
                
                    tileBuilder *setTilePostion(glm::ivec2 tilePostion)
                    {
                        this->tilePostion = tilePostion;
                        return this;
                    }


                    tileBuilder *setTileMatrial(material *tileMat)
                    {
                        this->tileMat = tileMat;
                        return this;                        
                    }
                    
                    tileBuilder *setTileName(const std::string& tileName)
                    {
                        this->tileName = tileName;
                        return this;                        
                    }
                    
                    tileBuilder *AddCompponnetToTile(component *toAdd)
                    {
                        this->tileComponents.push_back(toAdd);
                        return this;                        
                    }

                    tileBuilder *reset()
                    {
                        layer = 0;
                        tileDimensions = {1, 1};
                        tilePostion = {-1, -1};
                        tileMat = nullptr;
                        tileName = "";
                        tileComponents.clear();
                        return this;
                    }


                    entityTaleId build()
                    {
                        return entityManger::addEntity([&, this](gameObject::gameObjectBuilder *builder)
                            {
                                builder->
                                    setObjectName(this->tileName == "" ? this->tileName: "tile " + std::to_string(tilePostion.x) + ", " + std::to_string(tilePostion.y) + ", " + std::to_string(layer))->
                                    setObjectTransform(
                                        {{((float) tilePostion.x) / parent->tilesCount.x * 2.0  - 1.0f + (parent->tileDimensions.x *  (float)tileDimensions.x /(float)parent->tilemapDimensions.x),
                                        ((float) tilePostion.y) / parent->tilesCount.y * 2.0 - 1.0f +  (parent->tileDimensions.y *  (float)tileDimensions.y /(float)parent->tilemapDimensions.y), (float)layer / (float)parent->layerCount}, 
                                        {0, 0, 0}, 
                                        {parent->tileDimensions.x *  (float)tileDimensions.x /(float)parent->tilemapDimensions.x  * 2.0, parent->tileDimensions.y *  (float)tileDimensions.y / (float)parent->tilemapDimensions.y  * 2.0, layer / parent->layerCount}})->
                                        setWindowId(parent->winId)->
                                        addComponent(LTE::mesh::build([&](LTE::mesh::meshBuilder *builder)
                                            { 
                                                builder->
                                                    setIndexBuffer(tileIndices, 6)->
                                                    setShaderName("res/topDownScene/shaders/tile.glsl")->
                                                    setVertices(tilePostions, 12); 
                                            }))->            
                                        addComponent(tileMat);
                                        
                                for(auto& comp: tileComponents)
                                    builder->addComponent(comp);

                            });
                    }

                    int getLayer()
                    {
                        return layer;
                    }

                    glm::ivec2 getTilePostion()
                    {
                        return tilePostion;
                    }
            };

        private:
            glm::ivec2 tilemapDimensions;
            glm::ivec2 tileDimensions;
            glm::ivec2 tilesCount;
            int layerCount;
            windowPieceId winId;
            
            vector3d<entityTaleId> tiles;
            tileBuilder *builder;
        public:
            tilemap(glm::ivec2 tilemapDimensions, glm::ivec2 tileDimensions, int layerCount, windowPieceId winId): 
                tilemapDimensions(tilemapDimensions), tileDimensions(tileDimensions), tilesCount(tilemapDimensions / tileDimensions), winId(winId), layerCount(layerCount), tiles(layerCount, tilesCount.x, tilesCount.y)
            {
                builder = new tileBuilder(this);
            }
            ~tilemap(){}

            void addTile(const std::function<void(tileBuilder *Builder)>& buildTile)
            {
                builder->reset();
                buildTile(builder);
                tiles(builder->getLayer(), builder->getTilePostion().x, builder->getTilePostion().y) = builder->build();
            }

            void moveTileTo(windowPieceId winId, const glm::ivec2& tileNewPostion, int layer = -1);

    };
}