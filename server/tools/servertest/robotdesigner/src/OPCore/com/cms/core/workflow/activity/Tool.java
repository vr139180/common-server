package com.cms.core.workflow.activity;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.HashMap;

import com.cms.core.workflow.*;

public class Tool implements Implementation
{
	private String name;

	private Map	params =new HashMap();
	
	public Tool( String n)
	{
		name =n;
	}

	public String toString()
	{
		return name==null?"no action":name;
	}
	
	/**
	 * @return Returns the params.
	 */
	public Map getParams()
	{
		return params;
	}
	
	/**
	 * @return Returns the name.
	 */
	public String getName()
	{
		return name;
	}
	/**
	 * @param name The name to set.
	 */
	public void setName( String name)
	{
		this.name = name;
	}
	
	private static Map s_allactions =new HashMap();
	private static List<String> s_lists =new ArrayList<String>();
	
	public static List<String> getAllActions()
	{
		return s_lists;
	}
	
	public static Map getParamsOfAction( String name)
	{
		Map ret =(Map)s_allactions.get( name);
		return ret;
	}

	static
	{
		Map pms =null;
		
		//login
		s_lists.add( "login");
		pms =new HashMap();
		s_allactions.put( "login", pms);
		
		//logout
		s_lists.add( "logout");
		pms =new HashMap();
		s_allactions.put( "logout", pms);
		
		//selchr
		s_lists.add( "selchr");
		pms =new HashMap();
		s_allactions.put( "selchr", pms);
		
		//createchr
		s_lists.add( "createchr");
		pms =new HashMap();
		s_allactions.put( "createchr", pms);
		pms.put( "faction", "1");
		
		//idle
				s_lists.add( "idle");
				pms =new HashMap();
				s_allactions.put( "idle", pms);
				pms.put( "step", "500");
				
		//cheat
				s_lists.add( "cheat");
				pms =new HashMap();
				s_allactions.put( "cheat", pms);
				pms.put( "cheat", "add all 10000");
				
		//buyhero
				s_lists.add( "buyhero");
				pms =new HashMap();
				s_allactions.put( "buyhero", pms);
				pms.put( "heroid", "101011");
				
		//trainhero
				//s_lists.add( "trainhero");
				//pms =new HashMap();
				//s_allactions.put( "trainhero", pms);
				//pms.put( "heroid", "10000");
				//pms.put( "pricetype", "0");
				
				//random train a hero
				//s_lists.add( "randomtrainhero" );
				//pms = new HashMap();
				//s_allactions.put( "randomtrainhero", pms );
	
		//speeduphero
				//s_lists.add( "speeduphero");
				//pms =new HashMap();
				//s_allactions.put( "speeduphero", pms);
				//pms.put( "heroid", "10000");
				
		//finishhero
				//s_lists.add( "finishhero");
				//pms =new HashMap();
				//s_allactions.put( "finishhero", pms);
				
		//upgradeherorank
				s_lists.add( "upgradeherorank");
				pms = new HashMap();
				s_allactions.put("upgradeherorank", pms);
				pms.put( "heroid", "101011");
				
		//upgradeheroability
				s_lists.add( "upgradeheroability");
				pms = new HashMap();
				s_allactions.put("upgradeheroability", pms);
				pms.put( "heroid", "101011");
				
		//usetrainingbook
				s_lists.add( "usetrainingbook");
				pms = new HashMap();
				s_allactions.put("usetrainingbook", pms);
				pms.put( "heroid", "101011");
				
		//buildlist
				s_lists.add( "buildlist");
				pms =new HashMap();
				s_allactions.put( "buildlist", pms);
	
		//buildunlock
				//s_lists.add( "buildunlock");
				//pms =new HashMap();
				//s_allactions.put( "buildunlock", pms);
				//pms.put( "hq", "1");
				//pms.put( "zone", "1");

		//buildupgrade
				s_lists.add( "buildupgrade");
				pms =new HashMap();
				s_allactions.put( "buildupgrade", pms);
				pms.put( "hq", "1");
				pms.put( "zone", "1");
	
		//buildfinish
				s_lists.add( "buildfinish");
				pms =new HashMap();
				s_allactions.put( "buildfinish", pms);
				pms.put( "zone", "1");

		//buildspeedup
				s_lists.add( "buildspeedup");
				pms =new HashMap();
				s_allactions.put( "buildspeedup", pms);
	
		//gatherlist
				//s_lists.add( "gatherlist");
				//pms =new HashMap();
				//s_allactions.put( "gatherlist", pms);
	
		//gatherstart
				//s_lists.add( "gatherstart");
				//pms =new HashMap();
				//s_allactions.put( "gatherstart", pms);
				//pms.put( "gatherid", "1");
				//pms.put( "dur", "1");
	
		//gatherfinish
				//s_lists.add( "gatherfinish");
				//pms =new HashMap();
				//s_allactions.put( "gatherfinish", pms);
				//pms.put( "gatherid", "1");
	
		//gatherpeedup
				//s_lists.add( "gatherpeedup");
				//pms =new HashMap();
				//s_allactions.put( "gatherpeedup", pms);
				//pms.put( "gatherid", "1");
		
		//playerextra
				s_lists.add( "playerextra");
				pms = new HashMap();
				s_allactions.put( "playerextra", pms);
			
		//gather
				s_lists.add( "gather");
				pms = new HashMap();
				s_allactions.put( "gather", pms);
				pms.put( "area", "0");
				pms.put( "city", "0");
	
				
		//craft
				s_lists.add( "craft");
				pms = new HashMap();
				s_allactions.put( "craft", pms);
				pms.put( "modid", 1);
				
		//upgrademod
				s_lists.add( "upgrademod");
				pms = new HashMap();
				s_allactions.put( "upgrademod", pms);
				pms.put( "modid", 1);
				
		//craftlist
				//s_lists.add( "craftlist");
				//pms =new HashMap();
				//s_allactions.put( "craftlist", pms);

		//craft
				//s_lists.add( "craft");
				//pms =new HashMap();
				//s_allactions.put( "craft", pms);
				//pms.put( "modid", "1");
				//pms.put( "modlevel", "1");

		//craftfinish
				//s_lists.add( "craftfinish");
				//pms =new HashMap();
				//s_allactions.put( "craftfinish", pms);
				//pms.put( "slotid", "1");

		//craftspeedup
				//s_lists.add( "craftspeedup");
				//pms =new HashMap();
				//s_allactions.put( "craftspeedup", pms);
				//pms.put( "slotid", "1");
				
				
		//modchange
				s_lists.add( "modchange");
				pms =new HashMap();
				s_allactions.put( "modchange", pms);
				pms.put( "originalunit", "1");
				pms.put( "targetunit", "1");
				pms.put( "originalidx", "1");
				pms.put( "targetidx", "1");
				pms.put( "modid", "1");
				pms.put( "modtier", "1");

		//modlist
				s_lists.add( "modlist");
				pms =new HashMap();
				s_allactions.put( "modlist", pms);

		//consumablelist
				s_lists.add( "consumablelist");
				pms =new HashMap();
				s_allactions.put( "consumablelist", pms);

		//recipelist
				s_lists.add( "recipelist");
				pms =new HashMap();
				s_allactions.put( "recipelist", pms);

		//buyenergy
				s_lists.add( "buyenergy");
				pms =new HashMap();
				s_allactions.put( "buyenergy", pms);

		//buybattlepoint
				s_lists.add( "buybattlepoint");
				pms =new HashMap();
				s_allactions.put( "buybattlepoint", pms);

		//buypvpbonus
				s_lists.add( "buypvpbonus");
				pms =new HashMap();
				s_allactions.put( "buypvpbonus", pms);

		//monetization
				s_lists.add( "monetization");
				pms =new HashMap();
				s_allactions.put( "monetization", pms);

		//campaign
				s_lists.add( "campaign");
				pms =new HashMap();
				s_allactions.put( "campaign", pms);

		//maillist
				s_lists.add( "maillist");
				pms =new HashMap();
				s_allactions.put( "maillist", pms);

		//mailread
				s_lists.add( "mailread");
				pms =new HashMap();
				s_allactions.put( "mailread", pms);

		//maildel
				s_lists.add( "maildel");
				pms =new HashMap();
				s_allactions.put( "maildel", pms);

		//questlist
				s_lists.add( "questlist");
				pms =new HashMap();
				s_allactions.put( "questlist", pms);

		//questaccept
				s_lists.add( "questaccept");
				pms =new HashMap();
				s_allactions.put( "questaccept", pms);
				pms.put( "id", "1");

		//questfinish
				s_lists.add( "questfinish");
				pms =new HashMap();
				s_allactions.put( "questfinish", pms);
				pms.put( "id", "1");

		//questfinishtut
				s_lists.add( "questfinishtut");
				pms =new HashMap();
				s_allactions.put( "questfinishtut", pms);
				pms.put( "id", "1");
				
		// tutorial quest 
				s_lists.add("tutorialquest");
				pms = new HashMap();
				s_allactions.put( "tutorialquest", pms);
				pms.put( "faction", "1");
				
		//friendlist
				s_lists.add( "friendlist");
				pms =new HashMap();
				s_allactions.put( "friendlist", pms);
				pms.put( "type", "1");

		//friendadd
				s_lists.add( "friendadd");
				pms =new HashMap();
				s_allactions.put( "friendadd", pms);

		//friendagree
				s_lists.add( "friendagree");
				pms =new HashMap();
				s_allactions.put( "friendagree", pms);

		//frienddel
				s_lists.add( "frienddel");
				pms =new HashMap();
				s_allactions.put( "frienddel", pms);

		//pvecombat
				s_lists.add( "pvecombat");
				pms =new HashMap();
				s_allactions.put( "pvecombat", pms);
				pms.put( "battle_dataid", "1");
				
		//pvpteamset
				s_lists.add("pvpteamset");
				pms = new HashMap();
				s_allactions.put("pvpteamset", pms);
				
		//pvpcombat
				s_lists.add( "pvpcombat");
				pms =new HashMap();
				s_allactions.put( "pvpcombat", pms);
				pms.put( "pvp_type", "0");
				pms.put( "combattime", "120000");
				
		// levelup
				s_lists.add( "levelup");
				pms =new HashMap();
				s_allactions.put( "levelup", pms);
				
		// levelupreward
				s_lists.add( "levelupreward");
				pms = new HashMap();
				s_allactions.put( "levelupreward", pms);
				pms.put( "level", "1");
				
		// dailyreward
				s_lists.add( "dailyreward");
				pms =new HashMap();
				s_allactions.put( "dailyreward", pms);
				
		// loginreward
				s_lists.add( "loginreward");
				pms =new HashMap();
				s_allactions.put( "loginreward", pms);
				
		// signupreward
				s_lists.add( "signupreward");
				pms =new HashMap();
				s_allactions.put( "signupreward", pms);
				
		// Leaderboard
				s_lists.add("LBInfo");
				pms = new HashMap();
				s_allactions.put( "LBInfo", pms );
				pms.put( "seasonid", "0");
				
		// Guild List
				s_lists.add("guildlist");
				pms = new HashMap();
				s_allactions.put( "guildlist", pms );
				
		// Guild Info
				s_lists.add("guildinfo");
				pms = new HashMap();
				s_allactions.put( "guildinfo", pms );
				pms.put( "querytype", "0");
				
		//
		// Guild Create
				s_lists.add("guildcreate");
				pms = new HashMap();
				s_allactions.put( "guildcreate", pms );
				
		// Guild Invite List
				s_lists.add("guildinvitelist");
				pms = new HashMap();
				s_allactions.put( "guildinvitelist", pms );
	
		// Guild Accept
				s_lists.add("guildaccept");
				pms = new HashMap();
				s_allactions.put( "guildaccept", pms );
				
		// Guild Apply Join
				s_lists.add("guildapplyjoin");
				pms = new HashMap();
				s_allactions.put( "guildapplyjoin", pms );
				pms.put("guildid", "1");
				
		// Guild Invite Join
				s_lists.add("guildinvitejoin");
				pms = new HashMap();
				s_allactions.put("guildinvitejoin", pms);
				pms.put("playername", "1_12");
				
		// Guild Confirm Invite
				s_lists.add("guildconfirm");
				pms = new HashMap();
				s_allactions.put("guildconfirm", pms);
				pms.put("type", "0");
				
		// Guid Kick Member
				s_lists.add("guildkickmember");
				pms = new HashMap();
				s_allactions.put("guildkickmember", pms);
				
		// Update Member
				s_lists.add("guildupdatemember");
				pms = new HashMap();
				s_allactions.put("guildupdatemember", pms);
				
		// Update Desc
				s_lists.add("guildupdatedesc");
				pms = new HashMap();
				s_allactions.put("guildupdatedesc", pms);
		
		// Change Chairman
				s_lists.add("guildchairmanchg");
				pms = new HashMap();
				s_allactions.put("guildchairmanchg", pms);
				
		// Guild Quit
				s_lists.add("guildquit");
				pms = new HashMap();
				s_allactions.put("guildquit", pms);
				
		// VIPShop Info
				s_lists.add("vipshopinfo");
				pms = new HashMap();
				s_allactions.put("vipshopinfo", pms);
				
		// VIPShop Buy
				s_lists.add("vipshopbuy");
				pms = new HashMap();
				s_allactions.put("vipshopbuy", pms);
				
		// PPShop Info
				s_lists.add("ppshopinfo");
				pms = new HashMap();
				s_allactions.put("ppshopinfo", pms);
				
		// PPShop Buy
				s_lists.add("ppshopbuy");
				pms = new HashMap();
				s_allactions.put("ppshopbuy", pms);
				
		// PPShop Refresh
				s_lists.add("ppshoprefresh");
				pms = new HashMap();
				s_allactions.put("ppshoprefresh", pms);
		
		// Async Leaderboard
				s_lists.add("asyncleaderboard");
				pms = new HashMap();
				s_allactions.put("asyncleaderboard", pms);
		
		// Async Battle History
				s_lists.add("asyncbattlehistory");
				pms = new HashMap();
				s_allactions.put("asyncbattlehistory", pms);
		
		// Async Team Set
				s_lists.add("asyncteamset");
				pms = new HashMap();
				s_allactions.put("asyncteamset", pms);
				pms.put("heroid", "101011");
		
		// Async Battle
				s_lists.add("asyncbattle");
				pms = new HashMap();
				s_allactions.put("asyncbattle", pms);
	}
	

}
