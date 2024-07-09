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

#ifndef TRINITY_DBCSTRUCTURE_H
#define TRINITY_DBCSTRUCTURE_H

#include "Common.h"
#include "DBCEnums.h"
#include "Util.h"

// Structures using to access raw DBC data and required packing to portability
#pragma pack(push, 1)

struct AnimKitEntry
{
    uint32 ID;
    uint32 OneShotDuration;
    uint16 OneShotStopAnimKitID;
    uint16 LowDefAnimKitID;
};

struct AreaTableEntry
{
    uint32      ID;                                         // 0
    uint32      ContinentID;                                // 1
    uint32      ParentAreaID;                               // 2 if 0 then it's zone, else it's zone id of this area
    int32       AreaBit;                                    // 3
    uint32      Flags[2];                                   // 4-5,
    //uint32    SoundProviderPref;                          // 6,
    //uint32    SoundProviderPrefUnderwater;                // 7,
    //uint32    AmbienceID;                                 // 8,
    //uint32    ZoneMusic;                                  // 9,
    //char*     ZoneName;                                   // 10 - Internal name
    //uint32    IntroSound;                                 // 11
    uint32      ExplorationLevel;                           // 12
    char*       AreaName_lang;                              // 13 - In-game name
    uint32      FactionGroupMask;                           // 14
    uint32      LiquidTypeID[4];                            // 15-18
    //float     MinElevation;                               // 19
    //float     AmbientMultiplier;                          // 20
    //uint32    LightID;                                    // 21
    uint32      MountFlags;                                 // 22
    //uint32    UWIntroMusic;                               // 23
    //uint32    UWZoneMusic;                                // 24
    //uint32    UWAmbience;                                 // 25
    //uint32    WorldPvPID;                                 // 26 World_PVP_Area.dbc
    uint32      PvpCombatWorldStateID;                      // 27
    uint32      WildBattlePetLevelMin;                      // 28
    uint32      WildBattlePetLevelMax;                      // 29

    static const uint32 ExtraData = sizeof(int32);

    // Shaohao: DBC doesn't have localized strings
    // TODO: DATA figure out better localization pattern?
    const char* AreaName(LocaleConstant /*locale*/) const { return AreaName_lang; }

    // Shaohao: MOP doesn't have ContentTuningID
    int32 ContentTuningID = 0;

    // helpers
    char* GetName() const { return AreaName_lang; }

    EnumFlag<AreaFlags> GetFlags() const { return static_cast<AreaFlags>(Flags[0]); }
    EnumFlag<AreaFlags2> GetFlags2() const { return static_cast<AreaFlags2>(Flags[1]); }
    EnumFlag<AreaMountFlags> GetMountFlags() const { return static_cast<AreaMountFlags>(MountFlags); }

    bool IsSanctuary() const
    {
        return GetFlags().HasFlag(AreaFlags::NoPvP);
    }
};

struct AreaTriggerEntry
{
    DBCPosition3D Pos;
    uint32 ID;
    int16 ContinentID;
    int32 PhaseUseFlags;
    int16 PhaseID;
    int16 PhaseGroupID;
    float Radius;
    float BoxLength;
    float BoxWidth;
    float BoxHeight;
    float BoxYaw;
    int8 ShapeType;
    int16 ShapeID;
    int32 AreaTriggerActionSetID;
    int8 Flags;

    AreaTriggerShapeType GetShapeType() const { return static_cast<AreaTriggerShapeType>(ShapeType); }
};

struct ArmorLocationEntry
{
    uint32 ID;
    float Clothmodifier;
    float Leathermodifier;
    float Chainmodifier;
    float Platemodifier;
    float Modifier;
};

struct AuctionHouseEntry
{
    uint32 ID;
    uint32 FactionID;                                               // id of faction.dbc for player factions associated with city
    uint32 DepositRate;
    uint32 ConsignmentRate;
    char* Name_lang;

    const char* Name(LocaleConstant /*locale*/) const { return Name_lang; };

    char* GetName() const { return Name_lang; }
};

struct BankBagSlotPricesEntry
{
    uint32 ID;
    uint32 Cost;
};

struct BannedAddOnsEntry
{
    uint32      ID;                                         // 0
    //uint32    NameMD5[4];                                 // 1
    //uint32    VersionMD5[4];                              // 2
    //uint32    LastModified;                               // 3
    //uint32    Flags;                                      // 4
};

struct BattlemasterListEntry
{
    uint32      ID;                                         // 0
    int32       MapID[16];                                  // 1-16 mapid
    uint32      InstanceType;                               // 17 map type (3 - BG, 4 - arena)
    //uint32    GroupsAllowed;                              // 18 (0 or 1)
    LocalizedString Name;                                       // 19
    uint32      MaxGroupSize;                               // 20 maxGroupSize, used for checking if queue as group
    uint32      HolidayWorldState;                          // 21 new 3.1
    uint32      MinLevel;                                   // 22, min level (sync with PvPDifficulty.dbc content)
    uint32      MaxLevel;                                   // 23, max level (sync with PvPDifficulty.dbc content)
    //uint32    RatedPlayers;                               // 24 4.0.1
    uint32      MinPlayers;                                 // 25 - 4.0.6.13596
    uint32      MaxPlayers;                                 // 26 4.0.1
    uint32      Flags;                                      // 27 4.0.3, value 2 for Rated Battlegrounds
    //uint32    IconFileDataID;                             // 28
    //char*     GameType;                                   // 29
    //uint32    Unk1;                                       // 30

    EnumFlag<BattlemasterListFlags> GetFlags() const { return static_cast<BattlemasterListFlags>(Flags); }
};

#define MAX_OUTFIT_ITEMS 24

struct CharStartOutfitEntry
{
    //uint32 Id;                                            // 0
    uint8 Race;                                             // 1
    uint8 Class;                                            // 2
    uint8 Gender;                                           // 3
    //uint8 Unused;                                         // 4
    int32 ItemId[MAX_OUTFIT_ITEMS];                         // 5-28
    //int32 ItemDisplayId[MAX_OUTFIT_ITEMS];                // 29-52 not required at server side
    //int32 ItemInventorySlot[MAX_OUTFIT_ITEMS];            // 53-76 not required at server side
    uint32 PetDisplayId;                                    // 77 Pet Model ID for starting pet
    uint32 PetFamilyEntry;                                  // 78 Pet Family Entry for starting pet
};

enum CharSectionFlags
{
    SECTION_FLAG_PLAYER = 0x01,
    SECTION_FLAG_DEATH_KNIGHT = 0x04
};

enum CharSectionType
{
    SECTION_TYPE_SKIN = 0,
    SECTION_TYPE_FACE = 1,
    SECTION_TYPE_FACIAL_HAIR = 2,
    SECTION_TYPE_HAIR = 3,
    SECTION_TYPE_UNDERWEAR = 4
};

struct CharSectionsEntry
{
     //uint32 Id;
    uint32 Race;
    uint32 Gender;
    uint32 GenType;
    //char* TexturePath[3];
    uint32 Flags;
    uint32 Type;
    uint32 Color;
};

struct CharTitlesEntry
{
    uint32      ID;                                         // 0, title ids, for example in Quest::GetCharTitleId()
    //uint32    ConditionID;                                // 1
    LocalizedString Name;                                       // 2 m_name_lang
    LocalizedString Name1;                                      // 3 m_name1_lang
    uint32      MaskID;                                     // 4 m_mask_ID used in PLAYER_CHOSEN_TITLE and 1<<index in PLAYER__FIELD_KNOWN_TITLES
    uint32      Flags;                                      // 5
};

struct ChatChannelsEntry
{
    uint32      ID;                                         // 0
    uint32      Flags;                                      // 1
    uint32      FactionGroup;                               // 2
    LocalizedString       Name;                                       // 3
    LocalizedString       Shortcut;                                   // 4

    EnumFlag<ChatChannelFlags> GetFlags() const { return static_cast<ChatChannelFlags>(Flags); }

    // Shaohao: MOP doesn't have ChatChannelRuleset
    ChatChannelRuleset GetRuleset() const { return ChatChannelRuleset::None; }
};

struct ChrClassesEntry
{
    uint32      ID;
    uint32      DisplayPower;
    //char*     PetNameToken;
    char*       Name_lang;
    //char*     Name_female_lang;
    //char*     Name_male_lang;
    //char*     Filename;
    uint32      SpellClassSet;
    uint32      Flags;
    uint32      CinematicSequenceID;
    uint32      AttackPowerPerStrength;
    uint32      AttackPowerPerAgility;
    uint32      RangedAttackPowerPerAgility;
    uint32      DefaultSpec;
    //uint32    CreateScreenFileDataID;
    //uint32    SelectScreenFileDataID;
    //uint32    LowResScreenFileDataID;
    //uint32    IconFileDataID;

