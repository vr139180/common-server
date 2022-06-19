
SUBDIRS = .

AUTOMAKE_OPTIONS = foreign

noinst_LIBRARIES = libcmsLib.a

libcmsLib_a_CPPFLAGS = -DCURL_STATICLIB \
					   -I$(MYSRCTOP) -I$(MYLIBHPATH) -I$(MYLIBHPATH)/lua $(MYSDKLIBPATH)/zlib

libcmsLib_a_SOURCES = 	prolib/gen/Pro_all.cpp prolib/gen/ProtocolFactory.cpp \
						base/EwwSystem.cpp \
						base/Math.cpp \
						base/Profile.cpp \
						base/Vec.cpp \
						condition/ConditionElement.cpp \
						condition/ConditionLeaf.cpp \
						condition/ConditionNode.cpp \
						condition/ConditionValue.cpp \
						config/ASSOption.cpp \
						config/ConfigTool.cpp \
						config/CTSOption.cpp \
						config/DBSOption.cpp \
						config/DGSOption.cpp \
						config/FileConfigSourceImpl.cpp \
						config/GlobalConfig.cpp \
						config/GLSOption.cpp \
						config/GMTOption.cpp \
						config/GSSOption.cpp \
						config/GTSOption.cpp \
						config/HttpConfigSourceImpl.cpp \
						config/LGSOption.cpp \
						config/SvrTeamOption.cpp \
						encryption/md5/md5.cpp \
						encryption/rsa/RSAUtil.cpp \
						httpcurl/HttpClient.cpp \
						httpcurl/HTTPCurlRequest.cpp \
						httpcurl/HttpUrl.cpp \
						httpcurl/HttpUtilities.cpp \
						lua_src/lapi.c \
						lua_src/lauxlib.c \
						lua_src/lbaselib.c \
						lua_src/lcode.c \
						lua_src/ldblib.c \
						lua_src/ldebug.c \
						lua_src/ldo.c \
						lua_src/ldump.c \
						lua_src/lfunc.c \
						lua_src/lgc.c \
						lua_src/linit.c \
						lua_src/liolib.c \
						lua_src/llex.c \
						lua_src/lmathlib.c \
						lua_src/lmem.c \
						lua_src/loadlib.c \
						lua_src/lobject.c \
						lua_src/lopcodes.c \
						lua_src/loslib.c \
						lua_src/lparser.c \
						lua_src/lstate.c \
						lua_src/lstring.c \
						lua_src/lstrlib.c \
						lua_src/ltable.c \
						lua_src/ltablib.c \
						lua_src/ltm.c \
						lua_src/lua.c \
						lua_src/luac.c \
						lua_src/lundump.c \
						lua_src/lvm.c \
						lua_src/lzio.c \
						lua_src/print.c \
						luabind_src/class.cpp \
						luabind_src/class_info.cpp \
						luabind_src/class_registry.cpp \
						luabind_src/class_rep.cpp \
						luabind_src/create_class.cpp \
						luabind_src/error.cpp \
						luabind_src/exception_handler.cpp \
						luabind_src/function.cpp \
						luabind_src/inheritance.cpp \
						luabind_src/link_compatibility.cpp \
						luabind_src/object_rep.cpp \
						luabind_src/open.cpp \
						luabind_src/pcall.cpp \
						luabind_src/scope.cpp \
						luabind_src/stack_content_by_name.cpp \
						luabind_src/weak_ref.cpp \
						luabind_src/wrapper_base.cpp \
						net/LinkBase.cpp \
						net/MutexAllocator.cpp \
						net/NetAcceptor.cpp \
						net/NetConnector.cpp \
						net/NetDriverX.cpp \
						net/NetSession.cpp \
						os/linux/OSDriverLinux.cpp \
						prolib/BasicProtocol.cpp \
						script/ScriptContext.cpp \
						serialization/base64/base64.cpp \
						serialization/cjson/cJSON.c \
						system/AppStateMonitor.cpp \
						system/CommandBase.cpp \
						system/TimerContainer.cpp \
						system/VirtualMainThread.cpp \
						tinyxml/tinystr.cpp \
						tinyxml/tinyxml.cpp \
						tinyxml/tinyxmlerror.cpp \
						tinyxml/tinyxmlparser.cpp \
						util/FileLogOnlyOnce.cpp \
						util/FileUtil.cpp \
						util/ShareUtil.cpp \
						util/XmlUtil.cpp \
						./OSDriver.cpp \
						./ServerAppBase.cpp \
						./ServerAppWrapper.cpp \
						./ThreadLock.cpp \
						./Version.cpp

