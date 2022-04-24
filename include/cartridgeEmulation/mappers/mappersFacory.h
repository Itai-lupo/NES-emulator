#pragma once

#include "mapper.h"
#include "000.h"
#include "001.h"
#include "002.h"
#include "004.h"

#include "LaughTaleEngine.h"

struct mappersData
{
    uint8_t mapperID;
    uint8_t prgBanks;
    uint8_t chrBanks;
};


class mappersFacory
{
    public:
        static mapper *createMapper(const mappersData& data)
        {
            switch (data.mapperID)
            {
                case 0:
                    return new mapper_000(data.prgBanks, data.chrBanks);
                    break;
                case 1:
                    return new mapper_001(data.prgBanks, data.chrBanks);
                    break;
                case 2:
                    return new mapper_002(data.prgBanks, data.chrBanks);
                    break;
                case 4:
                    return new mapper_004(data.prgBanks, data.chrBanks);
                    break;
                default:
                    LAUGHTALE_ENGINR_LOG_FATAL("mapper not supported")
            }

        }
};  