    const char* Name(LocaleConstant /*locale*/) const { return Name_lang; }

    // TODO: Shaohao: MOP doesn't have ChrClassesEntry.ArmorTypeMask
    // not sure if it is needed, or if zero is fine
    static const uint32 ArmorTypeMask = 0;
};

struct ChrRacesEntry
{
    uint32 ID;
    uint32 Flags;
    uint32 FactionID;
    //uint32 ExplorationSoundID;
    uint32 MaleDisplayID;
    uint32 FemaleDisplayID;
    //char* ClientPrefix;
    //uint32 BaseLanuage;
    uint32 CreatureType;
    uint32 ResSicknessSpellID;
    //uint32 SplashSoundID;
    //char* ClientFileString;
    uint32 CinematicSequenceID;
    uint32 Alliance;
    char* Name_lang;
    //char* Name_female_lang;
    //char* Name_male_lang;
    //uint32 FacialHairCustomization[2];
    //char* HairCustomization;
    //uint32 Race_related;
    uint32 UnalteredVisualRaceID;
    //uint32 UaMaleCreatureSoundDataID;
    //uint32 UaFemaleCreatureSoundDataID;
    //uint32 ChrComponentTextureLayoutID;
    //uint32 DefaultClassID;
    //uint32 CreateScreenFileDataID;
    //uint32 SelectScreenFileDataID;
    //uint32 MaleCustomizeOffset[3];
    //uint32 FemaleCustomizeOffset[3];
    //uint32 NeutralRaceID;
    //uint32 LowResScreenFileDataID;

    const char* Name(LocaleConstant /*locale*/) const { return Name_lang; }

    EnumFlag<ChrRacesFlag> GetFlags() const { return static_cast<ChrRacesFlag>(Flags); }
};

#define MAX_MASTERY_SPELLS 2

struct ChrSpecializationEntry
{
    uint32      ID;                                         // 0 Specialization ID
    //char*     BackgroundFile;                             // 1
    uint32      ClassID;                                    // 2
    uint32      MasterySpellID[MAX_MASTERY_SPELLS];         // 3
    uint32      OrderIndex;                                 // 4
    uint32      PetTalentType;                              // 5
    uint32      Role;                                       // 6 (0 - Tank, 1 - Healer, 2 - DPS)
    uint32      SpellIconID;                                // 7
    uint32      RaidBuffs;                                  // 8
    uint32      Flags;                                      // 9
    char*       Name_lang;                                  // 10
    //char*     Description_lang;                           // 11
    uint32      MaxBuffs;                                   // 12

    const char* Name(LocaleConstant /*locale*/) const { return Name_lang; }

    EnumFlag<ChrSpecializationFlag> GetFlags() const { return static_cast<ChrSpecializationFlag>(Flags); }
    ChrSpecializationRole GetRole() const { return static_cast<ChrSpecializationRole>(Role); }

    bool IsPetSpecialization() const
    {
        return ClassID == 0;
    }
};

struct CreatureDisplayInfoEntry
{
    uint32 ID;
    uint32 ModelID;
    uint32 SoundID;
    uint32 ExtendedDisplayInfoID;
    float CreatureModelScale;
    //uint32 CreatureModelAlpha;
    //std::array<uint32, 3> TextureVariationFileDataID;
    //char* PortraitTextureName
    //uint32 SizeClass;
    //uint32 BloodID;
    //uint32 NPCSoundID;
    //uint32 ParticleColorID;
    //uint32 CreatureGeosetData
    //uint32 ObjectEffectPackageID;
    //uint32 AnimReplacementSetID;
    uint32 Flags;
    uint32 Gender;
    uint32 StateSpellVisualKitID;
};

struct CreatureDisplayInfoExtraEntry
{
    //uint32    ID;                                         // 0
    uint32      DisplayRaceID;                              // 1
    //uint32    DisplaySexID;                               // 2
    //uint32    SkinID;                                     // 3
    //uint32    FaceID;                                     // 4
    //uint32    HairStyleID;                                // 5
    //uint32    HairColorID;                                // 6
    //uint32    FacialHairID;                               // 7
    //uint32    NPCItemDisplay[11];                         // 8-18
    //uint32    Flags;                                      // 19
    //uint32    FileDataID;                                 // 20
    //uint32    Unk;                                        // 21
};

struct CreatureFamilyEntry
{
    uint32      ID;                                         // 0
    float       MinScale;                                   // 1
    uint32      MinScaleLevel;                              // 2
    float       MaxScale;                                   // 3
    uint32      MaxScaleLevel;                              // 4
    uint32      SkillLine[2];                               // 5-6
    uint32      PetFoodMask;                                // 7
    uint32      PetTalentType;                              // 8
    //uint32    CategoryEnumID;                             // 9
    char*       Name_lang;                                  // 10
    //char*     IconFile;                                   // 11

    const char* Name(LocaleConstant /*locale*/) const { return Name_lang; }
};

struct CreatureModelDataEntry
{
    uint32      ID;                                         // 0
    uint32      Flags;                                      // 1
    LocalizedString ModelName;                              // 2
    //uint32    SizeClass;                                  // 3
    float       ModelScale;                                 // 4
    //uint32    BloodID;                                    // 5
    //uint32    FootprintTextureID;                         // 6
    //float     FootprintTextureLength;                     // 7
    //float     FootprintTextureWidth;                      // 8
    //float     FootprintParticleScale;                     // 9
    //uint32    FoleyMaterialID;                            // 10
    //uint32    FootstepShakeSize;                          // 11
    //uint32    DeathThudShakeSize;                         // 12
    //uint32    SoundID;                                    // 13
    //float     CollisionWidth;                             // 14
    float       CollisionHeight;                            // 15
    float       MountHeight;                                // 16
    //float     GeoBoxMin[3];                               // 17-19
    //float     GeoBoxMax[3];                               // 20-22
    //float     WorldEffectScale;                           // 23
    //float     AttachedEffectScale;                        // 24
    //float     MissileCollisionRadius;                     // 25
    //float     MissileCollisionPush;                       // 26
    //float     MissileCollisionRaise;                      // 27
    //float     OverrideLootEffectScale;                    // 28
    //float     OverrideNameScale;                          // 29
    //float     OverrideSelectionRadius;                    // 30
    //float     TamedPetBaseScale;                          // 31
    //uint32    CreatureGeosetDataID;                       // 32
    float       HoverHeight;                                // 33

    // Shaohao: MOP doesn't have CreatureModelData.FileDataID
    uint32 FileDataID = 0;

    EnumFlag<CreatureModelDataFlags> GetFlags() const { return static_cast<CreatureModelDataFlags>(Flags); }
};

#define MAX_CREATURE_SPELL_DATA_SLOT 4

struct CreatureSpellDataEntry
{
    uint32    ID;                                           // 0        m_ID
    uint32    spellId[MAX_CREATURE_SPELL_DATA_SLOT];        // 1-4      m_spells[4]
    //uint32    availability[MAX_CREATURE_SPELL_DATA_SLOT]; // 4-7      m_availability[4]
};

struct CreatureTypeEntry
{
    uint32 ID;
    //char* Name_lang;
    //uint32 Flags;
};

/* not used
struct CurrencyCategoryEntry
{
    uint32    ID;                                           // 0
    uint32    Unk1;                                         // 1        0 for known categories and 3 for unknown one (3.0.9)
    char*   Name[16];                                       // 2-17     name
    //                                                      // 18       string flags
};
*/

struct DifficultyEntry
{
    uint32      ID;                                         // 0
    uint32      FallbackDifficultyID;                       // 1
    uint32      InstanceType;                               // 2
    uint32      MinPlayers;                                 // 3
    uint32      MaxPlayers;                                 // 4
    int32       OldEnumValue;                               // 5
    uint32      Flags;                                      // 6
    uint32      ToggleDifficultyID;                         // 7
    //uint32    GroupSizeHealthCurveID;                     // 8
    //uint32    GroupSizeDmgCurveID;                        // 9
    //uint32    GroupSizeSpellPointsCurveID;                // 10
    char*       Name_lang;                                  // 11

    const char* Name(LocaleConstant /*locale*/) const { return Name_lang; }
};

//$id$ID<32>
//        FallbackDifficultyID<32>
//InstanceType<32>
//        MinPlayers<32>
//MaxPlayers<32>
//        OldEnumValue<32>
//Flags<32>
//        ToggleDifficultyID<32>
//GroupSizeHealthCurveID<32>
//        GroupSizeDmgCurveID<32>
//GroupSizeSpellPointsCurveID<32>
//        Name_lang

