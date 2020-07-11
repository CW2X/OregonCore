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
 SDName: Boss_Lorekeeper_Polkelt
 SD%Complete: 100
 SDComment:
 SDCategory: Scholomance
 EndScriptData */

#include "DBScripts/ScriptMgr.h"
#include "AI/ScriptDevAI/include/ScriptedCreature.h"
#include "scholomance.h"

#define SPELL_VOLATILEINFECTION      24928
#define SPELL_DARKPLAGUE             18270
#define SPELL_CORROSIVEACID          23313
#define SPELL_NOXIOUSCATALYST        18151


class boss_lorekeeper_polkelt : public CreatureScript
{
public:
    boss_lorekeeper_polkelt() : CreatureScript("boss_lorekeeper_polkelt") { }

    struct boss_lorekeeper_polkeltAI : public ScriptedAI
    {
        boss_lorekeeper_polkeltAI(Creature* c) : ScriptedAI(c) {}

        uint32 VolatileInfection_Timer;
        uint32 Darkplague_Timer;
        uint32 CorrosiveAcid_Timer;
        uint32 NoxiousCatalyst_Timer;

        void Reset()
        {
            VolatileInfection_Timer = 38000;
            Darkplague_Timer = 8000;
            CorrosiveAcid_Timer = 45000;
            NoxiousCatalyst_Timer = 35000;
        }

        void JustDied(Unit* /*killer*/)
        {
            ScriptedInstance* pInstance = (ScriptedInstance*)me->GetInstanceData();
            if (pInstance)
            {
                pInstance->SetData(DATA_LOREKEEPERPOLKELT_DEATH, 0);

                if (pInstance->GetData(TYPE_GANDLING) == IN_PROGRESS)
                    me->SummonCreature(1853, 180.73f, -9.43856f, 75.507f, 1.61399f, TEMPSUMMON_DEAD_DESPAWN, 0);
            }
        }

        void EnterCombat(Unit* /*who*/)
        {
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            //VolatileInfection_Timer
            if (VolatileInfection_Timer <= diff)
            {
                DoCastVictim(SPELL_VOLATILEINFECTION);
                VolatileInfection_Timer = 32000;
            }
            else VolatileInfection_Timer -= diff;

            //Darkplague_Timer
            if (Darkplague_Timer <= diff)
            {
                DoCastVictim(SPELL_DARKPLAGUE);
                Darkplague_Timer = 8000;
            }
            else Darkplague_Timer -= diff;

            //CorrosiveAcid_Timer
            if (CorrosiveAcid_Timer <= diff)
            {
                DoCastVictim(SPELL_CORROSIVEACID);
                CorrosiveAcid_Timer = 25000;
            }
            else CorrosiveAcid_Timer -= diff;

            //NoxiousCatalyst_Timer
            if (NoxiousCatalyst_Timer <= diff)
            {
                DoCastVictim(SPELL_NOXIOUSCATALYST);
                NoxiousCatalyst_Timer = 38000;
            }
            else NoxiousCatalyst_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_lorekeeper_polkeltAI(pCreature);
    }

};

void AddSC_boss_lorekeeperpolkelt()
{
    new boss_lorekeeper_polkelt();
}


