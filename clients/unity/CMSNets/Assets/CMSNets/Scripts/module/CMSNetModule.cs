using com.cms.client.core;
using com.cms.client.network;
using com.cms.client.network.proto;
using com.cms.module.data;
using com.cms.module.network;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Text;

namespace com.cms.module
{
    public partial class CMSNetModule : MainThreadBase
    {
        private static CMSNetModule s_instance = new CMSNetModule();
        public static CMSNetModule instance() { return s_instance; }

        private AppProtoFactory factory_ = new AppProtoFactory();
        private LGSConnector lgs_connector_;
        private GTSConnector gts_connector_;

        private CMSModuleState cur_state_;

        public CMSNetModule()
        {
            factory_.init_protolib();
            this.init_net_funs();

            this.init_timers();

            this.lgs_connector_ = new LGSConnector(factory_);
            this.gts_connector_ = new GTSConnector(factory_);

            this.cur_state_ = CMSModuleState.CMS_Init;
        }

        public bool is_ready()
        {
            return this.cur_state_ == CMSModuleState.CMS_RoleReady;
        }

        #region gts and lgs server related

        public void set_loginserver( string ip, int port)
        {
            this.lgs_connector_.set_serverinfo(ip, port);
        }
        public void login_db( string acc, string pwd)
        {
            if (cur_state_ != CMSModuleState.CMS_Init)
                return;

            cur_state_ = CMSModuleState.CMS_LGSNet;
            this.lgs_connector_.login_db(acc, pwd);
        }
        public void login_token( string tk, Int64 uid)
        {
            if (cur_state_ != CMSModuleState.CMS_Init)
                return;

            cur_state_ = CMSModuleState.CMS_LGSNet;
            this.lgs_connector_.login_token(tk, uid);
        }

        public void set_gtsinfo(string ip, int port, Int64 uid, Int64 tk, Int32 slot)
        {
            this.gts_connector_.set_gtsinfo(ip, port, uid, tk, slot);
        }

        public void send_message(Google.Protobuf.IMessage p)
        {
            this.gts_connector_.send_message(p);
        }

        public void tick()
        {
            this.update(10);
            this.app_timer_.timer_tick();
        }

        protected void init_timers()
        {
            //20s 更新一下聊天频道激活
            this.add_apptimer(20 * 1000, this.timer_chatchannel_active);
            //15min 激活邮箱
            this.add_apptimer(15 * 60 * 1000, this.timer_mailbox_active);
        }

        public void timer_chatchannel_active(long tnow, int interval, long iid, ref bool finish)
        {
            //only do after role selected
            if (!is_ready())
                return;

            //send active message
            //没有注册任何频道，也需要定时激活
            send_message(this.build_chatchannelactive_msg());
        }

        public void timer_mailbox_active(long tnow, int interval, long iid, ref bool finish)
        {
            if (!is_ready())
                return;
        }

        #endregion

        #region chat channel related

        private List<ChatChannelInfoX> channels_actived_ = new List<ChatChannelInfoX>();

        public void clear_all_channels()
        {
            channels_actived_.Clear();
        }

        public void regist_one_channel( ChatChannelT t, Int64 cid)
        {
            for( int ii = 0; ii < this.channels_actived_.Count; ++ii)
            {
                ChatChannelInfoX x = this.channels_actived_[ii];
                if (x.type == t && x.channle_iid == cid)
                    return;
            }

            ChatChannelInfoX xi = new ChatChannelInfoX();
            xi.type = t;
            xi.channle_iid = cid;
            this.channels_actived_.Add(xi);
        }

        public void remove_one_channel( ChatChannelT t, Int64 cid)
        {
            for (int ii = 0; ii < this.channels_actived_.Count; ++ii)
            {
                ChatChannelInfoX x = this.channels_actived_[ii];
                if (x.type == t && x.channle_iid == cid)
                {
                    channels_actived_.RemoveAt(ii);
                    return;
                }
            }
        }

        #endregion

        #region event listener

        private List<ICMSListener> listeners_ = new List<ICMSListener>();

        public void add_listener(ICMSListener ls)
        {
            if (listeners_.Contains(ls))
                return;
            listeners_.Add(ls);
        }

        public void del_listener(ICMSListener ls)
        {
            listeners_.Remove(ls);
        }

        public void fire_cms_netstate(CMSNetState state)
        {
            for (int ind = 0; ind < listeners_.Count; ++ind)
                listeners_[ind].on_cms_netstate(state);
        }

        public void fire_cms_login()
        {
            for (int ind = 0; ind < listeners_.Count; ++ind)
                listeners_[ind].on_cms_login();
        }

        #endregion

        #region lgs and gts netevent , on main thread context

        public void on_lgs_event(CMSNetState state)
        {
            //成功之后开始连接gts
            if( state == CMSNetState.Net_LoginSucc)
            {
                cur_state_ = CMSModuleState.CMS_GTSNet;
                gts_connector_.connect();
            }

            fire_cms_netstate(state);
        }

        public void on_gts_event(CMSNetState state)
        {
            if( state == CMSNetState.Net_LoginSucc)
            {
                cur_state_ = CMSModuleState.CMS_Login;

                fire_cms_login();
            }
            else
            {
                fire_cms_netstate(state);
            }
        }

        public void on_recv_protocol_netthread(ushort iid, Google.Protobuf.IMessage pro, TCPAsyncSession sess)
        {
            thread_command_adapter1 fun = get_netfun(iid);
            if (fun != null)
                fun(iid, pro);
            else
            {
                Debug.WriteLine("recv netcommand:{0} no related function....", iid);
            }
        }

        #endregion
    }
}