struct DungeonEncounterEntry
{
    uint32      ID;                                         // 0
    uint32      MapID;                                      // 1
    uint32      DifficultyID;                               // 2
    uint32      OrderIndex;                                 // 3
    uint32      Bit;                                        // 4
    LocalizedString Name;                                   // 5
    //uint32    CreatureDisplayID;                          // 6
    //uint32    Flags;                                      // 7
    //uint32    Unk;                                        // 8 Flags2?

    // Shaohao: MOP doesn't have DungeonEncounter.CompleteWorldStateID
    int32 CompleteWorldStateID;
};

struct DurabilityCostsEntry
{
    uint32      ID;                                         // 0
    uint32      WeaponSubClassCost[21];                     // 1-22
    uint32      ArmorSubClassCost[8];                       // 23-30
};

struct EmotesEntry
{
    uint32      ID;                                         // 0
    //char*     EmoteSlashCommand;                          // 1
    uint32      AnimID;                                     // 2 ref to animationData
    uint32      EmoteFlags;                                 // 3 bitmask, may be unit_flags
    uint32      EmoteSpecProc;                              // 4 Can be 0, 1 or 2 (determine how emote are shown)
    uint32      EmoteSpecProcParam;                         // 5 uncomfirmed, may be enum UnitStandStateType
    //uint32    EmoteSoundID;                               // 6 ref to soundEntries
    //uint32    SpellVisualKitID                            // 7
};

struct EmotesTextEntry
{
    uint32      ID;                                         // 0
    //char*     Name_lang;                                  // 1
    uint32      EmoteID;                                    // 2
    //uint32    EmoteText[16];                              // 3-18
};

struct EmotesTextSoundEntry
{
    uint32 Id;                                              // 0
    uint32 EmotesTextID;                                    // 1
    uint32 RaceID;                                          // 2
    uint32 SexID;                                           // 3, 0 male / 1 female
    uint32 SoundID;                                         // 4
};

struct FactionEntry
{
    uint32      ID;                                         // 0
    int32       ReputationIndex;                            // 1
    std::array<Trinity::RaceMask<uint32>, 4> ReputationRaceMask; // 2-5
    uint32      ReputationClassMask[4];                     // 6-9
    int32       ReputationBase[4];                          // 10-13
    uint32      ReputationFlags[4];                         // 14-17
    uint32      ParentFactionID;                            // 18
    float       ParentFactionMod[2];                        // 19-20
    uint32      ParentFactionCap[2];                        // 21-22
    char*       Name_lang;                                  // 23
    //char*     Description_lang;                           // 24
    uint32      Expansion;                                  // 25
    //uint32    Flags;                                      // 26
    uint32      FriendshipRepID;                            // 27

    const char* Name(LocaleConstant /*locale*/) const { return Name_lang; }

    // Shaohao: MOP doesn't have Faction.ReputationMax[]
    static constexpr int32 ReputationMax[4] = { 0, 0, 0, 0};

    // Shaohao: MOP doesn't have Faction.RenownCurrencyID
    static const int32 RenownCurrencyID = 0;

    // Shaohao: paragon reputation was added in Legion
    static const uint16 ParagonFactionID = 0;

    // helpers
    bool CanHaveReputation() const
    {
        return ReputationIndex >= 0;
    }
};

#define MAX_FACTION_RELATIONS 4

struct FactionTemplateEntry
{
    uint32      ID;                                         // 0
    uint32      Faction;                                    // 1
    uint32      Flags;                                      // 2
    uint32      FactionGroup;                               // 3 m_factionGroup
    uint32      FriendGroup;                                // 4 m_friendGroup
    uint32      EnemyGroup;                                 // 5 m_enemyGroup
    uint32      Enemies[MAX_FACTION_RELATIONS];             // 6
    uint32      Friend[MAX_FACTION_RELATIONS];              // 10
    //-------------------------------------------------------  end structure

    // helpers
    bool IsFriendlyTo(FactionTemplateEntry const* entry) const
    {
        if (this == entry)
            return true;
        if (entry->Faction)
        {
            for (int32 i = 0; i < MAX_FACTION_RELATIONS; ++i)
                if (Enemies[i] == entry->Faction)
                    return false;
            for (int32 i = 0; i < MAX_FACTION_RELATIONS; ++i)
                if (Friend[i] == entry->Faction)
                    return true;
        }
        return (FriendGroup & entry->FactionGroup) || (FactionGroup & entry->FriendGroup);
    }

    bool IsHostileTo(FactionTemplateEntry const* entry) const
    {
        if (this == entry)
            return false;
        if (entry->Faction)
        {
            for (int32 i = 0; i < MAX_FACTION_RELATIONS; ++i)
                if (Enemies[i] == entry->Faction)
                    return true;
            for (int32 i = 0; i < MAX_FACTION_RELATIONS; ++i)
                if (Friend[i] == entry->Faction)
                    return false;
        }
        return (EnemyGroup & entry->FactionGroup) != 0;
    }

    bool IsHostileToPlayers() const { return (EnemyGroup & FACTION_MASK_PLAYER) !=0; }

    bool IsNeutralToAll() const
    {
        for (int i = 0; i < MAX_FACTION_RELATIONS; ++i)
            if (Enemies[i] != 0)
                return false;
        return EnemyGroup == 0 && FriendGroup == 0;
    }

    bool IsContestedGuardFaction() const { return (Flags & FACTION_TEMPLATE_FLAG_CONTESTED_GUARD) != 0; }
};

struct GameObjectDisplayInfoEntry
{
    uint32          ID;                                     // 0
    uint32          FileDataID;                             // 1
    //uint32        Sound[10];                              // 2-11
    DBCPosition3D   GeoBoxMin;                              // 12-14
    DBCPosition3D   GeoBoxMax;                              // 15-17
    //uint32        ObjectEffectPackageID;                  // 18
    //float         OverrideLootEffectScale;                // 19
    //float         OverrideNameScale;                      // 20
};

struct GameTablesEntry
{
    uint32 ID;                                                      // 0
    char* Name;                                                     // 1
    uint32 NumRows;                                                 // 2
    uint32 NumColumns;                                              // 3
};

struct GemPropertiesEntry
{
    uint32      ID;                                         // 0
    uint32      EnchantId;                                  // 1
    //uint32    MaxCountInv;                                // 2
    //uint32    MaxCountItem;                               // 3
    uint32      Type;                                       // 4
    uint32      MinItemLevel;                               // 5
};

struct GlyphPropertiesEntry
{
    uint32      ID;                                         // 0
    uint32      SpellID;                                    // 1
    uint32      Type;                                       // 2
    uint32      SpellIconID;                                // 3 GlyphIconId (SpellIcon.dbc)

    // Shaohao: MOP doesn't have GlyphExclusiveCategoryID
    uint32 GlyphExclusiveCategoryID = 0;
};

// GuildColorBackground.dbc
struct GuildColorBackgroundEntry
{
    uint32      ID;
    //uint8       Red;
    //uint8       Green;
    //uint8       Blue;
};

// GuildColorBorder.dbc
struct GuildColorBorderEntry
{
    uint32      ID;
    //uint8       Red;
    //uint8       Green;
    //uint8       Blue;
};

// GuildColorEmblem.dbc
struct GuildColorEmblemEntry
{
    uint32      ID;
    //uint8       Red;
    //uint8       Green;
    //uint8       Blue;
};

// common struct for:
// ItemDamageAmmo.dbc
// ItemDamageOneHand.dbc
// ItemDamageOneHandCaster.dbc
// ItemDamageRanged.dbc
// ItemDamageThrown.dbc
// ItemDamageTwoHand.dbc
// ItemDamageTwoHandCaster.dbc
// ItemDamageWand.dbc
struct ItemDamageEntry
{
    uint32      ID;                                         // 0 item level
    float       DPS[7];                                     // 1-7 multiplier for item quality
    uint32      ItemLevel;                                  // 8 item level
};


struct ItemDamageAmmoEntry
{
    uint32 ID;
    std::array<float, 7> Quality;
    uint32 ItemLevel;
};

struct ItemDamageOneHandEntry
{
    uint32 ID;
    std::array<float, 7> Quality;
    uint32 ItemLevel;
};

struct ItemDamageOneHandCasterEntry
{
    uint32 ID;
    std::array<float, 7> Quality;
    uint32 ItemLevel;
};

