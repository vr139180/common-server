using System;
using System.Collections.Generic;
using System.IO;
using System.Net;
using System.Net.Sockets;

namespace com.cms.client.network
{
    public class TCPAsyncSession
    {
        #region callback related

        //socket event pool
        //custom net event callback
        private INetProcess net_process_ = null;
        //proto factory
        private IProtoFactory proto_factory_ = null;

        #endregion

        //bind client socket
        private Socket socket_ = null;
        private int b_init_ =0;
        private object lock_ = new object();

        //send something
        private object send_lock_ = new object();
        private bool is_sending_ = false;
        private int send_pos_ = 0;
        private int send_len_ = 0;

        //send list
        private List<Google.Protobuf.IMessage> send_list_ = new List<Google.Protobuf.IMessage>();
        private SocketAsyncEventArgs send_event_ = null;

        //recv something
        private int recv_pos_ = 0;
        private SocketAsyncEventArgs recv_event_ = null;

        public TCPAsyncSession( IProtoFactory factory)
        {
            this.proto_factory_ = factory;
        }

        public INetProcess NetProcess
        {
            set { this.net_process_ = value; }
        }

        public bool bind_session( Socket client, bool success)
        {
            if (client == null)
                return false;

            if (!success)
            {
                lock (lock_)
                {
                    if (net_process_ != null)
                        net_process_.on_connected_netthread(false, this);

                    try
                    {
#if UNITY_METRO
                        client.Dispose();
#elif UNITY_WINRT
                        client.Dispose();
#else
                        client.Close();
#endif
                    }
                    catch (Exception) { }
                }

                return true;
            }

            lock (send_lock_)
            {
                send_list_.Clear();
            }

            lock (lock_)
            {
                reset_nolock();

                this.socket_ = client;

                //save local and remote ip information
                try
                {
                    this.LocalIP = (IPEndPoint)socket_.LocalEndPoint;
                    this.RemoteIP = (IPEndPoint)socket_.RemoteEndPoint;
                }
                catch (Exception) { }

                b_init_ = 0;

                this.send_event_ = new SocketAsyncEventArgs();
                this.send_event_.SetBuffer(new byte[cms.client.network.ConstValue.TCP_SENDBUFF], 0, cms.client.network.ConstValue.TCP_SENDBUFF);

                this.send_event_.Completed += new EventHandler<SocketAsyncEventArgs>(this.on_send_completed);

                this.recv_event_ = new SocketAsyncEventArgs();
                this.recv_event_.SetBuffer(new byte[cms.client.network.ConstValue.TCP_RECVBUFF], 0, cms.client.network.ConstValue.TCP_RECVBUFF);

                this.recv_event_.Completed += new EventHandler<SocketAsyncEventArgs>(this.on_recv_completed);

                try
                {
                    b_init_ = 1;

                    if (net_process_ != null)
                        net_process_.on_connected_netthread(true, this);

                    this.begin_read();
                }
                catch (Exception)
                {
                    b_init_ = 0;
                }
            }

            return b_init_ == 1;
        }

#region socket related

        public IPEndPoint LocalIP { get; private set; }
        public IPEndPoint RemoteIP { get; private set; }

        public bool active
        {
            get
            {
                return this.b_init_ == 1;
            }
        }

        public void on_connect_lost_netthread()
        {
            lock (lock_)
            {
                if (b_init_ != 1)
                    return;

                this.reset_nolock();
            }

            if (net_process_ != null)
                net_process_.on_connect_lost_netthread(this);
        }

#endregion

#region init and reset

        private void reset_nolock()
        {
            if (b_init_ != 1)
                return;

            this.b_init_ = 0;
            this.is_sending_ = false;

            this.send_pos_ = 0;
            this.send_len_ = 0;
            this.recv_pos_ = 0;

            try
            {
                this.send_event_.Dispose();
                this.recv_event_.Dispose();

                this.send_event_ = null;
                this.recv_event_ = null;

                if (socket_ != null)
                    socket_.Shutdown(SocketShutdown.Both);
            }
            catch (Exception)
            {
            }

            if (socket_ != null)
            {
#if UNITY_METRO
                socket_.Dispose();
#elif UNITY_WINRT
                socket_.Dispose();
#else
                socket_.Close();
#endif
            }

            socket_ = null;
        }

