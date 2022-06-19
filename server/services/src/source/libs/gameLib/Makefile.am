
SUBDIRS = .

AUTOMAKE_OPTIONS = foreign

noinst_LIBRARIES = libewwGameLib.a

libewwGameLib_a_CPPFLAGS = -DCURL_STATICLIB \
					   -I$(MYSRCTOP) -I$(MYLIBHPATH)

libewwGameLib_a_SOURCES = 	attributecalculator/attribute_calculator.cpp \
				attributecalculator/battle_property_generator.cpp \
				attributecalculator/property_container.cpp \
				attributecalculator/property_generator.cpp \
				staticdata/csv_loader.cpp \
				staticdata/default_value.cpp \
				staticdata/json_value.cpp \
				staticdata/static_filemanager.cpp \
				staticdata/staticgamedata.cpp \
				staticdata/subclass_value.cpp \
				staticdata/xml_loader.cpp \
				gamedata/AsyncData.cpp \
				gamedata/AsyncDummyData.cpp \
				gamedata/AsyncRewardData.cpp \
				gamedata/BattleInfoData.cpp \
				gamedata/BattleRefData.cpp \
				gamedata/BeginnerOfferData.cpp \
				gamedata/BuildingData.cpp \
				gamedata/CampaignData.cpp \
				gamedata/CampaignLootData.cpp \
				gamedata/ChallengeData.cpp \
				gamedata/CreatePlayerData.cpp \
				gamedata/ConvoyInterceptData.cpp \
				gamedata/CSVReader.cpp \
				gamedata/DropCollectionData.cpp \
				gamedata/GameStaticData.cpp \
				gamedata/GatherData.cpp \
				gamedata/GuildData.cpp \
				gamedata/GoldCostData.cpp \
				gamedata/HeroData.cpp \
				gamedata/HeroLevelUpData.cpp \
				gamedata/IllegalName.cpp \
				gamedata/ItemData.cpp \
				gamedata/LevelMap.cpp \
				gamedata/ModData.cpp \
				gamedata/MonetizationData.cpp \
				gamedata/PlayerLevelUpData.cpp \
				gamedata/PlayerRankData.cpp \
				gamedata/VIPShopData.cpp \
				gamedata/PPShopData.cpp \
				gamedata/ShopData.cpp \
				gamedata/PVPLootData.cpp \
				gamedata/RecipeData.cpp \
				gamedata/SeasonData.cpp \
				gamedata/SkirmishData.cpp \
				gamedata/SkirmishRankData.cpp \
				gamedata/SkirmishRewardData.cpp \
				gamedata/SkirmishEloBuilder.cpp \
				gamedata/TowData.cpp \
				gamedata/TowLootData.cpp \
				gamedata/FeatureData.cpp \
				gamedata/UnitData.cpp \
				gamedata/DailyRewardData.cpp \
				gamedata/VIPData.cpp \
				gamedata/VIPPrivilegeData.cpp \
				gamedata/GlobalConfigData.cpp \
				gamedata/VIPSignUpRewardDoubleItems.cpp \
				test/calculation_log.cpp \
				eventconfig/CPPEventConfig.cpp \
				eventconfig/CPPEventData.cpp \
				eventconfig/EventBuffEventData.cpp \
				eventconfig/EventInfo.cpp
