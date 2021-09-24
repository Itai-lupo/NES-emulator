#pragma once
#include "core.h"
#include "string.h"
#include "logger.h"
#include <unordered_map>
#include <functional>
#include <queue>

namespace LTE
{
    template<typename T> 
    class router
    {
        private:
            std::unordered_map<std::string, T> values;
            std::unordered_map<std::string, router<T>*> childs;
            
            std::queue<std::string> removeQueue;
            std::queue<std::string> deleteQueue;

            bool canRemove = true;
        public:

            router()
            {
                
            }

            void addRoute(std::string route)
            {
                size_t pos = 0;
                pos = route.find("/");

                std::string token = route.substr(0, pos);
                route.erase(0, pos + 1);
                if(pos == std::string::npos)
                    return;

                if(childs[token] == nullptr)
                    childs[token] = new router<T>();
                
                childs[token]->addRoute(route);
            }

            void addValue(std::string route, T value)
            {
                size_t pos = 0;
                pos = route.find("/");

                std::string token = route.substr(0, pos);
                if(pos == std::string::npos)
                {
                    values[route] = value;
                    return;
                }
                
                route.erase(0, pos + 1);
                if(!childs[token])
                    LAUGHTALE_ENGINR_LOG_FATAL("path: " << route << " wasn't found, please add the route before adding values");
                childs[token]->addValue(route, value);
            }

            T getValue(std::string route)
            {
                size_t pos = 0;
                pos = route.find("/");

                std::string token = route.substr(0, pos);
                if(pos == std::string::npos)
                {
                    return values[route];
                }
                
                route.erase(0, pos + 1);

                if(!childs[token])
                {
                    LAUGHTALE_ENGINR_LOG_ERROR("path: " << route << " wa'snt fount, please add the route before adding values");
                    return values["NULL DEFUALT VALUE PATH 123481455"];
                }
                return childs[token]->getValue(route);
            }

            void removeValue(std::string route)
            {
                if(!canRemove)
                {
                    removeQueue.push(route);
                    return;
                }
                size_t pos = 0;
                pos = route.find("/");

                std::string token = route.substr(0, pos);
                if(pos == std::string::npos)
                {
                    values.erase(token);
                    return;
                }

                route.erase(0, pos + 1);
                
                if(!childs[token])
                    return;

                childs[token]->removeValue(route);
            }

             void deleteValue(std::string route)
            {
                if(!canRemove)
                {
                    deleteQueue.push(route);
                    return;
                }

                size_t pos = 0;
                pos = route.find("/");

                std::string token = route.substr(0, pos);
                if(pos == std::string::npos)
                {
                    delete values[token];
                    values.erase(token);
                    return;
                }

                route.erase(0, pos + 1);
                
                if(!childs[token])
                    return;

                childs[token]->deleteValue(route);
            }

            T operator[](std::string route)
            {
                size_t pos = 0;
                pos = route.find("/");

                std::string token = route.substr(0, pos);
                if(pos == std::string::npos)
                {
                
                    return values[route];
                }

                route.erase(0, pos + 1);

                return childs[token]->getValue(route);
            }

            void clear()
            {

                
                values.clear();
                for(auto& child: childs)
                {
                    child.second->clear(); 
                    delete child.second;
                }
                childs.clear();

            }

            void clearAndDelete()
            {

                for(auto value: values)
                { 
                    delete value.second;
                }
                values.clear();
                
                for(auto& child: childs)
                {
                    child.second->clearAndDelete(); 
                    delete child.second;
                }
                childs.clear();

            }

            void clearQueues()
            {
                while(!removeQueue.empty())
                {
                    removeValue(removeQueue.front());
                    removeQueue.pop();
                }

                while(!deleteQueue.empty())
                {
                    deleteValue(deleteQueue.front());
                    LAUGHTALE_ENGINR_LOG_INFO(deleteQueue.front())
                    deleteQueue.pop();
                }
            }

            void itrateFrom(std::function<void(T value)> callback, std::string fromRoute = "")
            {
                if(fromRoute == "")
                {
                    canRemove = false;
                    for(std::pair<const std::basic_string<char>, T>& val: values)
                    {
                        if(val.second != values["NULL DEFUALT VALUE PATH 123481455"])
                            callback(val.second);
                    }

                    for(auto& child: childs)
                    {
                        if(child.second)
                            child.second->itrateFrom(callback);
                    }
    
                    canRemove = true;
                    clearQueues();
                    return;
                }

                size_t pos = 0;
                pos = fromRoute.find("/");

                std::string token = fromRoute.substr(0, pos);
                
                fromRoute.erase(0, pos + 1);

                if(!childs[token])
                {
                    LAUGHTALE_ENGINR_LOG_ERROR("path: " << fromRoute << " wa'snt found, please add the route before itrating it");
                }
                childs[token]->itrateFrom(callback, fromRoute);
            }

    };       

}