struct ItemDamageTwoHandEntry
{
    uint32 ID;
    std::array<float, 7> Quality;
    uint32 ItemLevel;
};

struct ItemDamageTwoHandCasterEntry
{
    uint32 ID;
    std::array<float, 7> Quality;
    uint32 ItemLevel;
};

struct ItemArmorQualityEntry
{
    uint32      ID;                                         // 0 item level
    float       QualityMod[7];                              // 1-7 multiplier for item quality
    uint32      ItemLevel;                                  // 8 item level
};

struct ItemArmorShieldEntry
{
    uint32      ID;                                         // 0 item level
    uint32      ItemLevel;                                  // 1 item level
    float       Quality[7];                                 // 2-8 quality
};

struct ItemArmorTotalEntry
{
    uint32 ID;
    uint32 ItemLevel;
    float Cloth;
    float Leather;
    float Mail;
    float Plate;
};

struct ItemBagFamilyEntry
{
    uint32      ID;                                         // 0
    //char*     Name_lang;                                  // 1        m_name_lang
};

struct ItemDisplayInfoEntry
{
    uint32      ID;                                         // 0
    //char*     ModelName[2];                               // 1-2
    //char*     ModelTexture[2];                            // 3-4
    //uint32    GeoSetGroup[3];                             // 5-7
    //uint32    Flags;                                      // 8
    //uint32    SpellVisualID;                              // 9
    //uint32    HelmetGeosetVis[2];                         // 10-11
    //char*     Texture[9];                                 // 12-20
    //uint32    ItemVisual;                                 // 21
    //uint32    ParticleColorID;                            // 22
};

#define MAX_ITEM_SET_ITEMS 17
#define MAX_ITEM_SET_SPELLS 8

struct ItemSetEntry
{
    uint32      ID;                                         // 0
    LocalizedString Name;                                  // 1
    uint32      ItemID[MAX_ITEM_SET_ITEMS];                 // 2-18
    uint32      RequiredSkill;                              // 19
    uint32      RequiredSkillRank;                          // 20

    // Shaohao: MOP doesn't have SetFlags
    uint32 SetFlags = 0;
};

//struct ItemSetSpellEntry
//{
//    uint32      ID;                                         // 0
//    uint32      ItemSetID;                                  // 1
//    uint32      SpellID;                                    // 2
//    uint32      Threshold;                                  // 3
//    uint32      ChrSpecID;                                  // 4
//};

#define MAX_ITEM_ENCHANTMENT_EFFECTS 3

struct ItemRandomPropertiesEntry
{
    uint32    ID;                                           // 0        m_ID
    //DbcStr internalName                                   // 1        m_Name
    uint32    enchant_id[MAX_ITEM_ENCHANTMENT_EFFECTS];     // 2-4      m_Enchantment
    // 5-6      unused
    char*     nameSuffix;                                   // 7        m_name_lang
};

struct ItemRandomSuffixEntry
{
    uint32    ID;                                           // 0        m_ID
    char*     nameSuffix;                                   // 1        m_name_lang
    // 2        m_internalName
    uint32    enchant_id[5];                                // 3-7      m_enchantment
    uint32    prefix[5];                                    // 8-12     m_allocationPct
};

struct LFGDungeonsEntry
{
    uint32      ID;                                         // 0
    LocalizedString Name;                                  // 1
    uint32      MinLevel;                                   // 2
    uint32      MaxLevel;                                   // 3
    uint32      TargetLevel;                                // 4
    //uint32    TargetLevelMin;                             // 5
    //uint32    TargetLevelMax;                             // 6
    int32       MapID;                                      // 7
    uint32      DifficultyID;                               // 8

    // Shaohao: MOP only has one Flags
    uint32      Flags[1];                                   // 9

    uint32      TypeID;                                     // 10
    //uint32    Faction;                                    // 11
    //char*     TextureFilename;                            // 12
    uint32      ExpansionLevel;                             // 13
    //uint32    OrderIndex;                                 // 14
    uint32      GroupID;                                    // 15
    //char*     Description_lang;                           // 16
    //uint32    RandomID;                                   // 17
    //uint32    CountTank;                                  // 18
    //uint32    CountHealer;                                // 19
    //uint32    CountDamage;                                // 20
    //uint32    MinCountTank;                               // 21
    //uint32    MinCountHealer;                             // 22
    //uint32    MinCountDamage;                             // 23
    //uint32    ScenarioID;                                 // 24
    //uint32    SubType;                                    // 25
    //uint32    LastBossJournalEncounterID;                 // 26
    //uint32    BonusReputationAmount;                      // 27
    //uint32    MentorCharLevel;                            // 28
    //uint32    MentorItemLevel;                            // 29

    // Shaohao: MOP doesn't have ContentTuningID
    int32 ContentTuningID = 0;

    // Shaohao: MOP doesn't have FinalEncounterID
    uint16 FinalEncounterID = 0;

    // Helpers
    uint32 Entry() const { return ID + (TypeID << 24); }
};

struct LightEntry
{
    uint32          ID;                                     // 0
    uint32          MapID;                                  // 1
    DBCPosition3D   Pos;                                    // 2-4
    //float         FalloffStart;                           // 5
    //float         FalloffEnd;                             // 6
    //uint32        LightParamsID[8];                       // 7-14
};

struct LiquidTypeEntry
{
    uint32      ID;                                         // 0
    //char*     Name;                                       // 1
    //uint32    Flags;                                      // 2
    uint32      SoundBank;                                  // 3 m_soundBank
    //uint32    SoundID;                                    // 4
    uint32      SpellID;                                    // 5
    //float     MaxDarkenDepth;                             // 6
    //float     FogDarkenIntensity;                         // 7
    //float     AmbDarkenIntensity;                         // 8
    //float     DirDarkenIntensity;                         // 9
    //uint32    LightID;                                    // 10
    //float     ParticleScale;                              // 11
    //uint32    ParticleMovement;                           // 12
    //uint32    ParticleTexSlots;                           // 13
    //uint32    MaterialID;                                 // 14
    //char*     Texture[6];                                 // 15-20
    //uint32    Color[2];                                   // 21-23
    //float     Float[18];                                  // 24-41
    //uint32    Int[4];                                     // 42-45
    //uint32    Unk3[5];                                    // 46-50
};

#define MAX_LOCK_CASE 8

struct LockEntry
{
    uint32      ID;                                         // 0
    uint32      Type[MAX_LOCK_CASE];                        // 1-8
    uint32      Index[MAX_LOCK_CASE];                       // 9-16
    uint32      Skill[MAX_LOCK_CASE];                       // 17-24
    //uint32    Action[MAX_LOCK_CASE];                      // 25-32
};

struct PhaseEntry
{
    uint32      ID;                                         // 0
    //char*     Name;
    uint32      Flags;                                      // 2

    EnumFlag<PhaseEntryFlags> GetFlags() const { return static_cast<PhaseEntryFlags>(Flags); }
};

struct MapEntry
{
    uint32          ID;                                     // 0
    //char*         Directory;                              // 1
    uint32          InstanceType;                           // 2
    // TODO: DATA this might break stuff; MOP only has one Flags
    uint32          Flags[1];                               // 3
    //uint32        MapType;                                // 4
    char*           MapName_lang;                           // 6
    uint32          AreaTableID;                            // 7
    //char*         MapDescription0_lang;                   // 8 Horde
    //char*         MapDescription1_lang;                   // 9 Alliance
    uint32          LoadingScreenID;                        // 10 LoadingScreens.dbc
    //float         MinimapIconScale;                       // 11
    uint32          CorpseMapID;                            // 12 map_id of entrance map in ghost mode (continent always and in most cases = normal entrance)
    DBCPosition2D   Corpse;                                 // 13 entrance coordinates in ghost mode  (in most cases = normal entrance)
    //uint32        TimeOfDayOverride;                      // 15
    uint32          ExpansionID;                            // 16
    uint32          RaidOffset;                             // 17
    uint32          MaxPlayers;                             // 18
    int32           ParentMapID;                            // 19 related to phasing

    // Shaohao: MOP doesn't have Map.CosmeticParentMapID
    static const int32 CosmeticParentMapID = -1;

    const char* MapName(LocaleConstant /*locale*/) const { return MapName_lang; }

    // Helpers
    uint32 Expansion() const { return ExpansionID; }

