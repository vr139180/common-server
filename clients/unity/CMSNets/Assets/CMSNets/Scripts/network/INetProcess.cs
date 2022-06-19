using System;
using System.Collections.Generic;
using System.Text;

namespace com.cms.client.network
{
    public interface INetProcess
    {
        void on_connected_netthread( bool success, TCPAsyncSession sess);

        void on_recv_protocol_netthread(UInt16 iid, Google.Protobuf.IMessage pro, TCPAsyncSession sess);

        void on_connect_lost_netthread( TCPAsyncSession sess);
    }
}
