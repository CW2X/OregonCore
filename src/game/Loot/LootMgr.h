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

#ifndef MANGOS_LOOTMGR_H
#define MANGOS_LOOTMGR_H

#include "Entities/ItemEnchantmentMgr.h"
#include "ByteBuffer.h"
#include "Utilities/LinkedReference/RefManager.h"
#include "Globals/Conditions.h"

#include <map>
#include <vector>

#define MAX_NR_LOOT_ITEMS 16
// note: the client cannot show more than 16 items total
#define MAX_NR_QUEST_ITEMS 32
// unrelated to the number of quest items shown, just for reserve

enum LootMethod
{
    FREE_FOR_ALL      = 0,
    ROUND_ROBIN       = 1,
    MASTER_LOOT       = 2,
    GROUP_LOOT        = 3,
    NEED_BEFORE_GREED = 4
};

enum PermissionTypes
{
    ALL_PERMISSION              = 0,
    GROUP_PERMISSION            = 1,
    MASTER_PERMISSION           = 2,
    RESTRICTED_PERMISSION       = 3,
    ROUND_ROBIN_PERMISSION      = 4,
    OWNER_PERMISSION            = 5,
    NONE_PERMISSION             = 6
};

enum LootType
{
    LOOT_NONE                   = 0,

    LOOT_CORPSE                 = 1,
    LOOT_SKINNING               = 2,
    LOOT_FISHING                = 3,

    LOOT_PICKPOCKETING          = 4,    //!< unsupported by client, sending LOOT_SKINNING instead
    LOOT_DISENCHANTING          = 5,    //!< unsupported by client, sending LOOT_SKINNING instead
    
    LOOT_PROSPECTING            = 6,    //!< unsupported by client, sending LOOT_SKINNING instead
    LOOT_INSIGNIA               = 7,    //!< unsupported by client, sending LOOT_SKINNING instead
    LOOT_FISHINGHOLE            = 8     //!< unsupported by client, sending LOOT_FISHING instead
};

enum LootError
{
    LOOT_ERROR_DIDNT_KILL               = 0,    // You don't have permission to loot that corpse.
    LOOT_ERROR_TOO_FAR                  = 4,    // You are too far away to loot that corpse.
    LOOT_ERROR_BAD_FACING               = 5,    // You must be facing the corpse to loot it.
    LOOT_ERROR_LOCKED                   = 6,    // Someone is already looting that corpse.
    LOOT_ERROR_NOTSTANDING              = 8,    // You need to be standing up to loot something!
    LOOT_ERROR_STUNNED                  = 9,    // You can't loot anything while stunned!
    LOOT_ERROR_PLAYER_NOT_FOUND         = 10,   // Player not found
    LOOT_ERROR_PLAY_TIME_EXCEEDED       = 11,   // Maximum play time exceeded
    LOOT_ERROR_MASTER_INV_FULL          = 12,   // That player's inventory is full
    LOOT_ERROR_MASTER_UNIQUE_ITEM       = 13,   // Player has too many of that item already
    LOOT_ERROR_MASTER_OTHER             = 14,   // Can't assign item to that player
    LOOT_ERROR_ALREADY_PICKPOCKETED     = 15,   // Your target has already had its pockets picked
    LOOT_ERROR_NOT_WHILE_SHAPESHIFTED   = 16    // You can't do that while shapeshifted.
};

// type of Loot Item in Loot View
enum LootSlotType
{
    LOOT_SLOT_TYPE_ALLOW_LOOT   = 0,                        // player can loot the item.
    LOOT_SLOT_TYPE_ROLL_ONGOING = 1,                        // roll is ongoing. player cannot loot.
    LOOT_SLOT_TYPE_MASTER       = 2,                        // item can only be distributed by group loot master.
    LOOT_SLOT_TYPE_LOCKED       = 3,                        // item is shown in red. player cannot loot.
    LOOT_SLOT_TYPE_OWNER        = 4                         // ignore binding confirmation and etc, for single player looting
};

class Player;
class LootStore;
class ConditionMgr;