    bool IsDungeon() const { return (InstanceType == MAP_INSTANCE || InstanceType == MAP_RAID) && !IsGarrison(); }
    bool IsNonRaidDungeon() const { return InstanceType == MAP_INSTANCE; }
    bool Instanceable() const { return InstanceType == MAP_INSTANCE || InstanceType == MAP_RAID || InstanceType == MAP_BATTLEGROUND || InstanceType == MAP_ARENA; }
    bool IsRaid() const { return InstanceType == MAP_RAID; }
    bool IsBattleground() const { return InstanceType == MAP_BATTLEGROUND; }
    bool IsBattleArena() const { return InstanceType == MAP_ARENA; }
    bool IsBattlegroundOrArena() const { return InstanceType == MAP_BATTLEGROUND || InstanceType == MAP_ARENA; }
    bool IsScenario() const { return InstanceType == MAP_SCENARIO; }
    bool IsWorldMap() const { return InstanceType == MAP_COMMON; }

    bool GetEntrancePos(int32 &mapid, float &x, float &y) const
    {
        if (CorpseMapID < 0)
            return false;
        mapid = CorpseMapID;
        x = Corpse.X;
        y = Corpse.Y;
        return true;
    }

    bool IsContinent() const
    {
        return ID == 0 || ID == 1 || ID == 530 || ID == 571 || ID == 870 || ID == 1116;
    }

    bool IsDynamicDifficultyMap() const { return GetFlags().HasFlag(MapFlags::DynamicDifficulty); }
    bool IsFlexLocking() const { return GetFlags().HasFlag(MapFlags::FlexibleRaidLocking); }
    bool IsGarrison() const { return GetFlags().HasFlag(MapFlags::Garrison); }
    bool IsSplitByFaction() const
    {
        return ID == 609 || // Acherus (DeathKnight Start)
            ID == 1265 ||   // Assault on the Dark Portal (WoD Intro)
            ID == 1481 ||   // Mardum (DH Start)
            ID == 2175 ||   // Exiles Reach - NPE
            ID == 2570;     // Forbidden Reach (Dracthyr/Evoker Start)
    }

    EnumFlag<MapFlags> GetFlags() const { return static_cast<MapFlags>(Flags[0]); }

    // Shaohao: MOP has no Map.Flags2
    EnumFlag<MapFlags2> GetFlags2() const { return static_cast<MapFlags2>(0); }
};

struct MapDifficultyEntry
{
    uint32      ID;                                         // 0
    uint32      MapID;                                      // 1
    uint32      DifficultyID;                               // 2 (for arenas: arena slot)
    LocalizedString Message;                                // 3 m_message_lang (text showed when transfer to map failed)
    uint32      RaidDuration;                               // 4 m_raidDuration in secs, 0 if no fixed reset time
    uint32      MaxPlayers;                                 // 5 m_maxPlayers some heroic versions have 0 when expected same amount as in normal version
    uint32      LockID;                                     // 6
    //uint32      ItemBonusTreeModID;                         // 7

    // TODO: DATA this probably breaks instance resets
    uint32 ResetInterval = 0;

    // Shaohao: MOP doesn't have Flags or ResetInterval
    bool HasResetSchedule() const { return false; }
    bool IsUsingEncounterLocks() const { return false; }
    bool IsRestoringDungeonState() const { return false; }

    uint32 GetRaidDuration() const { return RaidDuration; }
};

struct MinorTalentEntry
{
    uint32      ID;                                         // 0
    uint32      SpecID;                                     // 1
    uint32      SpellID;                                    // 2
    uint32      OrderIndex;                                 // 3
};

#define MAX_MOUNT_CAPABILITIES 24

struct MountTypeEntry
{
    uint32 Id;
    uint32 MountCapability[MAX_MOUNT_CAPABILITIES];
};

struct MovieEntry
{
    uint32      ID;                                         // 0 index
    uint32      Volume;                                     // 1
    //uint32    KeyID;                                      // 2
    //uint32    AudioFileDataID;                            // 3
    //uint32    SubtitleFileDataID;                         // 4
};

struct PowerDisplayEntry
{
    uint32      ID;                                         // 0
    uint32      ActualType;                                 // 1
    //char*     GlobalStringBaseTag;                        // 2
    //uint8     Red;                                        // 3
    //uint8     Green;                                      // 4
    //uint8     Blue;                                       // 5
};

struct PvPDifficultyEntry
{
    //uint32    ID;                                         // 0
    uint32      MapID;                                      // 1
    uint32      BracketID;                                  // 2 m_rangeIndex
    uint32      MinLevel;                                   // 3
    uint32      MaxLevel;                                   // 4

    // helpers
    BattlegroundBracketId GetBracketId() const { return BattlegroundBracketId(BracketID); }
};

struct QuestFactionRewEntry
{
  uint32        ID;                                         // 0
  int32         QuestRewFactionValue[10];                   // 1-10
};

struct RandomPropertiesPointsEntry
{
    uint32      ItemLevel;                                  // 0
    uint32      EpicPropertiesPoints[5];                    // 1-5
    uint32      RarePropertiesPoints[5];                    // 6-10
    uint32      UncommonPropertiesPoints[5];                // 11-15
};

struct ResearchBranchEntry
{
    uint32 Id;                                            // 0
    //DbcStr BranchName;                                  // 1
    //uint32 ResearchFieldId;                             // 2 research field (from ResearchField.dbc)
    uint32 FragmentCurrencyId;                            // 3
    //DbcStr Icon;                                        // 4
    uint32 KeystoneItemId;                                // 5
};

//struct ResearchFieldEntry
//{
//    uint32 Id;                                            // 0
//    DbcStr FieldName;                                     // 1
//    uint32 slot;                                          // 2
//};

struct ResearchProjectEntry
{
    uint32 Id;                                              // 0
    //DbcStr ProjectName;                                   // 1
    //DbcStr ProjectDescription;                            // 2
    uint32 Rarity;                                          // 3 0-common, 1-rare
    uint32 ResearchBranchId;                                // 4 branch id (from ResearchBranch.dbc)
    uint32 SpellId;                                         // 5
    uint32 KeystoneCount;                                   // 6
    //DbcStr ProjectIcon;                                   // 7
    uint32 RequiredFragmentCount;                           // 8
};

struct ResearchSiteEntry
{
    uint32 Id;                                              // 0
    uint32 MapId;                                           // 1
    uint32 QuestPOIBlobId;                                  // 2 blob id (from QuestPOIBlob.dbc)
    char* SiteName;                                        // 3
    //uint32 IconId;                                        // 4
};

struct SkillLineEntry
{
    uint32      ID;                                         // 0        m_ID
    uint32      CategoryID;                                 // 1        m_categoryID
    char*       DisplayName_lang;                           // 2        m_displayName_lang
    //char*     Description_lang;                           // 3        m_description_lang
    uint32      SpellIconID;                                // 4        m_spellIconID
    //char*     AlternateVerb_lang;                         // 5        m_alternateVerb_lang
    uint32      CanLink;                                    // 6        m_canLink (prof. with recipes)
    uint32      ParentSkillLineID;                          // 7
    uint32      Flags;                                      // 8

    // Shaohao: MOP doesn't have ParentTierIndex
    static const int32 ParentTierIndex = 0;

    const char* DisplayName(LocaleConstant /*locale*/) const { return DisplayName_lang; }

    EnumFlag<SkillLineFlags> GetFlags() const { return static_cast<SkillLineFlags>(Flags); }
};

struct SkillLineAbilityEntry
{
    uint32      ID;                                         // 0
    uint32      SkillLine;                                  // 1
    uint32      Spell;                                      // 2
    Trinity::RaceMask<uint32> RaceMask;                     // 3
    uint32      ClassMask;                                  // 4
    uint32      MinSkillLineRank;                           // 7
    uint32      SupercedesSpell;                            // 8
    uint32      AcquireMethod;                              // 9
    uint32      TrivialSkillLineRankHigh;                   // 10
    uint32      TrivialSkillLineRankLow;                    // 11
    uint32      NumSkillUps;                                // 12
    uint32      UniqueBit;                                  // 13
    uint32      TradeSkillCategoryID;                       // 14

    // Shaohao: MOP doesn't have SkillLineAbilityEntry.Flags
    EnumFlag<SkillLineAbilityFlags> GetFlags() const { return static_cast<SkillLineAbilityFlags>(0); }
};

struct SkillRaceClassInfoEntry
{
    //uint32    ID;                                         // 0
    uint32      SkillID;                                    // 1
    Trinity::RaceMask<uint32> RaceMask;                     // 2
    uint32      ClassMask;                                  // 3
    uint32      Flags;                                      // 4
    uint32      Availability;                               // 5
    uint32      MinLevel;                                   // 6
    uint32      SkillTierID;                                // 7
};

struct SpellRadiusEntry
{
    uint32 ID;
    float Radius;
    float RadiusPerLevel;
    float RadiusMin;
    float RadiusMax;
};