        public void force_reset()
        {
            lock (send_lock_)
            {
                send_list_.Clear();
            }

            lock (lock_)
            {
                this.reset_nolock();
            }
        }

#endregion


#region read function

        protected void begin_read()
        {
            bool completedAsync = true;
            try
            {
                lock (this.lock_)
                {
                    this.recv_event_.SetBuffer(this.recv_pos_, (cms.client.network.ConstValue.TCP_RECVBUFF - this.recv_pos_));
                    completedAsync = this.socket_.ReceiveAsync(this.recv_event_);
                }
            }
            catch (Exception)
            {
                this.on_connect_lost_netthread();
            }

            if (!completedAsync)
                this.on_recv_completed(this, this.recv_event_);
        }

        private void on_recv_completed(object sender, SocketAsyncEventArgs e)
        {
            if (e.SocketError != SocketError.Success || e.BytesTransferred < 1)
            {
                //disconnect
                this.on_connect_lost_netthread();
                return;
            }

            List<Google.Protobuf.IMessage> recvs = new List<Google.Protobuf.IMessage>();
            List<UInt16> readproiids = new List<UInt16>();
            bool read_err = false;

            lock (lock_)
            {
                if (b_init_ != 1)
                    return;

                this.recv_pos_ += e.BytesTransferred;

                int len = this.analy_package_nomutex(readproiids, recvs);

                if (len < 0)
                    read_err = true;

                if (len > 0)
                {
                    Buffer.BlockCopy(this.recv_event_.Buffer, len, this.recv_event_.Buffer, 0, recv_pos_ - len);
                    recv_pos_ -= len;
                }
            }

            if (read_err)
                on_connect_lost_netthread();
            else
            {
                for (int ii = 0; ii < recvs.Count; ++ii)
                {
                    Google.Protobuf.IMessage pr = recvs[ii];
                    UInt16 iid = readproiids[ii];

                    if (net_process_ != null)
                        net_process_.on_recv_protocol_netthread(iid, pr, this);
                }

                //continue to read
                this.begin_read();
            }
        }

        //#define NET_HEAD_MASK_ZERO_H	0x0000FFFF
        //#define NET_HEAD_MASK_ZERO_L	0xFFFF0000
        protected void get_nethead( UInt32 dat, out UInt16 proid, out UInt16 len)
        {
            len = (UInt16)((dat & 0xFFFF0000) >> 16);
            proid = (UInt16)(dat & 0x0000FFFF);
        }

        protected void make_nethead(UInt16 proid, UInt16 len, out UInt32 dat)
        {
            dat = len;
            dat = (dat << 16) & 0xFFFF0000;
            UInt32 p = proid;
            dat = dat|(p & 0x0000FFFF);
        }

        protected int analy_package_nomutex(List<UInt16> readproiids, List<Google.Protobuf.IMessage> readpro)
        {
            int reduce_len = 0;

            if (recv_pos_ < sizeof(UInt32))
                return reduce_len;

            byte[] rbuff = this.recv_event_.Buffer;

            do
            {
                int curpos = reduce_len;
                int curlen = recv_pos_ - reduce_len;
                if (curlen < sizeof(UInt32))
                    break;

                UInt32 len2 = BitConverter.ToUInt32(rbuff, curpos);
                UInt16 proiid = 0, llen = 0;
                get_nethead(len2, out proiid, out llen);

                if ( (llen + sizeof(UInt32)) > cms.client.network.ConstValue.TCP_PACKAGE_MAX)
                    return -1;

                if (curlen - sizeof(UInt32) < llen)
                    break;

                Google.Protobuf.IMessage pro = null;
                try
                {
                    pro = proto_factory_.iid_to_proto( proiid, rbuff, reduce_len + sizeof(UInt32), llen);
                }
                catch (Exception)
                {
                }

                if (pro == null)
                {
                    readpro.Clear();
                    readproiids.Clear();
                    return -1;
                }
                else
                {
                    readpro.Add(pro);
                    readproiids.Add(proiid);
                }

                reduce_len += (llen + sizeof(UInt32));
            }
            while (reduce_len < recv_pos_);

            return reduce_len;
        }

#endregion

#region send and recv function

