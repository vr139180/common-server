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
    public class LGSConnector : INetProcess
    {
        private LinkBase connector_;
        private CMSNetState state_;
        private int error_code;

        private string ip_;
        private int port_;

        private int type = 0;
        private string account;
        private string pwd;
        private string token;
        private Int64 userid;

        public LGSConnector(AppProtoFactory factory)
        {
            connector_ = new LinkBase(this, factory);
            state_ = CMSNetState.Net_Init;
        }

        public void set_serverinfo( string ip, int port)
        {
            this.ip_ = ip;
            this.port_ = port;
        }

        public int get_error() { return error_code; }

        public void login_db( string account, string pwd)
        {
            if (!(this.state_ == CMSNetState.Net_Init || this.state_ == CMSNetState.Net_ConnectFail))
                return;

            this.state_ = CMSNetState.Net_Connecting;
            this.notify_event(this.state_);

            this.type = 1;
            this.account = account;
            this.pwd = pwd;
            connector_.connect(ip_, port_);
        }

        public void login_token( string token, Int64 uid)
        {
            if (!(this.state_ == CMSNetState.Net_Init || this.state_ == CMSNetState.Net_ConnectFail))
                return;

            this.state_ = CMSNetState.Net_Connecting;
            this.notify_event(this.state_);

            this.type = 2;
            this.token = token;
            this.userid = uid;
            connector_.connect(ip_, port_);
        }

        private void notify_event( CMSNetState state)
        {
            SysCommand1<CMSNetState> cmd =
                    new SysCommand1<CMSNetState>( state, CMSNetModule.instance().on_lgs_event);
            CMSNetModule.instance().regist_systemcmd(cmd);
        }

        //-----------------------------INetProcess----------------------------------
        public void on_connected_netthread(bool success, TCPAsyncSession sess)
        {
            if (success)
            {
                User_Login_req req = new User_Login_req();
                req.Type = this.type;
                if( req.Type == 1)
                {
                    req.Account = this.account;
                    req.Pwd = this.pwd;
                }
                else
                {
                    req.Token = this.token;
                    req.Userid = this.userid;
                }
                
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
            if( this.state_ != CMSNetState.Net_LoginSucc && this.state_ != CMSNetState.Net_Init)
            {
                this.state_ = CMSNetState.Net_LoginFail;
                this.notify_event(this.state_);
            }
        }

        public void on_recv_protocol_netthread(ushort iid, IMessage pro, TCPAsyncSession sess)
        {
            if (iid == (UInt16)USER_PROTYPE.UserLoginAck)
            {
                User_Login_ack ack = (User_Login_ack)pro;
                if( ack.Result == 0)
                {
                    CMSNetModule.instance().set_gtsinfo(ack.Proxyip, ack.Proxyport, 
                        ack.UserIid, ack.Proxytoken, ack.Slot);
                    this.state_ = CMSNetState.Net_LoginSucc;
                }
                else
                {
                    this.state_ = CMSNetState.Net_LoginFail;
                    this.error_code = ack.Result;
                }

                notify_event(this.state_);
            }
        }
    }
}