// SpellEffect.dbc
struct SpellEffectEntry
{
    uint32      ID;                                         // 0
    uint32      DifficultyID;                               // 1
    uint32      Effect;                                     // 2
    float       EffectAmplitude;                            // 3
    uint32      EffectAura;                                 // 4
    uint32      EffectAuraPeriod;                           // 5
    uint32      EffectBasePoints;                           // 6
    float       EffectBonusCoefficient;                     // 7
    float       EffectChainAmplitude;                       // 8
    uint32      EffectChainTargets;                         // 9
    uint32      EffectDieSides;                             // 10
    uint32      EffectItemType;                             // 11
    uint32      EffectMechanic;                             // 12
    uint32      EffectMiscValue[2];                         // 13-14
    float       EffectPointsPerResource;                    // 15
    uint32      EffectRadiusIndex[2];                       // 16-17
    float       EffectRealPointsPerLevel;                   // 18
    flag128     EffectSpellClassMask;                       // 19-22
    uint32      EffectTriggerSpell;                         // 23
    float       EffectPosFacing;                            // 24
    uint32      ImplicitTarget[2];                          // 25-26
    uint32      SpellID;                                    // 27
    uint32      EffectIndex;                                // 28
    uint32      EffectAttributes;                           // 29

    static const uint32 ExtraData = sizeof(float) * 6 + sizeof(int32);

    // Shaohao: MOP doesn't have these
    float BonusCoefficientFromAP = 0;
    float PvpMultiplier = 0;
    float GroupSizeBasePointsCoefficient = 0;
    float Coefficient = 0;
    float Variance = 0;
    float ResourceCoefficient = 0;
    int32 ScalingClass = 0;

    // Shaohao: MOP doesn't have EffectAttributes
    SpellEffectAttributes GetEffectAttributes() const { return SpellEffectAttributes::None; }
};

struct SoundEntriesEntry
{
    uint32    Id;                                           // 0        m_ID
    //uint32    Type;                                       // 1        m_soundType
    //DbcStr    InternalName;                               // 2        m_name
    //DbcStr    FileName[10];                               // 3-12     m_File[10]
    //uint32    Unk13[10];                                  // 13-22    m_Freq[10]
    //DbcStr    Path;                                       // 23       m_DirectoryBase
    // 24       m_volumeFloat
    // 25       m_flags
    // 26       m_minDistance
    // 27       m_distanceCutoff
    // 28       m_EAXDef
    // 29       m_soundEntriesAdvancedID, new in 3.1
    //unk                                                   // 30       4.0.0
    //unk                                                   // 31       4.0.0
    //unk                                                   // 32       4.0.0
    //unk                                                   // 33       4.0.0
};

#define MAX_SPELL_EFFECTS 32
#define MAX_EFFECT_MASK 0xFFFFFFFF

// SpellEffectScaling.dbc
struct SpellEffectScalingEntry
{
    uint32 ID;                      // 0
    float Coefficient;              // 1
    float Variance;                 // 2
    float ResourceCoefficient;      // 3
    uint32 SpellEffectID;           // 4
};

// SpellAuraOptions.dbc
struct SpellAuraOptionsEntry
{
    uint32      ID;                                         // 0
    uint32      SpellID;                                    // 1
    uint32      DifficultyID;                               // 2
    uint32      CumulativeAura;                             // 3
    uint32      ProcChance;                                 // 4
    uint32      ProcCharges;                                // 5
    uint32      ProcTypeMask;                               // 6
    uint32      ProcCategoryRecovery;                       // 7
    uint32      SpellProcsPerMinuteID;                      // 8
};

// Spell.dbc
struct SpellEntry
{
    uint32      ID;                                         // 0
    char*       Name_lang;                                  // 1
    //char*     NameSubtext_lang;                           // 2
    //char*     Description_lang;                           // 3
    //char*     AuraDescription_lang;                       // 4
    uint32      RuneCostID;                                 // 5
    uint32      SpellMissileID;                             // 6
    uint32      DescriptionVariablesID;                     // 7
    float       BonusCoefficient;                           // 8
    uint32      ScalingID;                                  // 9
    uint32      AuraOptionsID;                              // 10
    uint32      AuraRestrictionsID;                         // 11
    uint32      CastingRequirementsID;                      // 12
    uint32      CategoriesID;                               // 13
    uint32      ClassOptionsID;                             // 14
    uint32      CooldownsID;                                // 15
    uint32      EquippedItemsID;                            // 16
    uint32      InterruptsID;                               // 17
    uint32      LevelsID;                                   // 18
    uint32      ReagentsID;                                 // 19
    uint32      ShapeshiftID;                               // 20
    uint32      TargetRestrictionsID;                       // 21
    uint32      TotemsID;                                   // 22
    uint32      RequiredProjectID;                          // 23
    uint32      MiscID;                                     // 24
};

// SpellCategories.dbc
struct SpellCategoriesEntry
{
    //uint32    ID;                                         // 0
    uint32      SpellID;                                    // 1
    uint32      DifficultyID;                               // 2
    uint32      Category;                                   // 3
    uint32      DefenseType;                                // 4
    uint32      DispelType;                                 // 5
    uint32      Mechanic;                                   // 6
    uint32      PreventionType;                             // 7
    uint32      StartRecoveryCategory;                      // 8
    uint32      ChargeCategory;                             // 9
};

struct SpellCategoryEntry
{
    uint32      ID;                                         // 0
    uint32      Flags;                                      // 1
    //uint8     UsesPerWeek;                                // 2
    //uint8     Padding[3];                                 // 2
    //char*     Name_lang;                                  // 3
    int32       MaxCharges;                                 // 4
    int32       ChargeRecoveryTime;                         // 5
};

struct SpellFocusObjectEntry
{
    uint32      ID;                                         // 0
    //char*     Name_lang;                                  // 1
};

// SpellEquippedItems.dbc
struct SpellEquippedItemsEntry
{
    //uint32    ID;                                         // 0
    uint32      SpellID;                                    // 1
    uint32      DifficultyID;                               // 2
    int32       EquippedItemClass;                          // 3       m_equippedItemClass (value)
    int32       EquippedItemInvTypes;              // 4       m_equippedItemInvTypes (mask)
    int32       EquippedItemSubclass;                   // 5       m_equippedItemSubclass (mask)
};

// SpellCooldowns.dbc
struct SpellCooldownsEntry
{
    //uint32    ID;                                         // 0
    uint32      SpellID;                                    // 1
    uint32      DifficultyID;                               // 2
    uint32      CategoryRecoveryTime;                       // 3
    uint32      RecoveryTime;                               // 4
    uint32      StartRecoveryTime;                          // 5

    // Shaohao: MOP doesn't have AuraSpellID
    int32 AuraSpellID = 0;
};

// SpellInterrupts.dbc
struct SpellInterruptsEntry
{
    //uint32    ID;                                         // 0
    uint32      SpellID;                                    // 1
    uint32      DifficultyID;                               // 2
    uint32      AuraInterruptFlags[2];                      // 3-4
    uint32      ChannelInterruptFlags[2];                   // 5-6
    uint32      InterruptFlags;                             // 7
};

// SpellLevels.dbc
struct SpellLevelsEntry
{
    //uint32    ID;                                         // 0
    uint32      SpellID;                                    // 1
    uint32      DifficultyID;                               // 2
    uint32      BaseLevel;                                  // 3
    uint32      MaxLevel;                                   // 4
    uint32      SpellLevel;                                 // 5
};

struct SpellRuneCostEntry
{
    uint32  ID;                                             // 0
    uint32  RuneCost[4];                                    // 1-3 (0=blood, 1=frost, 2=unholy)
    uint32  runePowerGain;                                  // 5

    bool NoRuneCost() const { return RuneCost[0] == 0 && RuneCost[1] == 0 && RuneCost[2] == 0 && RuneCost[3] == 0; }
    bool NoRunicPowerGain() const { return runePowerGain == 0; }
};

#define MAX_SHAPESHIFT_SPELLS 8

struct SpellShapeshiftFormEntry
{
    uint32      ID;                                         // 0
    //uint32    BonusActionBar;                             // 1
    //char*     Name_lang;                                  // 2
    uint32      Flags;                                      // 3
    int32       CreatureType;                               // 4 <=0 humanoid, other normal creature types
    //uint32    AttackIconID;                               // 5 unused, related to next field
    uint32      CombatRoundTime;                            // 6
    uint32      CreatureDisplayID[4];                       // 7-10 (0 - Alliance, 1 - Horde)
    uint32      PresetSpellID[MAX_SHAPESHIFT_SPELLS];       // 11-18 spells which appear in the bar after shapeshifting
    uint32      MountTypeID;                                // 19
    //uint32    ExitSoundEntriesID;                         // 20

