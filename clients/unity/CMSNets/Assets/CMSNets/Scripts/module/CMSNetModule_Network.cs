using com.cms.client.core;

using System;
using System.Collections.Generic;
using System.Text;
using System.Collections;
using com.cms.client.network;
using com.cms.client.network.gpro;
using com.cms.module.data;

namespace com.cms.module
{
    public partial class CMSNetModule : MainThreadBase
    {
        #region net function related
        private Dictionary<UInt16, thread_command_adapter1> net_funs_ = 
            new Dictionary<UInt16, thread_command_adapter1>();

        private void regist_netfun(UInt16 id, thread_command_adapter1 fun)
        {
            net_funs_.Add(id, fun);
        }

        private thread_command_adapter1 get_netfun( UInt16 id)
        {
            if (!net_funs_.ContainsKey(id))
                return null;
            return net_funs_[id];
        }

        #endregion

        private UserData user_data_ = new UserData();
        private UserMailBox mailbox_ = new UserMailBox();

        protected void init_net_funs()
        {
            regist_netfun((UInt16)USER_PROTYPE.UserRolelistAck, on_user_rolelist_ack);
            regist_netfun((UInt16)USER_PROTYPE.UserRoleselectAck, on_user_roleselect_ack);
        }

        //构造 chat channel 激活协议
        protected Chat_UserChannels_active build_chatchannelactive_msg()
        {
            Chat_UserChannels_active msg = new Chat_UserChannels_active();
            for( int ii =0; ii < this.channels_actived_.Count; ++ii)
            {
                ChatChannelInfoX xi = this.channels_actived_[ii];
                ChatChannelInfo cc = new ChatChannelInfo();
                cc.Type = xi.to_channeltype();
                cc.Channeldid = xi.channle_iid;
                msg.Channels.Add(cc);
            }

            return msg;
        }

        public void on_user_rolelist_ack(UInt16 iid, Google.Protobuf.IMessage pro)
        {
            User_RoleList_ack ack = (User_RoleList_ack)pro;
        }

        public void on_user_roleselect_ack(UInt16 iid, Google.Protobuf.IMessage pro)
        {
            User_RoleSelect_ack ack = (User_RoleSelect_ack)pro;
        }
    }
}
