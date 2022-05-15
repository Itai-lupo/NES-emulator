#pragma once
#include "LaughTaleEngine.h"

#include <algorithm>

#include <regex>

struct httpHeader: public LTE::GMNM::packetHeader
{
    std::string reqType;
    std::string reqPath;
    
    std::string headerData;

    std::map<std::string, std::string> headers; 

    std::string httpType;
    std::string status;
    std::string res;

    virtual uint32_t getContentLength() override
    {
        if(headers.find("Content-Length") != headers.end())
            return std::stoi( headers["Content-Length"] );
        else
            return 0;
    }

    virtual LTE::packetId getContentId() override
    {
        return 0;
    }

    virtual std::string toString() override
    {
        std::string resStr = "";
        resStr += httpType + " " + status + " " + res + "\n";
        
        for(auto& [headerType, headerValue]: headers)
            resStr += headerType + ": " + headerValue + "\n";
        return resStr;
    }
};

struct httpBody: public LTE::GMNM::packetBody
{
    std::string bodyData = "";
    
    virtual void *getData() override
    {
        return (void *)bodyData.c_str();
    }

    virtual std::string toString() override
    {
        return bodyData;
    }
};


struct httpJSONBody: public httpBody
{
    std::map<std::string, std::string> data; 
    
    virtual std::string toString() override
    {
        std::string resStr = "";
        for(auto& [dataName, dataValue]: data)
            resStr += dataName + ": " + dataValue + ",\n";
        return resStr;
    }
};

struct httpFileBody: public httpBody
{
    LTE::GMNM::byteStream& fileData;
    
    httpFileBody(LTE::GMNM::byteStream& fileData):fileData(fileData){}

    virtual std::string toString() override
    {
        std::string resStr = "file size: " + std::to_string(fileData.getSize());

        return resStr;
    }
};


class httpsProtocol: public LTE::GMNM::dataFormatter
{
    private:
        /* data */
    public:
            virtual uint32_t getHeaderSize() override
            {
                return 0;
            }

            void splitStringToMap(std::string &src, std::map<std::string, std::string>& dst, std::pair<std::string, std::string> delimiters)
            {                
                size_t pos = 0;
                size_t pos2;
                std::string token;
                while ((pos = src.find(delimiters.first)) != std::string::npos) {
                    token = src.substr(0, pos);
    
                    if((pos2 = token.find(delimiters.second)) != std::string::npos)
                        dst[token.substr(0, pos2)] = token.substr(pos2 + delimiters.second.length(), token.size());

                    src.erase(0, pos + delimiters.first.length());
                }
            }

            void formatHttpHeaders(httpHeader *resHeader, std::string& headersData)
            {
                std::string delimiter = "\r\n";

                size_t pos = 0;
                size_t pos2;
                std::string token;

                pos = headersData.find(delimiter);
                token = headersData.substr(0, pos);
                pos2 = token.find(" ");
                resHeader->httpType = token.substr(0, pos2);
                token.erase(0, pos2 + 1);
                pos2 = token.find(" ");
                resHeader->status = token.substr(0, pos2);
                token.erase(0, pos2 + 1);
                resHeader->res = token;
                headersData.erase(0, pos + delimiter.length());

                splitStringToMap(headersData, resHeader->headers, {"\r\n", ": "});                
            }

            void formatJsonBody(httpJSONBody *resHeader, std::string& bodyData)
            {
                size_t pos = 0;
                std::string token;

                bodyData.erase(0, 1);
                bodyData.erase(bodyData.length() - 1, bodyData.length());
                bodyData += ",";

                    
                bodyData.erase(std::remove(bodyData.begin(), bodyData.end(), '"'), bodyData.end());
                
                splitStringToMap(bodyData, resHeader->data, {",", ":"});
            }

            virtual void formatRecivedHeader(LTE::GMNM::byteStream& data, LTE::GMNM::packet& formatedHeader) override
            {
                LAUGHTALE_ENGINR_LOG_INFO(data)

                httpHeader *temp = new httpHeader();
                formatedHeader.header = temp;
                if(data.getSize() == 0)
                    return;
                temp->headerData = std::string((char*)data.getData().data());
                formatHttpHeaders(temp, temp->headerData);
                LAUGHTALE_ENGINR_LOG_INFO(temp->headerData)

            }

            virtual void formatRecivedBody(LTE::GMNM::byteStream& data, LTE::GMNM::packet& formatedData) override
            {
                httpBody *temp;

                if(data.getSize() == 0)
                {
                    formatedData.body = new httpBody(); 
                    return;
                }   
                
                std::string bodyData = std::string((char*)data.getData().data());

                if (dynamic_cast<httpHeader*>(formatedData.header)->headers["Content-Type"] == "application/json; charset=utf-8")
                {
                    temp = new httpJSONBody();
                    formatJsonBody((httpJSONBody*)temp, bodyData);
                }
                else if (dynamic_cast<httpHeader*>(formatedData.header)->headers["Content-Type"] == "application/octet-stream")
                {

                    temp = new httpFileBody(data); 
                    temp->bodyData = bodyData;
                }
                else{
                    temp = new httpBody(); 
                    temp->bodyData = bodyData;
                }

                formatedData.body = temp; 
                LAUGHTALE_ENGINR_LOG_INFO(formatedData)

            }

            
            virtual void formatHeaderToSend(LTE::GMNM::byteStream& buffer, LTE::GMNM::packet& dataToSend) override
            {  
                httpHeader *header = dynamic_cast<httpHeader *>(dataToSend.header);
                
                header->reqPath = std::regex_replace(header->reqPath, std::regex(" "), "%20");
                
                buffer <= header->reqType <= " " <= header->reqPath <= " HTTP/1.1\r\n"; 


                for(auto& [headerType, headerValue]: header->headers)
                    buffer <= headerType <= ": " <= headerValue <= "\r\n";

                buffer <= "\r\n";
            }

            virtual void formatBodyToSend(LTE::GMNM::byteStream& buffer, LTE::GMNM::packet& dataToSend) override
            {
                httpBody *bodyData = dynamic_cast<httpBody*>(dataToSend.body);
                buffer <= bodyData->bodyData;
                LAUGHTALE_ENGINR_LOG_INFO(dataToSend)

            }


};
