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

 /* ScriptData
 SDName: Boss_Baroness_Anastari
 SD%Complete: 90
 SDComment: MC disabled
 SDCategory: Stratholme
 EndScriptData */

#include "DBScripts/ScriptMgr.h"
#include "AI/ScriptDevAI/include/ScriptedCreature.h"
#include "stratholme.h"

#define SPELL_BANSHEEWAIL   16565
#define SPELL_BANSHEECURSE    16867
#define SPELL_SILENCE    18327
 //#define SPELL_POSSESS   17244

class boss_baroness_anastari : public CreatureScript
{
public:
    boss_baroness_anastari() : CreatureScript("boss_baroness_anastari") { }

    struct boss_baroness_anastariAI : public ScriptedAI
    {
        boss_baroness_anastariAI(Creature* c) : ScriptedAI(c)
        {
            pInstance = (ScriptedInstance*)me->GetInstanceData();
        }

        ScriptedInstance* pInstance;

        uint32 BansheeWail_Timer;
        uint32 BansheeCurse_Timer;
        uint32 Silence_Timer;
        //uint32 Possess_Timer;

        void Reset()
        {
            BansheeWail_Timer = 1000;
            BansheeCurse_Timer = 11000;
            Silence_Timer = 13000;
            //Possess_Timer = 35000;
        }

        void EnterCombat(Unit* /*who*/)
        {
        }

        void JustDied(Unit* /*Killer*/)
        {
            if (pInstance)
                pInstance->SetData(TYPE_BARONESS, IN_PROGRESS);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            //BansheeWail
            if (BansheeWail_Timer <= diff)
            {
                if (rand() % 100 < 95)
                    DoCastVictim(SPELL_BANSHEEWAIL);
                //4 seconds until we should cast this again
                BansheeWail_Timer = 4000;
            }
            else BansheeWail_Timer -= diff;

            //BansheeCurse
            if (BansheeCurse_Timer <= diff)
            {
                if (rand() % 100 < 75)
                    DoCastVictim(SPELL_BANSHEECURSE);
                //18 seconds until we should cast this again
                BansheeCurse_Timer = 18000;
            }
            else BansheeCurse_Timer -= diff;

            //Silence
            if (Silence_Timer <= diff)
            {
                if (rand() % 100 < 80)
                    DoCastVictim(SPELL_SILENCE);
                //13 seconds until we should cast this again
                Silence_Timer = 13000;
            }
            else Silence_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return GetInstanceAI<boss_baroness_anastariAI>(pCreature);
    }
};

void AddSC_boss_baroness_anastari()
{
    new boss_baroness_anastari();
}