        public virtual void send_proto(Google.Protobuf.IMessage pro)
        {
            if (b_init_ != 1) return;

            lock (send_lock_)
            {
                this.send_list_.Add(pro);
            }

            bool write_err = false;

            lock (lock_)
            {
                if (b_init_ != 1)
                    return;

                write_err = (!try_write_nomutext());
            }

            if (write_err)
                this.on_connect_lost_netthread();
        }

        private void on_send_completed(object sender, SocketAsyncEventArgs e)
        {
            if (e.SocketError != SocketError.Success || e.BytesTransferred < 0)
            {
                this.on_connect_lost_netthread();
                return;
            }

            bool write_err = false;

            lock (lock_)
            {
                is_sending_ = false;

                if (b_init_ != 1)
                    return;

                this.send_pos_ += e.BytesTransferred;
                this.send_len_ -= e.BytesTransferred;

                write_err = (!try_write_nomutext());
            }

            if (write_err)
                on_connect_lost_netthread();
        }

        protected bool try_write_nomutext()
        {
            if (is_sending_)
                return true;

            // move to header
            if (send_pos_ > 0)
            {
                if (send_len_ > 0)
                    Buffer.BlockCopy(this.send_event_.Buffer, send_pos_, this.send_event_.Buffer, 0, send_len_);
                send_pos_ = 0;
            }

            if (!fill_writebuffer_nomutex())
                return false;

            if (send_len_ > 0)
            {
                is_sending_ = true;

                var completedAsync = true;
                try
                {
                    //copy data to send buffer
                    this.send_event_.SetBuffer(0, send_len_);
                    completedAsync = this.socket_.SendAsync(this.send_event_);
                }
                catch (Exception)
                {
                    this.on_connect_lost_netthread();
                    is_sending_ = false;

                    return false;
                }

                if (!completedAsync)
                    this.on_send_completed(this, this.send_event_);
            }

            return true;
        }

        protected bool fill_writebuffer_nomutex()
        {
            byte[] sbuf = this.send_event_.Buffer;

            while ((cms.client.network.ConstValue.TCP_SENDBUFF - (send_pos_ + send_len_ + 4)) > 0 && send_list_.Count > 0)
            {
                Google.Protobuf.IMessage pro = null;
                int curpos = send_pos_ + send_len_;

                lock (send_lock_)
                {
                    if (send_list_.Count > 0)
                    {
                        pro = send_list_[0];
                        send_list_.RemoveAt(0);
                    }
                }

                if (pro == null)
                    break;

                UInt16 llen = (UInt16)pro.CalculateSize();
                UInt16 proiid = proto_factory_.proto_to_iid(pro);
                if( proiid == 0)
                {
                    return false;
                }

                //单包过大
                if ((llen+ sizeof(UInt32)) > cms.client.network.ConstValue.TCP_PACKAGE_MAX)
                {
                    //logError(out_net, "NetSession::fill_writebuffer_nomutex msg->SerializeToArray( pbuf, len) failed, maxsize=%d prosize=%d", len, len2);
                    return false;
                }

                //移动到数据区
                curpos += 4;

                bool bwrite = true;
                try
                {
                    MemoryStream ms = new MemoryStream(sbuf, curpos, llen);
                    Google.Protobuf.CodedOutputStream cout = new Google.Protobuf.CodedOutputStream(ms);
                    pro.WriteTo(cout);
                    cout.Flush();
                }
                catch(Exception)
                {
                    bwrite = false;
                }
                
                if( !bwrite)
                {
                    // push back
                    lock (send_lock_)
                    {
                        send_list_.Insert(0, pro);
                    }

                    return false;
                }

                // set len
                UInt32 wlen = 0;
                make_nethead(proiid, llen, out wlen);
                cms.client.utils.CommonUtil.uint2bytes(sbuf, curpos - 4, wlen);

                //proto content len
                send_len_ += 4;
                //proto len
                send_len_ += llen;
            }

            return true;
        }

#endregion


    }

}
