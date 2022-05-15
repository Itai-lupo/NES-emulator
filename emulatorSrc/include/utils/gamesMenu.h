#pragma once
#include "LaughTaleEngine.h"
#include "cartridge.h"
#include "imgui.h"

#include "httpsProtocol.h"
#include <filesystem>

#include <iostream>
#include <fstream>  

class serverConction
{
    private:
        std::string host;
        int port;
        LTE::connectionId id;
        std::vector<std::string> gamesList;


        std::string authToken;
        std::string email, password;
        std::string lastGame = "";

    public:
        serverConction( const std::string& host, int port, const std::string& email, const std::string& password): 
            host(host), port(port), email(email), password(password)
        {            
            id = LTE::GMNM::connectionsManager::addConnection(host, port, new httpsProtocol());
        }
        
        std::string getName()
        {
            return host + ":" + std::to_string(port);
        }

        LTE::connectionId getId()
        {
            return id;
        }

        void setGames(std::string& games)
        {
            gamesList.clear();

            size_t pos = 0;
            std::string token;

            while ((pos = games.find(";")) != std::string::npos) {
                token = games.substr(0, pos);
                gamesList.push_back(token);
                games.erase(0, pos + 1);
            }
        }

        void handleResponse(LTE::GMNM::packet data)
        {
            httpHeader *headerData = dynamic_cast<httpHeader *>(data.header);
            if(headerData->status == "401")
            {
                login();
                if(lastGame != "")
                    getGame(lastGame);
            }
            else if(headerData->headers["Content-Type"] == "application/json; charset=utf-8")
            {
                httpJSONBody *bodyData = dynamic_cast<httpJSONBody *>(data.body);
                if(headerData->headers["resType"] == "gameList")
                {
                    setGames(bodyData->data["data"]);
                }
                else if(headerData->headers["resType"] == "login")
                {
                    authToken = bodyData->data["data"];
                }
            }
            else if (headerData->headers["Content-Type"] == "application/octet-stream")
            {
                std::ofstream outfile ("res/roms/" + dynamic_cast<httpHeader*>(data.header)->headers["gameName"]);
                httpFileBody* a = dynamic_cast<httpFileBody*>(data.body);
                LTE::GMNM::byteStream& fileData = a->fileData;
                char temp;
                
                for (size_t i = 0; i < fileData.getSize(); i++)
                {
                    temp = fileData.getData()[i] ; 
                    outfile << temp;
                }
                outfile.close();
            }
        }

        void login()
        {
            LTE::GMNM::packet data;
            httpHeader *temp = new httpHeader();
            httpBody *tempBody = new httpBody();
            
            data.header = temp;
            data.body = tempBody;

            temp->reqType = "POST";
            temp->reqPath = "/api/v1/login";
            
            temp->headers["User-Agent"] = "Mozilla/4.0 (compatible; MSIE5.01; Windows NT)";
            temp->headers["Host"] = host + ":" + std::to_string(port);
            temp->headers["Accept-Language"] = "en-us";
            temp->headers["Accept"] = "*/*";
            temp->headers["Accept-Encoding"] = "gzip, deflate, br";
            temp->headers["Connection"] = "Keep-Alive";
            temp->headers["Content-Type"] = "application/json";
            
            tempBody->bodyData = "{"
                                "\"email\": \"" + email + "\"," 
                                "\"password\": \"" + password + "\""
                                "}";
            
            temp->headers["Content-Length"] = std::to_string(tempBody->bodyData.length());
            LTE::GMNM::connectionsManager::sendData(id, data);
        }

        void getGame(const std::string& gameName)
        {
            LTE::GMNM::packet data;
            httpHeader *temp = new httpHeader();
            data.body = new httpBody();

            data.header = temp;
 
            temp->reqType = "GET";
            temp->reqPath = "/api/v1/downloadGame/" + gameName;
            
            temp->headers["User-Agent"] = "Mozilla/4.0 (compatible; MSIE5.01; Windows NT)";
            temp->headers["Host"] = host + ":" + std::to_string(port);
            temp->headers["Accept-Language"] = "en-us";
            temp->headers["Accept-Encoding"] = "gzip, deflate";
            temp->headers["Content-Length"] = "0";
            temp->headers["Connection"] = "Keep-Alive";
            temp->headers["authorization"] =  "Bearer " + authToken;
            
            LTE::GMNM::connectionsManager::sendData(id, data);
        }

        const std::vector<std::string>& getGamesList()
        {
            return gamesList;
        }
};

struct loadGameEvent: public LTE::coreEventData
{
    loadGameEvent(const std::string& route): coreEventData(route){}

    std::string gamePath;
};

class gamesMenu
{
    public:

        static inline void serverHandShake(LTE::gameObject *eventEntity, LTE::coreEventData *sendor)
        {
            LTE::GMNM::connectionData *eventData = dynamic_cast<LTE::GMNM::connectionData *>(sendor);
            
            LTE::GMNM::packet data;
            httpHeader *temp = new httpHeader();
            data.header = temp;
            data.body = new httpBody();
            temp->reqType = "GET";
            temp->reqPath = "/api/v1/getGameList";
            
            temp->headers["User-Agent"] = "Mozilla/4.0 (compatible; MSIE5.01; Windows NT)";
            temp->headers["Host"] = eventData->ip + ":" + std::to_string(eventData->port);
            temp->headers["Accept-Language"] = "en-us";
            temp->headers["Accept-Encoding"] = "gzip, deflate";
            temp->headers["Content-Length"] = "0";
            temp->headers["Connection"] = "Keep-Alive";

            eventData->send(data);
        }