struct LootStoreItem
{
    uint32  itemid;                                         // id of the item

    uint32  reference;                                      // referenced TemplateleId
    float   chance;                                         // chance to drop for both quest and non-quest items, chance to be used for refs
    bool    needs_quest : 1;                                // quest drop (quest is required for item to drop)
    uint8   groupid     : 7;
    uint8   mincount;                                       // mincount for drop items
    uint8   maxcount    : 8;                                // max drop count for the item mincount or Ref multiplicator
    ConditionList conditions;                               // additional loot condition

    // Constructor
    // displayid is filled in IsValid() which must be called after
    LootStoreItem(uint32 _itemid, uint32 _reference, float _chance, bool _needs_quest, uint8 _groupid, int32 _mincount, uint8 _maxcount)
        : itemid(_itemid), reference(_reference), chance(_chance), needs_quest(_needs_quest),
        groupid(_groupid), mincount(_mincount), maxcount(_maxcount) {}

    bool Roll() const;                                      // Checks if the entry takes it's chance (at loot generation)
    bool IsValid(LootStore const& store, uint32 entry) const;
    // Checks correctness of values
};

struct LootItem
{
    uint32  itemid;
    uint32  randomSuffix;
    int32   randomPropertyId;
    ConditionList conditions;                               // additional loot condition
    uint8   count             : 8;
    bool    is_looted         : 1;
    bool    is_blocked        : 1;
    bool    freeforall        : 1;                          // free for all
    bool    is_underthreshold : 1;
    bool    is_counted        : 1;
    bool    needs_quest       : 1;                          // quest drop

    // Constructor, copies most fields from LootStoreItem, generates random count and random suffixes/properties
    // Should be called for non-reference LootStoreItem entries only (reference = 0)
    explicit LootItem(LootStoreItem const& li);

    // Basic checks for player/item compatibility - if false no chance to see the item in the loot
    bool AllowedForPlayer(Player const* player) const;
};

struct QuestItem
{
    uint8   index;                                          // position in quest_items;
    bool    is_looted;

    QuestItem()
        : index(0), is_looted(false) {}

    QuestItem(uint8 _index, bool _islooted = false)
        : index(_index), is_looted(_islooted) {}
};

struct Loot;
class LootTemplate;

typedef std::vector<QuestItem> QuestItemList;
typedef std::map<uint32, QuestItemList*> QuestItemMap;
typedef std::vector<LootStoreItem> LootStoreItemList;
typedef UNORDERED_MAP<uint32, LootTemplate*> LootTemplateMap;

typedef std::set<uint32> LootIdSet;

class LootStore
{
    public:
        explicit LootStore(char const* name, char const* entryName) : m_name(name), m_entryName(entryName) {}
        virtual ~LootStore()
        {
            Clear();
        }

        void Verify() const;

        void LoadAndCollectLootIds(LootIdSet& ids_set);
        void CheckLootRefs(LootIdSet* ref_set = NULL) const;// check existence reference and remove it from ref_set
        void ReportUnusedIds(LootIdSet const& ids_set) const;
        void ReportNonExistingId(uint32 id) const;

        bool HaveLootFor(uint32 loot_id) const
        {
            return m_LootTemplates.find(loot_id) != m_LootTemplates.end();
        }
        bool HaveQuestLootFor(uint32 loot_id) const;
        bool HaveQuestLootForPlayer(uint32 loot_id, Player* player) const;

        LootTemplate const* GetLootFor(uint32 loot_id) const;
        void ResetConditions();
        LootTemplate* GetLootForConditionFill(uint32 loot_id);

        char const* GetName() const
        {
            return m_name;
        }
        char const* GetEntryName() const
        {
            return m_entryName;
        }
    protected:
        void LoadLootTable();
        void Clear();
    private:
        LootTemplateMap m_LootTemplates;
        char const* m_name;
        char const* m_entryName;
};

class LootTemplate
{
        class  LootGroup;                                       // A set of loot definitions for items (refs are not allowed inside)
        typedef std::vector<LootGroup> LootGroups;

