using System;
using System.Collections.Generic;
using System.Text;

namespace com.cms.module
{
    public enum CMSNetState
    {
        Net_Init = 0,
        Net_Connecting,
        Net_ConnectFail,
        Net_LoginWait,
        Net_LoginFail,
        Net_LoginSucc
    }

    public enum CMSModuleState
    {
        CMS_Init = 0,
        CMS_LGSNet,
        CMS_GTSNet,
        CMS_Login,
        //选择完角色，初始化完成
        CMS_RoleReady,
    }

    public interface ICMSListener
    {
        void on_cms_netstate(CMSNetState state);
        void on_cms_login();
    }
}
