using System;
using System.Collections.Generic;
using System.Text;

using com.cms.client.network;

namespace com.cms.client.core
{
    public delegate void thread_command_adapter1(UInt16 iid, Google.Protobuf.IMessage pro);
    public delegate void thread_command_adapter2<T>(UInt16 iid, Google.Protobuf.IMessage pro, T p1);

    public delegate void thread_command_adapter3<T>( T p1);

    //basic command
    public abstract class CmdBase
    {
        public abstract void run_in_mainthread();
    }

    public class SysCommand1<T> : CmdBase
    {
        private T param_;
        private thread_command_adapter3<T> fun_ = null;

        public SysCommand1(T p, thread_command_adapter3<T> f)
        {
            this.param_ = p;
            this.fun_ = f;
        }

        public override void run_in_mainthread()
        {
            if (fun_ != null)
                fun_(this.param_);
        }
    }

    public class NetCommand1 : CmdBase
    {
        private UInt16 iid_;
        private Google.Protobuf.IMessage proto_ =null;
        private thread_command_adapter1 fun_ = null;

        public NetCommand1(UInt16 iid, Google.Protobuf.IMessage pro, thread_command_adapter1 fun)
        {
            this.iid_ = iid;
            this.proto_ = pro;
            this.fun_ = fun;
        }

        public override void run_in_mainthread()
        {
            if (fun_ != null)
                fun_(iid_,this.proto_);
        }
    }

    public class NetCommand2<T> : CmdBase
    {
        private UInt16 iid_;
        private Google.Protobuf.IMessage proto_ =null;
        private thread_command_adapter2<T> fun_ = null;
        private T param1_;

        public NetCommand2(UInt16 iid, Google.Protobuf.IMessage pro, thread_command_adapter2<T> fun, T p)
        {
            this.iid_ = iid;
            this.proto_ = pro;
            this.fun_ = fun;
            this.param1_ =p;
        }

        public override void run_in_mainthread()
        {
            if (fun_ != null)
                fun_(iid_, this.proto_, param1_);
        }
    }
}
