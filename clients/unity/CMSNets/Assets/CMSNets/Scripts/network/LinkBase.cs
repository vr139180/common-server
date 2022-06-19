using System;
using System.Collections.Generic;
using System.Text;

namespace com.cms.client.network
{
    public class LinkBase
    {
        private TCPConnector connector_ =new TCPConnector();
        private TCPAsyncSession session_ = null;

        private bool is_connected_ = false;

        public LinkBase(INetProcess np, IProtoFactory factory)
        {
            session_ = new TCPAsyncSession(factory);
            session_.NetProcess = np;

            connector_.Session = session_;
        }

        public bool Connected
        {
            get { return is_connected_; }
            set { is_connected_ = value; }
        }

        public void send_proto(Google.Protobuf.IMessage p)
        {
            session_.send_proto(p);
        }

        public void connect(string ip, int port)
        {
            if (this.Connected || is_connecting())
                return;

            is_connected_ = false;

            connector_.connect(ip, port);
        }

        public void force_close()
        {
            session_.force_reset();

            connector_.force_close();

            is_connected_ = false;
        }

        protected bool is_connecting()
        {
	        return connector_.is_connecting();
        }
    }
}