    public:
        // Adds an entry to the group (at loading stage)
        void AddEntry(LootStoreItem& item);
        // Rolls for every item in the template and adds the rolled items the the loot
        void Process(Loot& loot, LootStore const& store, uint8 GroupId = 0) const;
        void CopyConditions(ConditionList conditions);

        // True if template includes at least 1 quest drop entry
        bool HasQuestDrop(LootTemplateMap const& store, uint8 GroupId = 0) const;
        // True if template includes at least 1 quest drop for an active quest of the player
        bool HasQuestDropForPlayer(LootTemplateMap const& store, Player const* player, uint8 GroupId = 0) const;

        // Checks integrity of the template
        void Verify(LootStore const& store, uint32 Id) const;
        void CheckLootRefs(LootTemplateMap const& store, LootIdSet* ref_set) const;
        bool addConditionItem(Condition* cond);
        bool isReference(uint32 id);

    private:
        LootStoreItemList Entries;                          // not grouped only
        LootGroups        Groups;                           // groups have own (optimised) processing, grouped entries go there
};

//=====================================================

class LootValidatorRef :  public Reference<Loot, LootValidatorRef>
{
    public:
        LootValidatorRef() {}
        void targetObjectDestroyLink() override {}
        void sourceObjectDestroyLink() override {}
};

//=====================================================

class LootValidatorRefManager : public RefManager<Loot, LootValidatorRef>
{
    public:
        typedef LinkedListHead::Iterator< LootValidatorRef > iterator;

        LootValidatorRef* getFirst()
        {
            return (LootValidatorRef*)RefManager<Loot, LootValidatorRef>::getFirst();
        }
        LootValidatorRef* getLast()
        {
            return (LootValidatorRef*)RefManager<Loot, LootValidatorRef>::getLast();
        }

        iterator begin()
        {
            return iterator(getFirst());
        }
        iterator end()
        {
            return iterator(NULL);
        }
        iterator rbegin()
        {
            return iterator(getLast());
        }
        iterator rend()
        {
            return iterator(NULL);
        }
};

//=====================================================
struct LootView;

ByteBuffer& operator<<(ByteBuffer& b, LootItem const& li);
ByteBuffer& operator<<(ByteBuffer& b, LootView const& lv);

struct Loot
{
    friend ByteBuffer& operator<<(ByteBuffer& b, LootView const& lv);

        QuestItemMap const& GetPlayerQuestItems() const
        {
            return PlayerQuestItems;
        }
        QuestItemMap const& GetPlayerFFAItems() const
        {
            return PlayerFFAItems;
        }
        QuestItemMap const& GetPlayerNonQuestNonFFAConditionalItems() const
        {
            return PlayerNonQuestNonFFAConditionalItems;
        }

        std::vector<LootItem> items;
        std::vector<LootItem> quest_items;
        uint32 gold;
        uint8 unlootedCount;
        uint64 roundRobinPlayer;                            // GUID of the player having the Round-Robin ownership for the loot. If 0, round robin owner has released
        LootType loot_type;

        Loot(uint32 _gold = 0) : gold(_gold), unlootedCount(0), roundRobinPlayer(0), loot_type(LOOT_CORPSE) {}
        ~Loot()
        {
            clear();
        }

        // if loot becomes invalid this reference is used to inform the listener
        void addLootValidatorRef(LootValidatorRef* pLootValidatorRef)
        {
            i_LootValidatorRefManager.insertFirst(pLootValidatorRef);
        }

        // void clear();
        void clear()
        {
            gold = 0;
            PlayersLooting.clear();
            for (QuestItemMap::iterator itr = PlayerQuestItems.begin(); itr != PlayerQuestItems.end(); ++itr)
                delete itr->second;
            for (QuestItemMap::iterator itr = PlayerFFAItems.begin(); itr != PlayerFFAItems.end(); ++itr)
                delete itr->second;
            for (QuestItemMap::iterator itr = PlayerNonQuestNonFFAConditionalItems.begin(); itr != PlayerNonQuestNonFFAConditionalItems.end(); ++itr)
                delete itr->second;

            PlayerQuestItems.clear();
            PlayerFFAItems.clear();
            PlayerNonQuestNonFFAConditionalItems.clear();

            items.clear();
            quest_items.clear();
            gold = 0;
            unlootedCount = 0;
            i_LootValidatorRefManager.clearReferences();
            roundRobinPlayer = 0;
            loot_type = LOOT_NONE;
        }

