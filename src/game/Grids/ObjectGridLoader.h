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

#ifndef MANGOS_OBJECTGRIDLOADER_H
#define MANGOS_OBJECTGRIDLOADER_H

#include "Utilities/TypeList.h"
#include "Platform/Define.h"
#include "GameSystem/GridLoader.h"
#include "Maps/GridDefines.h"
#include "Grids/Cell.h"

class ObjectWorldLoader;

class ObjectGridLoader
{
        friend class ObjectWorldLoader;

    public:
        ObjectGridLoader(NGridType& grid, Map* map, const Cell& cell)
            : i_cell(cell), i_grid(grid), i_map(map), i_gameObjects(0), i_creatures(0), i_corpses (0)
        {}

        void Visit(GameObjectMapType& m);
        void Visit(CreatureMapType& m);
        void Visit(CorpseMapType&) {}

        void Visit(DynamicObjectMapType&) { }

        void LoadN(void);

    private:
        Cell i_cell;
        NGridType& i_grid;
        Map* i_map;
        uint32 i_gameObjects;
        uint32 i_creatures;
        uint32 i_corpses;
};

//Stop the creatures before unloading the NGrid
class ObjectGridStoper
{
    public:
        void Visit(CreatureMapType &m);
        template<class T> void Visit(GridRefManager<T> &) {}
};

//Move the foreign creatures back to respawn positions before unloading the NGrid
class ObjectGridEvacuator
{
    public:
        void Visit(CreatureMapType& m);

        template<class T> void Visit(GridRefManager<T> &) {}
};

//Clean up and remove from world
class ObjectGridCleaner
{
    public:
        template<class T> void Visit(GridRefManager<T>&);
};

//Delete objects before deleting NGrid
class ObjectGridUnloader
{
    public:
        template<class T> void Visit(GridRefManager<T> &m);
};
#endif

