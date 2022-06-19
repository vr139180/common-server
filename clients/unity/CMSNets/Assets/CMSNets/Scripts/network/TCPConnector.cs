using System;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Text;

namespace com.cms.client.network
{
    public class TCPConnector
    {
        private SocketAsyncEventArgs conn_evt_ = null;
        private TCPAsyncSession session_ = null;

        private bool is_connecting_ = false;

        public TCPConnector()
        {

        }

        public TCPAsyncSession Session
        {
            set { this.session_ = value; }
        }

        public bool connect(string ip, int port)
        {
            if (is_connecting_)
                return false;

            IPEndPoint addr = null;
            try
            {
                addr = new IPEndPoint(IPAddress.Parse(ip), port);
            }
            catch (Exception)
            {
                return false;
            }

            lock (this)
            {
                Socket sock = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

                conn_evt_ = new SocketAsyncEventArgs();
                conn_evt_.UserToken = sock;
                conn_evt_.RemoteEndPoint = addr;
                conn_evt_.Completed += new EventHandler<SocketAsyncEventArgs>(this.on_connect_completed);

                is_connecting_ = true;

                if (!sock.ConnectAsync(this.conn_evt_))
                {
                    force_close(true);
                    return false;
                }
            }

            return true;
        }

        protected void on_connect_completed(object sender, SocketAsyncEventArgs e)
        {
            Socket sock = e.UserToken as Socket;
            if (sock != null && sock.Connected)
            {
                sock.NoDelay = true;

                sock.ReceiveBufferSize = cms.client.network.ConstValue.TCP_RECVBUFF;
                sock.SendBufferSize = cms.client.network.ConstValue.TCP_SENDBUFF;

                //sock.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.DontLinger, true);

                e.UserToken = null;

                session_.bind_session(sock, true);
            }
            else
            {
                session_.bind_session((Socket)e.UserToken, false);
            }

            force_close();
        }

        public void force_close( bool nolock =false)
        {
            is_connecting_ = false;

            if (nolock)
            {
                force_close_nolock();
            }
            else
            {
                lock (this)
                {
                    force_close_nolock();
                }
            }
        }

        protected void force_close_nolock()
        {
            is_connecting_ = false;

            if (conn_evt_ != null)
            {
                Socket s = (Socket)conn_evt_.UserToken;
                conn_evt_.Dispose();
                conn_evt_ = null;

                try
                {
                    if (s != null)
                        s.Shutdown(SocketShutdown.Both);
                }
                catch (Exception) { }

                if (s != null)
                {
#if UNITY_METRO
                    s.Dispose();
#elif UNITY_WINRT
                    s.Dispose();
#else
                    s.Close();
#endif
                }
            }

        }

        public bool is_connecting()
        {
            return is_connecting_;
        }


    }
}
