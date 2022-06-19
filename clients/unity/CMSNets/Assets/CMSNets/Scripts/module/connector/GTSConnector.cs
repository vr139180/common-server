using com.cms.client.core;
using com.cms.client.network;
using com.cms.client.network.gpro;
using com.cms.client.network.proto;
using Google.Protobuf;
using System;
using System.Collections.Generic;
using System.Text;

namespace com.cms.module.network
{
    public class GTSConnector : INetProcess
    {
        private LinkBase connector_;
        private CMSNetState state_;
        private int error_code;

        private string gts_ip_;
        private int gts_port_;
        private Int64 user_iid_;
        private Int32 slot_;
        private Int64 proxytoken_;

        public GTSConnector(AppProtoFactory factory)
        {
            connector_ = new LinkBase(this, factory);
            state_ = CMSNetState.Net_Init;
        }

        public void set_gtsinfo(string ip, int port, Int64 uid, Int64 tk, Int32 slot)
        {
            this.gts_ip_ = ip;
            this.gts_port_ = port;
            this.user_iid_ = uid;
            this.proxytoken_ = tk;
            this.slot_ = slot;
        }

        public void send_message(Google.Protobuf.IMessage p)
        {
            this.connector_.send_proto(p);
        }

        public void connect()
        {
            if (!(this.state_ == CMSNetState.Net_Init || this.state_ == CMSNetState.Net_ConnectFail))
                return;

            state_ = CMSNetState.Net_Connecting;
            this.connector_.connect(gts_ip_, gts_port_);
        }

        public int get_error() { return error_code; }

        private void notify_event(CMSNetState state)
        {
            SysCommand1<CMSNetState> cmd =
                    new SysCommand1<CMSNetState>(state, CMSNetModule.instance().on_gts_event);
            CMSNetModule.instance().regist_systemcmd(cmd);
        }

        //-----------------------------INetProcess----------------------------------
        public void on_connected_netthread(bool success, TCPAsyncSession sess)
        {
            if (success)
            {
                User_ProxyLogin_req req = new User_ProxyLogin_req();
                req.UserIid = this.user_iid_;
                req.Proxytoken = this.proxytoken_;
                req.Slot = this.slot_;

                sess.send_proto(req);

                this.state_ = CMSNetState.Net_LoginWait;
            }
            else
            {
                this.state_ = CMSNetState.Net_ConnectFail;
            }

            this.notify_event(this.state_);
        }

        public void on_connect_lost_netthread(TCPAsyncSession sess)
        {
            //除了成功之外都做失败处理
            if (this.state_ != CMSNetState.Net_LoginSucc && this.state_ != CMSNetState.Net_Init)
            {
                this.state_ = CMSNetState.Net_LoginFail;
                this.notify_event(this.state_);
            }
        }

        public void on_recv_protocol_netthread(ushort iid, IMessage pro, TCPAsyncSession sess)
        {
            if (iid == (UInt16)USER_PROTYPE.UserProxyloginAck)
            {
                User_ProxyLogin_ack ack = (User_ProxyLogin_ack)pro;
                if (ack.Result == 0)
                {
                    this.state_ = CMSNetState.Net_LoginSucc;
                }
                else
                {
                    this.state_ = CMSNetState.Net_LoginFail;
                    this.error_code = ack.Result;
                }

                notify_event(this.state_);
            }
            else
            {
                NetCommand2<TCPAsyncSession> cmd = new NetCommand2<TCPAsyncSession>(iid, pro,
                    CMSNetModule.instance().on_recv_protocol_netthread, sess);
                CMSNetModule.instance().regist_netcmd(cmd);
            }
        }
    }
}