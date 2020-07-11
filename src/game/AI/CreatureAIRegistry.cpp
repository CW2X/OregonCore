/*
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

#include "AI/BaseAI/PassiveAI.h"
#include "AI/BaseAI/ReactorAI.h"
#include "Combat/CombatAI.h"
#include "AI/BaseAI/GuardAI.h"
#include "AI/BaseAI/PetAI.h"
#include "AI/BaseAI/TotemAI.h"
#include "AI/EventAI/CreatureEventAI.h"
#include "MotionGenerators/RandomMovementGenerator.h"
#include "MotionGenerators/MovementGeneratorImpl.h"
#include "AI/CreatureAIRegistry.h"
#include "MotionGenerators/WaypointMovementGenerator.h"
#include "AI/BaseAI/CreatureAIFactory.h"
#include "AI/ScriptDevAI/base/SmartAI.h"

//#include "AI/BaseAI/CreatureAIImpl.h"
namespace AIRegistry
{
void Initialize()
{
    (new CreatureAIFactory<NullCreatureAI>("NullCreatureAI"))->RegisterSelf();
    (new CreatureAIFactory<TriggerAI>("TriggerAI"))->RegisterSelf();
    (new CreatureAIFactory<AggressorAI>("AggressorAI"))->RegisterSelf();
    (new CreatureAIFactory<ReactorAI>("ReactorAI"))->RegisterSelf();
    (new CreatureAIFactory<PassiveAI>("PassiveAI"))->RegisterSelf();
    (new CreatureAIFactory<CritterAI>("CritterAI"))->RegisterSelf();
    (new CreatureAIFactory<GuardAI>("GuardAI"))->RegisterSelf();
    (new CreatureAIFactory<PetAI>("PetAI"))->RegisterSelf();
    (new CreatureAIFactory<TotemAI>("TotemAI"))->RegisterSelf();
    (new CreatureAIFactory<CombatAI>("CombatAI"))->RegisterSelf();
    (new CreatureAIFactory<PossessedAI>("PossessedAI"))->RegisterSelf();
    (new CreatureAIFactory<CreatureEventAI>("EventAI"))->RegisterSelf();
    (new CreatureAIFactory<SmartAI>("SmartAI"))->RegisterSelf();

    (new GameObjectAIFactory<GameObjectAI>("GameObjectAI"))->RegisterSelf();
    (new GameObjectAIFactory<SmartGameObjectAI>("SmartGameObjectAI"))->RegisterSelf();

    (new MovementGeneratorFactory<RandomMovementGenerator<Creature> >(RANDOM_MOTION_TYPE))->RegisterSelf();
    (new MovementGeneratorFactory<WaypointMovementGenerator<Creature> >(WAYPOINT_MOTION_TYPE))->RegisterSelf();
}
}



