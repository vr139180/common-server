
SUBDIRS = .

AUTOMAKE_OPTIONS = foreign

noinst_PROGRAMS = ewwGateway

ewwGateway_CPPFLAGS = -I$(MYSRCTOP) -I$(MYLIBHPATH)

ewwGateway_LDADD =	\
					$(MYSRCTOP)/ewwLogSystem/libewwLogSystem.a \
					$(MYSRCTOP)/ewwLib/libewwLib.a \
					$(MYSRCTOP)/ewwGameLib/libewwGameLib.a \
 					$(MYLIBPATH)/libapr-1.a \
					$(MYLIBPATH)/libaprutil-1.a \
					$(MYLIBPATH)/libapriconv-1.a \
					$(MYLIBPATH)/libexpat.a \
					$(MYLIBPATH)/libboost_thread.a \
					$(MYLIBPATH)/libboost_system.a \
					$(MYLIBPATH)/libboost_filesystem.a \
					$(MYLIBPATH)/libboost_regex.a \
					$(MYLIBPATH)/libboost_date_time.a \
					$(MYLIBPATH)/libboost_chrono.a \
					$(MYLIBPATH)/libboost_atomic.a \
					-lrt \
					-lcurl \
					-ltcmalloc_minimal

#ewwGateway_DEPENDENCIES =

ewwGateway_SOURCES =./CTSCmdHandler.cpp \
					./CTSLink.cpp \
					./DGSCmdHandler.cpp \
					./DGSLink.cpp \
					./esMain.cpp \
					./ewwGatewayApp.cpp \
					./ewwGatewayApp_NetCmd.cpp \
					./GSSCmdHandler.cpp \
					./GSSLink.cpp \
					./GTSCmdHandler.cpp \
					./GTSStateMonitor.cpp \
					./gwAccount.cpp \
					./PlayerProxy.cpp

