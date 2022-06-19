using System;
using System.Collections.Generic;
using System.Text;

namespace com.cms.client.network
{
    public class ConstValue
    {
        public static readonly int TCP_PACKAGE_MAX = 1024 * 16;    //16k
        public static readonly int TCP_SENDBUFF = TCP_PACKAGE_MAX * 2;
        public static readonly int TCP_RECVBUFF = TCP_PACKAGE_MAX * 2;

    }
}