    EnumFlag<SpellShapeshiftFormFlags> GetFlags() const { return static_cast<SpellShapeshiftFormFlags>(Flags); }
};

// SpellShapeshift.dbc
struct SpellShapeshiftEntry
{
    uint32      ID;                                         // 0
    uint32      ShapeshiftExclude[2];                       // 1-2
    uint32      ShapeshiftMask[2];                          // 3-4
    //uint32    StanceBarOrder;                             // 5
};

// SpellTargetRestrictions.dbc
struct SpellTargetRestrictionsEntry
{
    uint32      ID;                                         // 0
    uint32      SpellID;                                    // 1
    uint32      DifficultyID;                               // 2
    float       ConeDegrees;                                // 3
    float       Width;                                      // 4
    uint32      MaxTargets;                                 // 5
    uint32      MaxTargetLevel;                             // 6
    uint32      TargetCreatureType;                         // 7
    uint32      Targets;                                    // 8
};

// SpellScaling.dbc
struct SpellScalingEntry
{
    uint32      ID;                                         // 0
    int32       CastTimeMin;                                // 1
    int32       CastTimeMax;                                // 2
    uint32      CastTimeMaxLevel;                           // 3
    int32       ScalingClass;                               // 4
    float       NerfFactor;                                 // 5
    uint32      NerfMaxLevel;                               // 6
    uint32      MaxScalingLevel;                            // 7
    uint32      ScalesFromItemLevel;                        // 8

    // TODO: DATA Shaohao: MinScalingLevel doesn't exist; support NerfFactor and NerfMaxLevel
    uint32 MinScalingLevel = 0;
};

#define MAX_ITEM_ENCHANTMENT_EFFECTS 3

struct SpellItemEnchantmentEntry
{
    uint32      ID;                                             // 0
    uint32      Charges;                                        // 1
    uint32      Effect[MAX_ITEM_ENCHANTMENT_EFFECTS];           // 2-4
    uint32      EffectPointsMin[MAX_ITEM_ENCHANTMENT_EFFECTS];  // 5-7
    uint32      EffectArg[MAX_ITEM_ENCHANTMENT_EFFECTS];        // 8-10
    LocalizedString Name;                                       // 11
    uint32      ItemVisual;                                     // 12
    uint32      Flags;                                          // 13
    uint32      SRCItemID;                                      // 14
    uint32      ConditionID;                                    // 15
    uint32      RequiredSkillID;                                // 16
    uint32      RequiredSkillRank;                              // 17
    uint32      MinLevel;                                       // 18
    uint32      MaxLevel;                                       // 19
    uint32      ItemLevel;                                      // 20
    int32       ScalingClass;                                   // 21
    int32       ScalingClassRestricted;                         // 22
    float       EffectScalingPoints[MAX_ITEM_ENCHANTMENT_EFFECTS];//23-25

    // Shaohao: MOP doesn't have SpellItemEnchantment.TransmogUseConditionID or Duration
    uint32 TransmogUseConditionID = 0;
    int32 Duration = 0;

    EnumFlag<SpellItemEnchantmentFlags> GetFlags() const { return static_cast<SpellItemEnchantmentFlags>(Flags); }
};

struct StableSlotPricesEntry
{
    uint32 Slot;
    uint32 Price;
};

struct SummonPropertiesEntry
{
    uint32      ID;                                             // 0
    uint32      Control;                                        // 1, 0 - can't be controlled?, 1 - something guardian?, 2 - pet?, 3 - something controllable?, 4 - taxi/mount?
    uint32      Faction;                                        // 2, 14 rows > 0
    uint32      Title;                                          // 3, see enum
    int32       Slot;                                           // 4, 0-6
    uint32      Flags;                                          // 5

    EnumFlag<SummonPropertiesFlags> GetFlags() const { return static_cast<SummonPropertiesFlags>(Flags); }
};

#define MAX_TALENT_TIERS 7
#define MAX_TALENT_COLUMNS 3

struct TalentEntry
{
    uint32      ID;                                             // 0
    uint32      SpecID;                                         // 1 0 - any specialization
    uint32      TierID;                                         // 2 0-6
    uint32      ColumnIndex;                                    // 3 0-2
    uint32      SpellID;                                        // 4
    uint32      Flags;                                          // 5 All 0
    uint32      CategoryMask[2];                                // 6 All 0
    uint32      ClassID;                                        // 7
    uint32      OverridesSpellID;                               // 8 spellid that is replaced by talent
    //char*     Description_lang
};

struct TaxiNodesEntry
{
    uint32 ID;
    uint32 ContinentID;
    DBCPosition3D Pos;
    char * Name_lang;
    std::array<int32, 2> MountCreatureID;
    int32 ConditionID;
    int32 Flags;
    DBCPosition2D MapOffset;

//    int32 UiTextureKitID;
//    float Facing;

    // Shaohao: DBC doesn't have localized strings
    // TODO: DATA figure out better localization pattern?
    const char* Name(LocaleConstant /*locale*/) const { return Name_lang; }

    EnumFlag<TaxiNodeFlags> GetFlags() const { return static_cast<TaxiNodeFlags>(Flags); }

    bool IsPartOfTaxiNetwork() const
    {
        return GetFlags().HasFlag(TaxiNodeFlags::ShowOnAllianceMap | TaxiNodeFlags::ShowOnHordeMap)
               // manually whitelisted nodes
               || ID == 1985   // [Hidden] Argus Ground Points Hub (Ground TP out to here, TP to Vindicaar from here)
               || ID == 1986   // [Hidden] Argus Vindicaar Ground Hub (Vindicaar TP out to here, TP to ground from here)
               || ID == 1987   // [Hidden] Argus Vindicaar No Load Hub (Vindicaar No Load transition goes through here)
               || ID == 2627   // [Hidden] 9.0 Bastion Ground Points Hub (Ground TP out to here, TP to Sanctum from here)
               || ID == 2628   // [Hidden] 9.0 Bastion Ground Hub (Sanctum TP out to here, TP to ground from here)
               || ID == 2732   // [HIDDEN] 9.2 Resonant Peaks - Teleport Network - Hidden Hub (Connects all Nodes to each other without unique paths)
               || ID == 2835   // [Hidden] 10.0 Travel Network - Destination Input
               || ID == 2843   // [Hidden] 10.0 Travel Network - Destination Output
                ;
    }
};

struct TaxiPathEntry
{
    uint32 ID;
    uint32 FromTaxiNode;
    uint32 ToTaxiNode;
    uint32 Cost;
};

struct TaxiPathNodeEntry
{
    uint32 ID;
    uint32 PathID;
    uint32 NodeIndex;
    uint32 ContinentID;
    DBCPosition3D Loc;
    int32 Flags;
    uint32 Delay;
    int32 ArrivalEventID;
    int32 DepartureEventID;
};

#define MAX_VEHICLE_SEATS 8

struct VehicleEntry
{
    uint32      ID;                                         // 0
    uint32      Flags;                                      // 1
    uint32      FlagsB;                                     // 2
    float       TurnSpeed;                                  // 3
    float       PitchSpeed;                                 // 4
    float       PitchMin;                                   // 5
    float       PitchMax;                                   // 6
    uint32      SeatID[MAX_VEHICLE_SEATS];                  // 7-14
    float       MouseLookOffsetPitch;                       // 15
    float       CameraFadeDistScalarMin;                    // 16
    float       CameraFadeDistScalarMax;                    // 17
    float       CameraPitchOffset;                          // 18
    float       FacingLimitRight;                           // 19
    float       FacingLimitLeft;                            // 20
    float       MsslTrgtTurnLingering;                      // 21
    float       MsslTrgtPitchLingering;                     // 22
    float       MsslTrgtMouseLingering;                     // 23
    float       MsslTrgtEndOpacity;                         // 24
    float       MsslTrgtArcSpeed;                           // 25
    float       MsslTrgtArcRepeat;                          // 26
    float       MsslTrgtArcWidth;                           // 27
    float       MsslTrgtImpactRadius[2];                    // 28-29
    //char*     MsslTrgtArcTexture;                         // 30
    //char*     MsslTrgtImpactTexture;                      // 31
    //char*     MsslTrgtImpactModel;                        // 32-33
    float       CameraYawOffset;                            // 34
    uint32      UILocomotionType;                           // 35
    float       MsslTrgtImpactTexRadius;                    // 36
    uint32      VehicleUIIndicatorID;                       // 37
    uint32      PowerDisplayID[3];                          // 38-40
};