        bool empty() const
        {
            return items.empty() && gold == 0;
        }
        bool isLooted() const
        {
            return gold == 0 && unlootedCount == 0;
        }

        void NotifyItemRemoved(uint8 lootIndex);
        void NotifyQuestItemRemoved(uint8 questIndex);
        void NotifyMoneyRemoved();
        void AddLooter(uint64 GUID)
        {
            PlayersLooting.insert(GUID);
        }
        void RemoveLooter(uint64 GUID)
        {
            PlayersLooting.erase(GUID);
        }

        void generateMoneyLoot(uint32 minAmount, uint32 maxAmount);
        void FillLoot(uint32 loot_id, LootStore const& store, Player* loot_owner, bool personal);
        void FillNotNormalLootFor(Player* player);

        // Inserts the item into the loot (called by LootTemplate processors)
        void AddItem(LootStoreItem const& item);

        LootItem* LootItemInSlot(uint32 lootslot, Player* player, QuestItem** qitem = NULL, QuestItem** ffaitem = NULL, QuestItem** conditem = NULL);
        uint32 GetMaxSlotInLootFor(Player* player) const;
        bool hasItemForAll() const;
        bool hasItemFor(Player* player) const;
        bool hasOverThresholdItem() const;

    private:
        QuestItemList* FillFFALoot(Player* player);
        QuestItemList* FillQuestLoot(Player* player);
        QuestItemList* FillNonQuestNonFFAConditionalLoot(Player* player);

        std::set<uint64> PlayersLooting;
        QuestItemMap PlayerQuestItems;
        QuestItemMap PlayerFFAItems;
        QuestItemMap PlayerNonQuestNonFFAConditionalItems;

        // All rolls are registered here. They need to know, when the loot is not valid anymore
        LootValidatorRefManager i_LootValidatorRefManager;

};

struct LootView
{
    Loot& loot;
    Player* viewer;
    PermissionTypes permission;
    LootView(Loot& _loot, Player* _viewer, PermissionTypes _permission = ALL_PERMISSION)
        : loot(_loot), viewer(_viewer), permission(_permission) {}
};

extern LootStore LootTemplates_Creature;
extern LootStore LootTemplates_Fishing;
extern LootStore LootTemplates_Gameobject;
extern LootStore LootTemplates_Item;
extern LootStore LootTemplates_Mail;
extern LootStore LootTemplates_Pickpocketing;
extern LootStore LootTemplates_Reference;
extern LootStore LootTemplates_Skinning;
extern LootStore LootTemplates_Disenchant;
extern LootStore LootTemplates_Prospecting;

void LoadLootTemplates_Creature();
void LoadLootTemplates_Fishing();
void LoadLootTemplates_Gameobject();
void LoadLootTemplates_Item();
void LoadLootTemplates_Mail();
void LoadLootTemplates_Pickpocketing();
void LoadLootTemplates_Skinning();
void LoadLootTemplates_Disenchant();
void LoadLootTemplates_Prospecting();
void LoadLootTemplates_Reference();

inline void LoadLootTables()
{
    LoadLootTemplates_Creature();
    LoadLootTemplates_Fishing();
    LoadLootTemplates_Gameobject();
    LoadLootTemplates_Item();
    LoadLootTemplates_Mail();
    LoadLootTemplates_Pickpocketing();
    LoadLootTemplates_Skinning();
    LoadLootTemplates_Disenchant();
    LoadLootTemplates_Prospecting();
    LoadLootTemplates_Mail();
    LoadLootTemplates_Reference();
}

#endif



