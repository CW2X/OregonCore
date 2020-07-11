﻿/*
 * This file is part of the MaNGOSCore Project. See AUTHORS file for Copyright information
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

#ifndef __WORLDSOCKETMGR_H
#define __WORLDSOCKETMGR_H


#include <ace/Singleton.h>
#include <ace/Thread_Mutex.h>

#include "Platform/Define.h"

class WorldSocket;
class ReactorRunnable;
class ACE_Event_Handler;

// Manages all sockets connected to peers and network threads
class WorldSocketMgr
{
    public:
        friend class WorldSocket;
        friend class ACE_Singleton<WorldSocketMgr, ACE_Thread_Mutex>;

        // Start network, listen at address:port .
        int StartNetwork (uint16 port, const char* address);

        // Stops all network threads, It will wait for all running threads .
        void StopNetwork();

        // Wait untill all network threads have "joined" .
        void Wait();

        // Make this class singleton .
        static WorldSocketMgr* Instance();

    private:
        int OnSocketOpen(WorldSocket* sock);

        int StartReactiveIO(uint16 port, const char* address);

    private:
        WorldSocketMgr();
        virtual ~WorldSocketMgr();

        ReactorRunnable* m_NetThreads;
        size_t m_NetThreadsCount;

        int m_SockOutKBuff;
        int m_SockOutUBuff;
        bool m_UseNoDelay;

        ACE_Event_Handler* m_Acceptor;
};

#define sWorldSocketMgr WorldSocketMgr::Instance()

#endif


