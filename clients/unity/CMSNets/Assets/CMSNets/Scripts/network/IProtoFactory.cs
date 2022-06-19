using System;
using System.Collections.Generic;
using System.Text;

namespace com.cms.client.network
{
    public interface IProtoFactory
    {
        UInt16 proto_to_iid(Google.Protobuf.IMessage pro);
        Google.Protobuf.IMessage iid_to_proto(UInt16 proiid, byte[] pdata, int start, UInt16 len);
    }
}
