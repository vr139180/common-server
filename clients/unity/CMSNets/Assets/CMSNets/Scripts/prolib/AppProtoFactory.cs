/**
* protocol module
*
* auto generate file, don't change it.
*
* @category		
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2022
*/

using System;
using System.Collections.Generic;
using System.Text;
using System.Collections;
using com.cms.client.network;

namespace com.cms.client.network.proto
{
    public partial class AppProtoFactory : IProtoFactory
    {
        private Dictionary<Google.Protobuf.Reflection.MessageDescriptor, UInt16> name_2_iid = 
            new Dictionary<Google.Protobuf.Reflection.MessageDescriptor, UInt16>();
        private Dictionary<UInt16, Google.Protobuf.Reflection.MessageDescriptor> iid_2_pro = 
            new Dictionary<UInt16, Google.Protobuf.Reflection.MessageDescriptor>();

        private void regist_one( UInt16 iid, Google.Protobuf.IMessage msg)
        {
            name_2_iid.Add(msg.Descriptor, iid);
            iid_2_pro.Add(iid, msg.Descriptor);
        }

        public UInt16 proto_to_iid(Google.Protobuf.IMessage pro)
        {
            Google.Protobuf.Reflection.MessageDescriptor sname = pro.Descriptor;
            UInt16 iid = 0;
            try
            {
                if (!name_2_iid.TryGetValue(sname, out iid))
                    iid = 0;
            }
            catch (Exception){
                iid = 0;
            }

            return iid;
        }

        public Google.Protobuf.IMessage iid_to_proto(UInt16 proiid, byte[] pdata, int start, UInt16 len)
        {
            try
            {
                Google.Protobuf.Reflection.MessageDescriptor desc = null;

                if (!iid_2_pro.TryGetValue(proiid, out desc))
                    desc = null;

                if (desc == null)
                    return null;

                Type t = desc.ClrType;
                object obj = System.Activator.CreateInstance(t);
                Google.Protobuf.IMessage msg = (Google.Protobuf.IMessage)obj;

                Google.Protobuf.CodedInputStream cin = new Google.Protobuf.CodedInputStream(pdata, start, len);
                msg.MergeFrom(cin);

                return msg;
            }
            catch(Exception)
            {
                return null;
            }
        }
    }

}