        static inline void handelResponse(LTE::gameObject *eventEntity, LTE::coreEventData *sendor)
        {
            LTE::GMNM::connectionReadData *eventData = dynamic_cast<LTE::GMNM::connectionReadData *>(sendor);
            LTE::GMNM::packet data = eventData->data;
            for(auto server: servers)
            {
                if(server->getId() == eventData->conId)
                    server->handleResponse(eventData->data);
            }
        }

    private:

        static inline int page;
        static inline bool showDebugInfo = false;
        static inline LTE::windowPieceId winId;
        static inline cartridge *cart;
        static inline loadGameEvent *sendorData;
        static inline LTE::coreEventData *qSendorData;
        static inline bool showMenu = true;
        static inline std::vector<serverConction*> servers;
 

        static void quiteGame(LTE::gameObject *eventEntity, LTE::coreEventData *sendor)
        {
            LTE::KeyData *e = dynamic_cast<LTE::KeyData *>(sendor);
            if(e->key == LT_KEY_ESCAPE)
            {
                LTE::eventManger::trigerEvent(qSendorData);
                cart->unload();
                showMenu = true;
            }
        }

        static void ImGuiRenderGameMenu(LTE::gameObject *eventEntity, LTE::coreEventData *sendor)
        {
            if(!showMenu) return;

            LTE::onUpdateData *eventData = static_cast<LTE::onUpdateData *>(sendor);
            
            if (!ImGui::Begin("game list"))
            {
                ImGui::End();
                return;
            }

            std::filesystem::recursive_directory_iterator iter("./res/roms");
            std::filesystem::recursive_directory_iterator end;
            while (iter != end)
            {
                if (ImGui::Button(iter->path().stem().c_str()))
                {
                    sendorData->gamePath = iter->path().string();
                    LTE::eventManger::trigerEvent(sendorData);
                    showMenu = false;
                }

                std::error_code ec;
                iter.increment(ec);
                if (ec)
                    LAUGHTALE_ENGINR_LOG_ERROR("Error While Accessing : " << iter->path().string() << " :: " << ec.message());
            }

            ImGui::End();
        }

        static void ImGuiRenderServerMenu(LTE::gameObject *eventEntity, LTE::coreEventData *sendor)
        {
            if(!showMenu) return;

            
            if (!ImGui::Begin("server list"))
            {
                ImGui::End();
                return;
            }

            static char hostBuffer[100];
            static char emailBuffer[100];
            static char passwordBuffer[100];
            static int port = 8080;

            ImGui::InputText("##serverHost", hostBuffer, 100);
            ImGui::InputInt("##serverPort", &port);

            ImGui::InputText("##email", emailBuffer, 100);
            ImGui::InputText("##password", passwordBuffer, 100, ImGuiInputTextFlags_Password);

            if(ImGui::Button("add"))
            {
                serverConction *temp = new serverConction(hostBuffer, port, emailBuffer, passwordBuffer);

                if(temp->getId() == 0)
                    delete temp;
                else{
                    servers.push_back(temp);
                    temp->login();
                }

                strcpy(hostBuffer, "\0");
                port = 0;
            }

            for(auto server: servers)
            {
                if(ImGui::CollapsingHeader(server->getName().c_str()))
                {
                    for (auto& game: server->getGamesList())
                    {
                        if(ImGui::Button(game.c_str()))
                            server->getGame(game);
                    }
                }
            }
            
            ImGui::End();
        }


    public:
        static void init(LTE::windowPieceId winId, cartridge *cart){
            gamesMenu::winId = winId;
            gamesMenu::cart = cart;
            
            LTE::eventManger::addCoustemEventsRoute("load game/");
            LTE::eventManger::addCoustemEventsRoute("quite game/");
            

            sendorData = new loadGameEvent("load game/");
            qSendorData = new LTE::coreEventData("quite game/");

            // LTE::eventManger::startBuildingEvent()->
            //     setEventRoute("ImGui render/show games menu")->
            //     setEventCallback(ImGuiRenderGameMenu)->
            //     setWindowId(winId)->add();

            // LTE::eventManger::startBuildingEvent()->
            //     setEventRoute("ImGui render/show server menu")->
            //     setEventCallback(ImGuiRenderServerMenu)->
            //     setWindowId(winId)->add();
            
            LTE::eventManger::startBuildingEvent()->
                setEventRoute("Key pressed/check if quite game")->
                setEventCallback(quiteGame)->
                setWindowId(winId)->add();
            
            // LTE::eventManger::startBuildingEvent()->
            //     setEventRoute("server connection/server hand shake")->
            //     setEventCallback(serverHandShake)->add();


            // LTE::eventManger::startBuildingEvent()->
            //     setEventRoute("message received/handel response")->
            //     setEventCallback(handelResponse)->add();


        }
};

