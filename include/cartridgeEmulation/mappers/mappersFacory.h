#pragma once

#include "mapper.h"
#include "000.h"

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
            }

        }
};  