struct VehicleSeatEntry
{
    uint32          ID;                                     // 0
    uint32          Flags;                                  // 1
    int32           AttachmentID;                           // 2
    DBCPosition3D   AttachmentOffset;                       // 3-5
    float           EnterPreDelay;                          // 6
    float           EnterSpeed;                             // 7
    float           EnterGravity;                           // 8
    float           EnterMinDuration;                       // 9
    float           EnterMaxDuration;                       // 10
    float           EnterMinArcHeight;                      // 11
    float           EnterMaxArcHeight;                      // 12
    int32           EnterAnimStart;                         // 13
    int32           EnterAnimLoop;                          // 14
    int32           RideAnimStart;                          // 15
    int32           RideAnimLoop;                           // 16
    int32           RideUpperAnimStart;                     // 17
    int32           RideUpperAnimLoop;                      // 18
    float           ExitPreDelay;                           // 19
    float           ExitSpeed;                              // 20
    float           ExitGravity;                            // 21
    float           ExitMinDuration;                        // 22
    float           ExitMaxDuration;                        // 23
    float           ExitMinArcHeight;                       // 24
    float           ExitMaxArcHeight;                       // 25
    int32           ExitAnimStart;                          // 26
    int32           ExitAnimLoop;                           // 27
    int32           ExitAnimEnd;                            // 28
    float           PassengerYaw;                           // 29
    float           PassengerPitch;                         // 30
    float           PassengerRoll;                          // 31
    int32           PassengerAttachmentID;                  // 32
    int32           VehicleEnterAnim;                       // 33
    int32           VehicleExitAnim;                        // 34
    int32           VehicleRideAnimLoop;                    // 35
    int32           VehicleEnterAnimBone;                   // 36
    int32           VehicleExitAnimBone;                    // 37
    int32           VehicleRideAnimLoopBone;                // 38
    float           VehicleEnterAnimDelay;                  // 39
    float           VehicleExitAnimDelay;                   // 40
    uint32          VehicleAbilityDisplay;                  // 41
    uint32          EnterUISoundID;                         // 42
    uint32          ExitUISoundID;                          // 43
    uint32          FlagsB;                                 // 44
    float           CameraEnteringDelay;                    // 45
    float           CameraEnteringDuration;                 // 46
    float           CameraExitingDelay;                     // 47
    float           CameraExitingDuration;                  // 48
    DBCPosition3D   CameraOffset;                           // 49-51
    float           CameraPosChaseRate;                     // 52
    float           CameraFacingChaseRate;                  // 53
    float           CameraEnteringZoom;                     // 54
    float           CameraSeatZoomMin;                      // 55
    float           CameraSeatZoomMax;                      // 56
    uint32          EnterAnimKitID;                         // 57
    uint32          RideAnimKitID;                          // 58
    uint32          ExitAnimKitID;                          // 59
    uint32          VehicleEnterAnimKitID;                  // 60
    uint32          VehicleRideAnimKitID;                   // 61
    uint32          VehicleExitAnimKitID;                   // 62
    uint32          CameraModeID;                           // 63
    uint32          FlagsC;                                 // 64
    uint32          UISkinFileDataID;                       // 65

    inline bool HasFlag(VehicleSeatFlags flag) const { return !!(Flags & flag); }
    inline bool HasFlag(VehicleSeatFlagsB flag) const { return !!(FlagsB & flag); }

    inline bool CanEnterOrExit() const { return HasFlag(VehicleSeatFlags(VEHICLE_SEAT_FLAG_CAN_ENTER_OR_EXIT | VEHICLE_SEAT_FLAG_CAN_CONTROL | VEHICLE_SEAT_FLAG_SHOULD_USE_VEH_SEAT_EXIT_ANIM_ON_VOLUNTARY_EXIT)); }
    inline bool CanSwitchFromSeat() const { return HasFlag(VEHICLE_SEAT_FLAG_CAN_SWITCH); }
    inline bool IsUsableByOverride() const {
        return HasFlag(VehicleSeatFlags(VEHICLE_SEAT_FLAG_UNCONTROLLED | VEHICLE_SEAT_FLAG_UNK18))
            || HasFlag(VehicleSeatFlagsB(VEHICLE_SEAT_FLAG_B_USABLE_FORCED | VEHICLE_SEAT_FLAG_B_USABLE_FORCED_2 |
                VEHICLE_SEAT_FLAG_B_USABLE_FORCED_3 | VEHICLE_SEAT_FLAG_B_USABLE_FORCED_4));
    }
    inline bool IsEjectable() const { return HasFlag(VEHICLE_SEAT_FLAG_B_EJECTABLE); }
};

struct WMOAreaTableEntry
{
    uint32      ID;                                         // 0 index
    int32       WmoID;                                      // 1 used in root WMO
    int32       NameSetID;                                  // 2 used in adt file
    int32       WmoGroupID;                                 // 3 used in group WMO
    //uint32    SoundProviderPref;                          // 4
    //uint32    SoundProviderPrefUnderwater;                // 5
    //uint32    AmbienceID;                                 // 6
    //uint32    ZoneMusic;                                  // 7
    //uint32    IntroSound;                                 // 8
    uint32      Flags;                                      // 9 used for indoor/outdoor determination
    uint32      AreaTableID;                                // 10 link to AreaTableEntry.ID
    //char*     AreaName_lang;                              // 11       m_AreaName_lang
    //uint32    UWIntroSound;                               // 12
    //uint32    UWZoneMusic;                                // 13
    //uint32    UWAmbience;                                 // 14
};

struct WorldMapAreaEntry
{
    //uint32    ID;                                         // 0
    uint32      MapID;                                      // 1
    uint32      AreaID;                                     // 2 index (continent 0 areas ignored)
    //char*     AreaName                                    // 3
    float       LocLeft;                                    // 4
    float       LocRight;                                   // 5
    float       LocTop;                                     // 6
    float       LocBottom;                                  // 7
    int32       DisplayMapID;                               // 8 -1 (map_id have correct map) other: virtual map where zone show (map_id - where zone in fact internally)
    //int32     DefaultDungeonFloor;                        // 9 pointer to DungeonMap.dbc (owerride loc coordinates)
    //uint32    ParentWorldMapID;                           // 10
    //uint32    Flags;                                      // 11
    //uint32    LevelRangeMin;                              // 12 Minimum recommended level displayed on world map
    //uint32    LevelRangeMax;                              // 13 Maximum recommended level displayed on world map
};

struct WorldMapTransformsEntry
{
    //uint32 ID;                                            // 0
    uint32 MapID;                                           // 1
    DBCPosition3D RegionMin;                                // 2
    DBCPosition3D RegionMax;                                // 3
    uint32 NewMapID;                                        // 4
    DBCPosition2D RegionOffset;                             // 5
    //uint32 NewDungeonMapID;                               // 6
    //uint32 Flags;                                         // 7
    //uint32 NewAreaID;                                     // 8
    float RegionScale;                                      // 9
};

/*
struct WorldStateSounds
{
    uint32    ID;                                           // 0        Worldstate
    uint32    unk;                                          // 1
    uint32    areaTable;                                    // 2
    uint32    WMOAreaTable;                                 // 3
    uint32    zoneIntroMusicTable;                          // 4
    uint32    zoneIntroMusic;                               // 5
    uint32    zoneMusic;                                    // 6
    uint32    soundAmbience;                                // 7
    uint32    soundProviderPreferences;                     // 8
};
*/

/*
struct WorldStateUI
{
    uint32    ID;                                           // 0
    uint32    map_id;                                       // 1        Can be -1 to show up everywhere.
    uint32    zone;                                         // 2        Can be zero for "everywhere".
    uint32    phaseMask;                                    // 3        Phase this WorldState is avaliable in
    uint32    icon;                                         // 4        The icon that is used in the interface.
    char*     textureFilename;                              // 5
    char*     text;                                         // 6-21     The worldstate text
    char*     description;                                  // 22-38    Text shown when hovering mouse on icon
    uint32    worldstateID;                                 // 39       This is the actual ID used
    uint32    type;                                         // 40       0 = unknown, 1 = unknown, 2 = not shown in ui, 3 = wintergrasp
    uint32    unk1;                                         // 41
    uint32    unk2;                                         // 43
    uint32    unk3;                                         // 44-58
    uint32    unk4;                                         // 59-61    Used for some progress bars.
    uint32    unk7;                                         // 62       Unused in 3.3.5a
};
*/

#pragma pack(pop)

typedef std::map<uint32, uint32> TalentSpellPosMap;
#endif
