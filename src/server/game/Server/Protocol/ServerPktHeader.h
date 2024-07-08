/*
 * Copyright (C) 2008-2016 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __SERVERPKTHDR_H__
#define __SERVERPKTHDR_H__

#include "Define.h"

#pragma pack(push, 1)

struct ServerPktHeader
{
    ServerPktHeader(uint32 size, uint32 cmd, bool encrypt) : size(size)
    {
        if (encrypt)
        {
            uint32 data = (size << 13) | (cmd & MAX_SMSG_OPCODE_NUMBER);
            memcpy(&header[0], &data, 4);
            //_authCrypt->EncryptSend(reinterpret_cast<uint8*>(&header[0]), getHeaderLength());
        }
        else
        {
            // Dynamic header size is not needed anymore, we are using not encrypted part for only the first few packets
            memcpy(&header[0], &size, 2);
            memcpy(&header[2], &cmd, 2);
        }
    }

    uint8 getHeaderLength()
    {
        return 4;
    }

    const uint32 size;
    uint8 header[4];
};

#pragma pack(pop)

#endif
